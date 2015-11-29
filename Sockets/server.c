#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 9999
#define SIZEQ 5
#define SIZE 512

int connector = -1;

void liberarPuerto() {
	if(connector != -1) { //Hacer solo si esta creado
		puts("LIBERANDO SOCKET");
		shutdown(connector,2); //http://www.gnu.org/software/libc/manual/html_node/Closing-a-Socket.html
		close(connector);
	}
	exit(0);
}

int main(int argc, char const *argv[]) {
	int connector2, socketSize, readBytes;
	struct sockaddr_in server, client;
	char buffer[SIZE];
	socketSize = sizeof(struct sockaddr_in);

	if((connector = socket(AF_INET, SOCK_STREAM, 0)) != -1) {
		signal(SIGINT, liberarPuerto); //Si el programa recibe un ^C, liberamos el puerto antes de salir
		signal(SIGCHLD, SIG_IGN); //Cuando el hijo que ejecute un comando muera, no pasa nada
		puts("SOCKET OK");
		server.sin_family = AF_INET; //Familia (protocolo)
		server.sin_port = htons(PORT); //Puerto
		server.sin_addr.s_addr = INADDR_ANY; //IP

		if(bind(connector,(struct sockaddr*) &server, sizeof(struct sockaddr)) != -1 && listen(connector, SIZEQ) != -1) {
			puts("PREPARADO PARA RECIBIR CONEXIONES");
			while(1) {
				if((connector2 = accept(connector,(struct sockaddr*) &client,(socklen_t*) &socketSize)) != -1) {
					puts("NUEVA CONEXION ACEPTADA");
					puts("LEYENDO DEL SOCKET");
					readBytes = read(connector2,buffer,SIZE);
					buffer[readBytes] = '\0'; //Solo mostramos hasta donde leemos
					
					if(fork() == 0) { //HIJO
						puts("EJECUTANDO COMANDO EN SEGUNDO PLANO");
						dup2(connector2, STDOUT_FILENO); //Ponemos como salida estándar el socket
						dup2(connector2, STDERR_FILENO); //Lo mismo con la salida de errores
						close(connector2);
						system(buffer); //Ejecutamos el comando
						buffer[0] = '\0'; //Por si acaso
						exit(0);
					}
					close(connector2); //Cerramos descriptores para las peticiones resueltas; si no, se amontonan
				}
				else puts("ERROR AL INTENTAR ACEPTAR LA CONEXION");
			}
		}
		else puts("ERROR AL ABRIR EL PUERTO");
	}
	else puts("NO SE PUEDE CREAR EL SOCKET");

	return 0;
}
