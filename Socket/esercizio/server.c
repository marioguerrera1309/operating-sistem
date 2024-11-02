/**
Utilizzare i socket, di tipo stream, nel dominio di Internet per implementare un sistema di votazione. (Suggerimento: usare il localhost con l’indirizzo di loopback).

Il server, dopo aver atteso la connessione  del client, 
	- lo interroga inviandogli un messaggio contenente la stringa “CAMPIONA”
	- ne attende la risposta, che può essere la stringa “SOTTO_SOGLIA” o la stringa “SOGLIA_SUPERATA”
	- stampa a video la stringa ricevuta
	- chiude la connessione con il client.

Il client, dopo essersi connesso al server, 
	- attende il messaggio di interrogazione del server
	- se il messaggio contiene “CAMPIONA”, risponderà “SOTTO_SOGLIA” se il valore restituito dalla funzione time(NULL) è pari, altrimenti risponderà “SOGLIA_SUPERATA”
	- chiude la connessione e termina.
*/
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
int main() {
	int x;
	int tcp_socket;
	char buf[50];
	tcp_socket=socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_socket==-1) {
		printf("Errore creazione socket\n");
		exit(1);
	}
	struct sockaddr_in addr_in;
	addr_in.sin_family=AF_INET;
	addr_in.sin_port=htons(40000);
	addr_in.sin_addr.s_addr=htonl(INADDR_ANY);
	x=bind(tcp_socket, (struct sockaddr*)&addr_in, sizeof(addr_in));
	if(x==-1) {
		printf("Errore bind\n");
		exit(1);
	}
	x=listen(tcp_socket, 1);
	if(x==-1) {
		printf("Errore listen\n");
		exit(1);
	}
	x=accept(tcp_socket, NULL, NULL);
	if(x==-1) {
		printf("Errore accept\n");
		exit(1);
	}
	printf("Ricevuta richiesta\n");
	sleep(5);
	x=read(tcp_socket, buf, 50);
	if(x==-1) {
		printf("Errore read\n");
		perror("Erorre: ");
		exit(1);
	}
	buf[x]='\0';
	printf("%s\n", buf);
	close(tcp_socket);
}
