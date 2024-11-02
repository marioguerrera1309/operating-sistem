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
void *funzione (void* args) {
	int *sock, len, x, vid=-1, nid=0;
	char buf[500];
	sock=(int *)args;
	while(1) {
		sleep(1);
		sprintf(buf, "read");
		x=write(*sock, buf, strlen(buf)+1);
		if(x<=0) {
			printf("Errore invio messaggio\n");
		}
		while(nid!=vid) {
			len=read(*sock, buf, 500);
			if(len>0) {
				buf[len]='\0';
				sscanf(buf, "%d %s: %s", &(ultimomessaggio.id), ultimomessaggio.utente, ultimomessaggio.messaggio);
				nid=ultimomessaggio.id;
				printf("%d %s: %s\n", ultimomessaggio.id, ultimomessaggio.utente, ultimomessaggio.messaggio);
			}
		}
	}
}
int main() {
	int x, sock;
	char buf[500], nome[50];
	pthread_t thread;
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9876);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock=socket(AF_INET, SOCK_STREAM, 0);
	x=connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	if(x==-1) {
		printf("Errore connect\n");
		exit(0);
	}
	sleep(1);
	pthread_create(&thread, 0, funzione, &sock);
	printf("Inserisci nome: ");
	scanf("%s", nome);
	sprintf(buf, "register %s", nome);
	x=write(sock, buf, strlen(buf)+1);
	if(x<=0) {
		printf("Errore invio messaggio\n");
	}
	while(1) {
		printf("Inserisci comando: ");
		scanf("%s", buf);
		x=write(sock, buf, strlen(buf)+1);
		if(x<=0) {
			printf("Errore invio messaggio\n");
		}
		if(strncmp(buf, "close", 5)==0) {
			break;
		}
	}
	close(sock);
}
