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
int main() {
	int x, sock;
	char buf[50];
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9875);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock=socket(AF_INET, SOCK_STREAM, 0);
	x=connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	if(x==-1) {
		printf("Errore connect\n");
		exit(1);
	}
	while(1) {
		printf("Inserisci stringa (esci per uscire): ");
		scanf("%s", buf);
		x=write(sock, buf, strlen(buf));
		x=read(sock, buf, 50);
		buf[x]='\0';
		if(x<=0) {
			printf("Errore\n");
		} else if(strncmp(buf, "esci", 4)==0) {
			printf("Uscita in corso\n");
			break;
		} else {
			printf("Stringa ricevuta: %s\n", buf);
		}
	}
	close(sock);
}
