package com.example.fallalert

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothSocket
import android.os.Bundle
import android.telephony.SmsManager
import android.widget.Toast
import java.io.InputStream
import java.util.UUID

class MainActivity : Activity() {
    private val uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
    private lateinit var socket: BluetoothSocket
    private lateinit var inputStream: InputStream

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        Thread {
            val adapter = BluetoothAdapter.getDefaultAdapter()
            if (adapter == null) {
                runOnUiThread { Toast.makeText(this, "El dispositivo no tiene Bluetooth", Toast.LENGTH_LONG).show() }
                return@Thread
            }

            val device = adapter.bondedDevices.firstOrNull {
                it.name.contains("HC-05", ignoreCase = true) || it.name.contains("HC-06", ignoreCase = true)
            }

            if (device == null) {
                runOnUiThread { Toast.makeText(this, "Empareja un módulo HC-05/HC-06 primero", Toast.LENGTH_LONG).show() }
                return@Thread
            }

            socket = device.createRfcommSocketToServiceRecord(uuid)
            socket.connect()
            inputStream = socket.inputStream

            val buffer = ByteArray(256)
            while (true) {
                val bytesRead = inputStream.read(buffer)
                val message = String(buffer, 0, bytesRead).trim()
                if (message.contains("FALL_ALERT")) {
                    runOnUiThread {
                        Toast.makeText(this, "¡Golpe fuerte detectado!", Toast.LENGTH_LONG).show()
                        sendAlertSms(message)
                    }
                }
            }
        }.start()
    }

    private fun sendAlertSms(message: String) {
        val phoneNumber = "YOUR_CONTACT_NUMBER"
        val smsManager = SmsManager.getDefault()
        smsManager.sendTextMessage(phoneNumber, null, "Alerta de caída: $message", null, null)
    }
}
