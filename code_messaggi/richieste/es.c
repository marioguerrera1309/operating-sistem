#include<stdio.h>
#include<string.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#define N 10
#define X 50
struct dati {
	int command;
	int value;
};
struct messaggio {
	long mtype;
	struct dati dato;
};
void funzione_figlio(int num) {
	int msqid;
	key_t key;
	struct messaggio msg;
	key=ftok("/home/mario/Scrivania/sistemioperativi/code_messaggi/richieste", 0);
	msqid=msgget(key, 0);
	int cont=0;
	while(msg.dato.command!=2) {
		msgrcv(msqid, &msg, sizeof(struct dati), num, 0);
		if(msg.dato.command==0) {
			msg.mtype=1;
			msg.dato.command=1;
			cont++;
			msg.dato.value=cont;
			msgsnd(msqid, &msg, sizeof(struct dati), 0);
		}
	}
	exit(0);
}
int main() {
	pid_t figlio;
	key_t key;
	int msqid, j;
	struct messaggio msg;
	for(int i=0;i<N;i++) {
		figlio=fork();
		switch(figlio) {
			case -1:
				printf("Errore\n");
				exit(1);
			case 0: 
				funzione_figlio(i+2);
				exit(1);
			default: 
				break;
		}
	}
	key=ftok("/home/mario/Scrivania/sistemioperativi/code_messaggi/richieste", 0);	
	msqid=msgget(key, 0666 | IPC_CREAT);
	for(int i=0;i<X;i++) {
		for(j=0;j<N;j++) {
			msg.mtype=j+2;
			msg.dato.command=0;//richiesta
			msgsnd(msqid, &msg, sizeof(struct dati), 0);
			msgrcv(msqid, &msg, sizeof(struct dati), 1, 0);
			printf("Figlio: %d valore: %d\n", j, msg.dato.value);
		}
	}
	for(int i=0;i<N;i++) {
		msg.mtype=j+2;
		msg.dato.command=2;//stop
		msgsnd(msqid, &msg, sizeof(struct dati), 0);
	}
	msgctl(msqid, IPC_RMID, 0);
}
