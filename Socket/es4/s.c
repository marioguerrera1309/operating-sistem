/*
Implementare un'applicazione che offra un servizio di ECHO utilizzando
i socket nel dominio di internet.

Si devono iplementare sia un server che un client.

I client richiedono delle stringhe da tastiera e le inviano al server.

Il server risponde trasmettendo ad ognuno dei client con la stessa stringa inviata
dal dato client.

Se il client invia la stringa esci il server termina la connessione
con il client.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pthread.h>
void *funzione(void *args) {
	int *csock, x;
	char buf[50];
	csock=(int*)args;
	while(1) {
		x=read(*csock, buf, 50);
		if(x>0) {
			buf[x]='\0';
			if(strncmp(buf, "esci", 4)==0) {
				close(*csock);
				pthread_exit(0);
			} else {
				x=write(*csock, buf, strlen(buf)+1);
				if(x<=0) {
					printf("Errore invio\n");
				}
			}
		}
	}
}
int main() {
	int x, sock, csock;
	pthread_t thread;
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9875);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	sock=socket(AF_INET, SOCK_STREAM, 0);
	x=bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	if(x==-1) {
		printf("Errore bind\n");
		exit(1);
	}
	x=listen(sock, 5);
	if(x==-1) {
		printf("Errore listen\n");
		exit(1);
	}
	x=0;
	while(1) {
		csock=accept(sock, NULL, NULL);
		pthread_create(&thread, 0, funzione, (void*)&csock);
		x++;
		printf("Host collegati: %d\n", x);
	}
}
