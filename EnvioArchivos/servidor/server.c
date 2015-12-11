#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define PORT 9999
#define SIZEQ 5
#define SIZE 512
#define MAXKB 20000000

int connector = -1;

void liberarPuerto() {
	if(connector != -1) { //Hacer solo si esta creado
		puts("\nLIBERANDO SOCKET");
		close(connector);
	}
	exit(0);
}

void sendFile(const char fileName[], int socketDescriptor) {
	char buffer[SIZE];
	struct stat st;
	int fileDescriptor, readBytes, percent, oldpercent;
	double fileSize, totalBytes, remainData;
	totalBytes = percent = oldpercent = 0;

	fileDescriptor = open(fileName,O_RDONLY);
	if(fileDescriptor != -1) {
		//Opcional hasta el do-while
		stat(fileName, &st); //Recojemos todos los datos del fichero que hemos abierto
		fileSize = st.st_size; //Nos guardamos el numero de bytes que ocupa el archivo
		remainData = fileSize;
		//Antes de enviar el archivo, enviamos el tamaño que tiene
		snprintf(buffer,SIZE,"%g",fileSize); //Ponemos el tamaño del fichero en el buffer
		write(socketDescriptor,buffer,strlen(buffer)); //Enviamos el buffer al cliente
		read(socketDescriptor,buffer,SIZE); //Esperamos a que el cliente envíe una señal. WORKAROUND DE LA PARRA
		do {
			readBytes = read(fileDescriptor,buffer,SIZE);
			//Muestra el % de lo que envia de 5 en 5. Opcional
			totalBytes += readBytes;
			remainData -= readBytes;
			percent = (totalBytes*100/fileSize);
			if(percent % 5 == 0 && percent > oldpercent) {
				oldpercent = percent;
				if(fileSize < MAXKB) printf("[SERVIDOR] %g%gKB ENVIADOS (%d%%)\n", totalBytes/1024, fileSize/1024, percent);
				else printf("[SERVIDOR] %g/%gMB ENVIADOS (%d%%)\n", (totalBytes/1024)/1024, (fileSize/1024)/1024, percent);
			}
			if(readBytes > 0) write(socketDescriptor,buffer,readBytes);
		} while(readBytes > 0);
		puts("[SERVIDOR] TERMINADO DE LEER Y ENVIAR EL ARCHIVO");
		close(fileDescriptor);
		close(socketDescriptor);
	}
	else puts("[SERVIDOR] NO SE HA PODIDO ABRIR EL ARCHIVO");
}

int main(/*int argc, char const *argv[]*/) {
	int connector2, socketSize, readBytes;
	struct sockaddr_in server, client;
	char buffer[SIZE];
	socketSize = sizeof(struct sockaddr_in);

	if((connector = socket(AF_INET, SOCK_STREAM, 0)) != -1) {
		puts("[SERVIDOR] SOCKET OK");
		signal(SIGINT, liberarPuerto); //Si el programa recibe un ^C, liberamos el puerto antes de salir
		signal(SIGCHLD, SIG_IGN); //Cuando el hijo que ejecute un comando muera, no pasa nada
		server.sin_family = AF_INET; //Familia (protocolo)
		server.sin_port = htons(PORT); //Puerto
		server.sin_addr.s_addr = INADDR_ANY; //IP

		if(bind(connector,(struct sockaddr*) &server, sizeof(struct sockaddr)) != -1 && listen(connector, SIZEQ) != -1) {
			puts("[SERVIDOR] PREPARADO PARA RECIBIR CONEXIONES");
			while(1) {
				if((connector2 = accept(connector,(struct sockaddr*) &client,(socklen_t*) &socketSize)) != -1) {
					puts("[SERVIDOR] NUEVA CONEXION ACEPTADA");
					puts("[SERVIDOR] LEYENDO DEL SOCKET");
					readBytes = read(connector2,buffer,SIZE);
					buffer[readBytes] = '\0'; //Solo mostramos hasta donde leemos
					
					if(fork() == 0) { //HIJO
						puts("[SERVIDOR] ENVIANDO ARCHIVO EN SEGUNDO PLANO");
						sendFile(buffer,connector2);
						puts("[SERVIDOR] CONEXION FINALIZADA");
						close(connector2); //Necesario?
						exit(0);
					}
					close(connector2); //Cerramos descriptores para las peticiones resueltas; si no, se amontonan
				}
				else puts("[SERVIDOR] ERROR AL INTENTAR ACEPTAR LA CONEXION");
			}
		}
		else puts("[SERVIDOR] ERROR AL ABRIR EL PUERTO");
	}
	else puts("[SERVIDOR] NO SE PUEDE CREAR EL SOCKET");

	return 0;
}
