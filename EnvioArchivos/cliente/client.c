#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define PORT 9999
#define SIZEQ 5
#define SIZE 512
#define MAXKB 20000000

void receiveFile(const char fileName[], int socketDescriptor) {
	char buffer[SIZE];
	struct stat st;
	int fileDescriptor, readBytes, percent, oldpercent;
	double fileSize, totalBytes, remainData;
	totalBytes = percent = oldpercent = 0;

	fileDescriptor = creat(fileName,0666);
	if(fileDescriptor != -1) {
		//Antes de recibir el archivo, recibimos el tamaño que ocupa. Opcional
		read(socketDescriptor,buffer,SIZE);
		write(socketDescriptor,"1",strlen("1")); //Enviamos una señal al servidor para que sepa que hemos cogido el tamaño del archivo. WORKAROUND DE LA PARRA
		fileSize = atof(buffer);
		remainData = fileSize;
		do {
			readBytes = read(socketDescriptor,buffer,SIZE);
			//Muestra el % de lo que envia de 5 en 5. Opcional
			totalBytes += readBytes;
			remainData -= readBytes;
			percent = (totalBytes*100/fileSize);
			if(percent % 5 == 0 && percent > oldpercent) {
				oldpercent = percent;
				if(fileSize < MAXKB) printf("[CLIENTE] %g/%gKB RECIBIDOS (%d%%)\n", totalBytes/1024, fileSize/1024, percent);
				else printf("[CLIENTE] %g/%gMB RECIBIDOS (%d%%)\n", (totalBytes/1024)/1024, (fileSize/1024)/1024, percent);
			}
			if(readBytes > 0) write(fileDescriptor,buffer,readBytes);
		} while(readBytes > 0);
		puts("[CLIENTE] TERMINADO DE RECIBIR Y ESCRIBIR EL ARCHIVO");
		close(fileDescriptor);
		close(socketDescriptor);
	}
	else puts("[CLIENTE] NO SE HA PODIDO CREAR EL ARCHIVO");
}

int main(int argc, char const *argv[]) {
	int connector, connector2;
	struct sockaddr_in server;
	if(argc == 4) {
		if((connector = socket(AF_INET,SOCK_STREAM,0)) > 0) {
			puts("[CLIENTE] SOCKET OK");
			server.sin_family = AF_INET; //Familia (protocolo)
			server.sin_port = htons(PORT); //Puerto
			server.sin_addr.s_addr = inet_addr(argv[1]); //IP

			if((connector2 = connect(connector,(struct sockaddr*) &server, sizeof(struct sockaddr))) != -1) {
				puts("[CLIENTE] CONECTADO");
				write(connector,argv[2],SIZE);
				puts("[CLIENTE] RECIBIENDO ARCHIVO");
				receiveFile(argv[3], connector);
				close(connector2); //Cerramos la conexion
			}
			else puts("[CLIENTE] ERROR AL CONECTAR");
			close(connector); //Cerramos el socket. El puerto se queda abierto
		}
		else puts("[CLIENTE] ERROR AL CREAR EL SOCKET");
	}
	else puts("ARGUMENTOS: <DIRECCION IP> <ARCHIVO SOLICITADO> <NOMBRE ARCHIVO GUARDADO>");
	
	return 0;
}
