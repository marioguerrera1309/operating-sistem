#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
#include<netinet/in.h>
#include<netinet/ip.h>
struct messaggio {
	char messaggio[140];
	char utente[30];
	int id;
};
struct messaggio last;
pthread_mutex_t mutex;
void *worker_thread(void *args) {
	int sock=(int)args;
	char buff[500];
	char txbuff[500];
	char nome[30];
	int len;
	int id=0;
	while(1) {
		len=read(sock, buff, 500);
		if(len==0) {
			pthread_exit(NULL);
		} else if(len>0) {
			buff[len]='\0';
			if(strncmp(buff, "register", 8)==0) {
				strcpy(nome, &(buff[9]));
				sprintf(txbuff, "Welcome %s", &(buff[9]));
				write(sock, txbuff, strlen(txbuff));
					
			} else if(strncmp(buff, "send", 4)==0) {
				pthread_mutex_lock(&mutex);
				strcpy(last.utente, nome);
				strcpy(last.messaggio, &(buff[5]));
				id++;
				last.id=id;
				pthread_mutex_unlock(&mutex);
				write(sock, "ok", 2);
			} else if(strncmp(buff, "read", 4)==0) {
				pthread_mutex_lock(&mutex);
				sprintf(txbuff, "%d %s: %s", last.id , last.utente, last.messaggio);
				write(sock, txbuff, strlen(txbuff));
			} else if(strncmp(buff, "close", 5)==0) {
				close(sock);
				pthread_exit(NULL);
			} else {
				printf("Errore\n");
			}
		}
	}	 	
}
int main() {
	//csock sd del client
	int sock, csock;
	int x;
	pthread_t tid;
	last.id=-1;
	pthread_mutex_init(&mutex, 0);
	struct sockaddr_in serv_addr;
	sock=socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(40000);
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	//casting di sockaddr_in -> sockaddr
	x=bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(x==0) {
		printf("Bind eseguita\n");
	} else if(x==-1) {
		perror("Errore Bind\n");
		exit(1);
	} else {
		perror("Errore generico\n");
		exit(1);
	}
	listen(sock, 10);
	while(1) {
		csock=accept(sock, NULL, NULL);
		printf("Nuova connessione stabilita\n");
		x=pthread_create(&tid, NULL, worker_thread, csock);
		if(x!=0) {
			printf("Errore\n");
		}
		settiamo detached il thread
		x=pthread_detach(tid);
		if(x!=0) {
			printf("Errore\n");
		}
	}
}
