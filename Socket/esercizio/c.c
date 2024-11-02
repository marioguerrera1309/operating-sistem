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

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<time.h>
int main() {
	int x, sock, len;
	char buf[50];
	sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock==-1) {
		printf("Errore socket\n");
		exit(1);
	}
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9876);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	x=connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	if(x==-1) {
		printf("Errore connect\n");
		exit(1);
	}
	len=0;
	x=time(NULL);
	while(len==0) {
		len=read(sock, buf, 50);
		if(len>0) {
			buf[len]='\0';
			if(strcmp(buf, "CAMPIONA")==0) {
				printf("Messaggio ricevuto: %s\n", buf);				
			}
			if(x%2==0) {
				sprintf(buf, "SOTTO_SOGLIA");
				if(write(sock, buf, strlen(buf))>0) {
					printf("Messaggio inviato\n");
				}
			} else {
				sprintf(buf, "SOPRA_SOGLIA");
				if(write(sock, buf, strlen(buf))>0) {
					printf("Messaggio inviato\n");
				}
			}
		}
	}
}
