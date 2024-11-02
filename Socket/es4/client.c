/*
 *	Implementare un'applicazione che offra un servizio di ECHO utilizzando
 *	i socket nel dominio di internet.
 *
 *	Si devono iplementare sia un server che un client.
 *
 *	I client richiedono delle stringhe da tastiera e le inviano al server.
 *
 *	Il server risponde trasmettendo ad ognuno dei client con la stessa stringa inviata
 *	dal dato client.
 *
 *	Se il client invia la stringa "esci" il server termina la connessione
 *	con il client.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LOCALHOST "127.0.0.1"
#define PORT 50000

#define MSG_SIZE 200
 
int main() {
	int sockfd;
	struct sockaddr_in addr;
	char msg[MSG_SIZE];
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(LOCALHOST);
	
	if(connect(sockfd, (struct sockaddr *)(&addr), sizeof(struct sockaddr_in)) == -1) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	
	while(strcmp(msg, "esci\n") != 0) {
		printf("Invia stringa: ");
		fgets(msg, MSG_SIZE, stdin);
		send(sockfd, (void *)(msg), sizeof(msg), 0);
		recv(sockfd, (void *)(msg), sizeof(msg), 0);
		printf("Ricevuta stringa: %s\n", msg);
	}
	
	if(close(sockfd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	
	printf("Termino esecuzione...\n");
	
	exit(EXIT_SUCCESS);
}
