#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<pthread.h>
struct dati {//struct che conterrà l' ultimo messaggio
	char messaggio[100];
	char utente[50];
	int id;
}ultimomessaggio;
pthread_mutex_t mutex;
void *funzione (void *args) {
	int *csock, len, x;
	char buf[500], utente[50];
	csock=(int *)args;
	while(1) {
		len=read(*csock, buf, 500);
		if(len<=0) {
			pthread_exit(0);
		} else {
			buf[len]='\0';
			if(strncmp(buf, "register", 8)==0) {
				strcpy(utente, &(buf[9]));
				printf("Si è registrato %s\n", utente);
				sprintf(buf, "Benvenuto %s\n", utente);
				x=write(*csock, buf, strlen(buf));
				if(x<=0) {
					printf("Errore invio messaggio\n");
				}
				pthread_mutex_lock(&mutex);
				strcpy(ultimomessaggio.messaggio, "register");
				strcpy(ultimomessaggio.utente, utente);
				(ultimomessaggio.id)++;
				pthread_mutex_unlock(&mutex);
			} else if(strncmp(buf, "send", 4)==0) {
				pthread_mutex_lock(&mutex);
				strcpy(ultimomessaggio.messaggio, &(buf[5]));
				strcpy(ultimomessaggio.utente, utente);
				(ultimomessaggio.id)++;
				pthread_mutex_unlock(&mutex);
				sprintf(buf, "ok");
				x=write(*csock, buf, strlen(buf));
				if(x<=0) {
					printf("Errore invio messaggio\n");
				}
			} else if(strncmp(buf, "read", 4)==0) {
				pthread_mutex_lock(&mutex);
				sprintf(buf, "%d %s: %s", ultimomessaggio.id, ultimomessaggio.utente, ultimomessaggio.messaggio);
				pthread_mutex_unlock(&mutex);
				x=write(*csock, buf, strlen(buf));
				if(x<=0) {
					printf("Errore invio messaggio\n");
				}
			} else if(strncmp(buf, "close", 5)==0){
				close(*csock);
			} else {
				printf("Errore\n");
				pthread_exit(0);
			}
		}
	}
}
int main() {
	int sock, csock, x;
	pthread_t thread;
	pthread_mutex_init(&mutex, 0);
	struct sockaddr_in addr;
	pthread_mutex_lock(&mutex);
	(ultimomessaggio.id)=0;
	pthread_mutex_unlock(&mutex);
	sock=socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9876);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
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
	while(1) {
		csock=accept(sock, NULL, NULL);
		printf("Un host si è collegato\n");
		x=pthread_create(&thread, 0, funzione, (void*)&csock);
		pthread_detach(thread);
	}
}
