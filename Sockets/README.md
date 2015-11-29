# Práctica 2 - Sockets

Implementar una aplicación cliente-servidor mediante la utilización de sockets cuya
funcionalidad será la ejecución remota de comandos sin parámetros. El ejercicio
constará de dos partes (procesos): el cliente que enviará el comando a ejecutar al
servidor y el servidor que recibirá la petición desde el cliente, ejecutará el comando y
devolverá los resultados al cliente.

El cliente se ejecutará en la máquina local y su función es establecer la comunicación
con el servidor, enviar al servidor el comando, recibir los resultados y mostrarlos por
pantalla. Para ello, el usuario escribirá en el prompt el comando:

**ClienteRemoto IP_Servidor Comando**

El servidor se lanzará con la orden ServidorRemoto y debe estar en todo momento
escuchando por el puerto 9999, preparado para aceptar conexiones. Tras una petición de
conexión por parte de un cliente, debe crear un hijo que será el encargado de realizar
todas las operaciones necesarias relacionadas con la ejecución de comando solicitado
por el cliente.

La puntuación del ejercicio será la siguiente:

	# Implementación correcta del cliente y su estructura. (1,5 puntos).
	# Implementación correcta del servidor y su estructura. (1,5 puntos).
	# Ejecución remota de comandos. (3 puntos).
