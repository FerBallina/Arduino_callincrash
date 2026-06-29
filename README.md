- investigar mas en https://www.luisllamas.es/arduino-orientacion-imu-mpu-6050/#leer-inclinación-con-acelerómetro



# Arduino_callincrash

Este proyecto muestra una forma sencilla de usar un Arduino Nano con un sensor MPU-6050 para detectar una desaceleración fuerte y enviar una alerta a una aplicación Android.

## Idea
- El Arduino lee los datos del acelerómetro del MPU-6050 por I2C.
- Calcula la magnitud total de la aceleración.
- Si detecta un valor por encima de un umbral configurable, envía un mensaje de alerta por Bluetooth.
- La app Android recibe ese mensaje y puede enviar un SMS, mostrar una notificación o iniciar una llamada.

## Hardware
- Arduino Nano
- Módulo MPU-6050
- Módulo Bluetooth HC-05 o HC-06
- Cables de conexión

## Conexiones básicas
- MPU-6050:
  - VCC -> 5V
  - GND -> GND
  - SDA -> A4
  - SCL -> A5
- Bluetooth HC-05/HC-06:
  - RX -> D2
  - TX -> D3
  - VCC -> 5V
  - GND -> GND

## Ajuste del umbral
El umbral se puede cambiar desde el puerto serie o desde el módulo Bluetooth enviando un comando como:
- THRESHOLD=3.5

Valores más altos reducen falsos positivos, valores más bajos hacen el sistema más sensible.

## Archivos
- [arduino/MPU6050_fall_alert/MPU6050_fall_alert.ino](arduino/MPU6050_fall_alert/MPU6050_fall_alert.ino): firmware del Arduino.
- [android/AlertReceiver.kt](android/AlertReceiver.kt): ejemplo de recepción por Bluetooth en Android.
- [android/README.md](android/README.md): guía para integrar la app Android.

## Notas importantes
- Este ejemplo es un prototipo de detección y no reemplaza un sistema médico o de seguridad profesional.
- Para una solución más robusta conviene combinar aceleración, giroscopio y un algoritmo de caída.
- En producción se recomienda usar un servicio de notificación real y validar permisos y seguridad de la app.
