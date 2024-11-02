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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define LOCALHOST "127.0.0.1"
#define PORT 50000

#define MSG_SIZE 200

void *connection_handler(void *arg);
 
int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in addr;
	pthread_t lwpid;
	pthread_attr_t thread_attr;
	int *sockfdp;
	
	if(pthread_attr_init(&thread_attr) != 0) {
		perror("pthread_attr_init");
		exit(EXIT_FAILURE);
	}
	if(pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
		perror("pthread_attr_setdetachstate");
		exit(EXIT_FAILURE);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		perror("sockfd");
		exit(EXIT_FAILURE);
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(LOCALHOST);
	
	if(bind(sockfd, (struct sockaddr *)(&addr), sizeof(struct sockaddr_in)) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	if(listen(sockfd, 5) == -1) {
		perror("lsiten");
		exit(EXIT_FAILURE);
	}
	
	while(1) {
		sockfdp = (int *)(malloc(sizeof(int)));
		
		*sockfdp = accept(sockfd, NULL, NULL);
		if(*sockfdp == -1) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
	
		pthread_create(&lwpid, &thread_attr, connection_handler, sockfdp);
	}
	
	if(pthread_attr_destroy(&thread_attr) != 0) {
		perror("pthread_attr_destroy");
		exit(EXIT_FAILURE);
	}
	
	if(close(sockfd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	
	printf("Termino esecuzione...\n");
	
	exit(EXIT_SUCCESS); 
}

void *connection_handler(void *arg) {
	int sockfd;
	char msg[MSG_SIZE];
	
	sockfd = *(int *)(arg);
	free(arg);
	
	while(strcmp(msg, "esci\n") != 0) {
		recv(sockfd, (void *)(msg), sizeof(msg), 0);
		msg[strlen(msg) - 1] = '\0';
		printf("Il thread %ld ha ricevuto la stringa \"%s\"\n", pthread_self(), msg);
		msg[strlen(msg)] = '\n';
		send(sockfd, (void *)(msg), sizeof(msg), 0);
	}
	
	if(close(sockfd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	
	printf("Il thread %ld chiude la connessione...\n", pthread_self());
	
	pthread_exit(NULL);
}
