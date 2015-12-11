# Práctica 4 - Transferencia de archivos remotos 

Implementar una aplicación cliente-servidor mediante la utilización de sockets cuya
funcionalidad será la transferencia de archivos remotos. El ejercicio constará de dos
partes (procesos):

1. **El cliente** que enviará al servidor el nombre del archivo a transferir, recibirá los
datos del archivo desde el servidor a través del socket y los mostrará por
pantalla.
2. **El servidor** que recibirá la petición desde el cliente del nombre del archivo a
transferir y transferirá la información del archivo al cliente a través del socket.

El cliente y el servidor deben estar en máquinas distintas o, en su defecto, deben
ejecutarse en terminales y directorios distintos.
El cliente se ejecutará con la orden siguiente:

**Cliente IP_Servidor Archivo**

El servidor se lanzará con la orden **Servidor** y debe estar en todo momento escuchando
por el puerto 9999, preparado para aceptar conexiones. Tras una petición de conexión
por parte de un cliente, debe crear un hijo que será el encargado de realizar todas las
operaciones necesarias relacionadas con la transferencia del archivo solicitado por el
cliente.
