#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define PORT 9999
#define SIZEQ 5
#define SIZE 512

int main(int argc, char const *argv[]) {
	int i, connector, connector2, readBytes;
	struct sockaddr_in server;
	char buffer[SIZE], args[SIZE];

	if(argc >= 3) {
		args[0] = '\0';
		for(i = 2; i < argc; i++) {
			if(i == 2) sprintf(args,"%s%s", args, argv[i]);
			else sprintf(args,"%s %s", args, argv[i]);
		}
		printf("CMD: %s\n", args);

		if((connector = socket(AF_INET,SOCK_STREAM,0)) > 0) {
			puts("SOCKET OK");
			server.sin_family = AF_INET; //Familia (protocolo)
			server.sin_port = htons(PORT); //Puerto
			server.sin_addr.s_addr = inet_addr(argv[1]); //IP

			if((connector2 = connect(connector,(struct sockaddr*) &server, sizeof(struct sockaddr))) != -1) {
				puts("CONECTADO AL SERVIDOR");
				write(connector,args,SIZE);
				puts("LEYENDO RESPUESTA DEL SERVIDOR");
				do {
					readBytes = read(connector,buffer,512);
					buffer[readBytes] = '\0';
					printf("%s", buffer);
				} while(readBytes > 0);
				close(connector2); //Cerramos la conexion
			}
			else puts("ERROR AL CONECTAR AL SERVIDOR");
			shutdown(connector,2); //http://www.gnu.org/software/libc/manual/html_node/Closing-a-Socket.html
			close(connector); //Cerramos el socket. El puerto se queda abierto
		}
		else puts("ERROR AL CREAR EL SOCKET");
	}
	else puts("INTRODUCE UNA IP Y UN COMANDO");
	
	return 0;
}
