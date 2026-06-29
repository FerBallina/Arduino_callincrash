#include <Wire.h>
#include <SoftwareSerial.h>

const uint8_t MPU6050_ADDRESS = 0x68;
const uint8_t PWR_MGMT_1 = 0x6B;
const uint8_t ACCEL_CONFIG = 0x1C;
const uint8_t ACCEL_XOUT_H = 0x3B;
const uint8_t ACCEL_YOUT_H = 0x3D;
const uint8_t ACCEL_ZOUT_H = 0x3F;

const float BASELINE_G = 1.0f;
float fallThresholdG = 3.0f; // configurable via Serial/Bluetooth command
const unsigned long ALERT_COOLDOWN_MS = 5000;

SoftwareSerial btSerial(2, 3); // RX, TX

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

int16_t readAccelerometerAxis(uint8_t reg) {
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDRESS, 2, true);

  uint8_t high = Wire.read();
  uint8_t low = Wire.read();
  return (int16_t)((high << 8) | low);
}

float readAxisG(uint8_t reg) {
  return readAccelerometerAxis(reg) / 16384.0f;
}

void parseThresholdCommand(String command) {
  command.trim();
  if (command.startsWith("THRESHOLD=")) {
    float newThreshold = command.substring(10).toFloat();
    if (newThreshold > 0.0f) {
      fallThresholdG = newThreshold;
      Serial.print("Threshold set to ");
      Serial.print(fallThresholdG, 2);
      Serial.println(" g");
      btSerial.print("Threshold set to ");
      btSerial.print(fallThresholdG, 2);
      btSerial.println(" g");
    }
  }
}

void processCommands() {
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    parseThresholdCommand(command);
  }

  while (btSerial.available()) {
    String command = btSerial.readStringUntil('\n');
    parseThresholdCommand(command);
  }
}

void sendAlert(float magnitudeG) {
  static unsigned long lastAlertAt = 0;
  unsigned long now = millis();

  if (now - lastAlertAt < ALERT_COOLDOWN_MS) {
    return;
  }

  lastAlertAt = now;

  Serial.print("FALL_ALERT magnitudeG=");
  Serial.println(magnitudeG, 2);
  btSerial.print("FALL_ALERT magnitudeG=");
  btSerial.println(magnitudeG, 2);
}

void setup() {
  Serial.begin(115200);
  btSerial.begin(9600);
  Wire.begin();

  writeRegister(PWR_MGMT_1, 0x00);
  writeRegister(ACCEL_CONFIG, 0x00); // +/- 2g
  delay(100);

  Serial.println("MPU6050 fall alert ready");
  Serial.println("Send THRESHOLD=3.5 to change the trigger level");
  btSerial.println("MPU6050 fall alert ready");
}

void loop() {
  processCommands();

  float axG = readAxisG(ACCEL_XOUT_H);
  float ayG = readAxisG(ACCEL_YOUT_H);
  float azG = readAxisG(ACCEL_ZOUT_H);
  float magnitudeG = sqrt(axG * axG + ayG * ayG + azG * azG);

  bool trigger = false;

  if (magnitudeG > (BASELINE_G + fallThresholdG) || magnitudeG < (BASELINE_G - fallThresholdG)) {
    trigger = true;
  }

  if (trigger) {
    sendAlert(magnitudeG);
  }

  delay(20);
}
