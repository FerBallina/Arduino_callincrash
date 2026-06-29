# Android side sketch

Este es un esqueleto mínimo para una app Android que escucha mensajes por Bluetooth Classic desde un módulo HC-05/HC-06.

## Pasos
1. Crea un proyecto Android Studio con una Activity vacía.
2. Copia el contenido de AlertReceiver.kt a MainActivity.kt.
3. Empareja el módulo HC-05/HC-06 con el teléfono.
4. Reemplaza YOUR_CONTACT_NUMBER por un número válido.
5. Ejecuta la app y mantén el dispositivo cerca del módulo Bluetooth.

## Permisos
Añade estos permisos en AndroidManifest.xml:
- android.permission.BLUETOOTH
- android.permission.BLUETOOTH_CONNECT
- android.permission.SEND_SMS

> Si no quieres enviar SMS, puedes dejar solo la parte de Toast/Notificación.
