/**
 * Scrivere un programma in C che permetta la raccolta dati
 * da una serie di sensori (3 sensori).
 * 
 * Ogni sensore è rappresentato da un processo/thread.
 * - Il primo sensore periodicamente e ogni secondo monitora
 *   la temperatura di una stanza.
 * - Il secondo sensore, periodicamente e ogni secondo, monitora
 *   l'umidità di una stanza.
 * - Il terzo sensore, periodicamente e ogni secondo, monitora
 *   il consumo energetico.
 * 
 * Tutti i sensori inviano i propri valori al
 * main thread/processo padre che si occuperà di stampare a video
 * i valori dei tre sensori ogni 2 secondi.
 * 
 * Dopo 20 secondi di esecuzione tutti i processi/thread sensori
 * imposteranno il valore del sensore a -1 per comunicare al main
 * thread/processo padre che hanno terminato.
 * 
 * Il processo padre/main thread, quando leggerà che il valore di
 * tutti e tre i sensori è uguale a -1 termina.
 * 
 * Utilizzare thread/mutex oppure processi/shared memory/semafori
 * per l'implementazione del programma.
**/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/wait.h>
struct memoria {
	int t, u, c;
};
struct msgbuf {
	long mtype;
	struct memoria mem;
};
int main() {
	int x, r;
	int msgid;
	struct msgbuf msg, mess[3];
	msgid=msgget(IPC_PRIVATE, IPC_CREAT | 0666);
	pid_t figli[3];
	for(int i=0;i<3;i++) {
		figli[i]=fork();
		switch(figli[i]) {
			case 0:
				srand(getpid());
				if(i==0) {
					for(int j=0;j<20;j++) {
						r=(rand()%40);
						msg.mtype=1;
						msg.mem.t=r;
						msgsnd(msgid, &msg, sizeof(struct memoria), 0);
						sleep(1);	
					}
					msg.mtype=1;
					msg.mem.t=-1;
					msgsnd(msgid, &msg, sizeof(struct memoria), 0);
					exit(0);
				} else if(i==1) {
					for(int j=0;j<20;j++) {
						r=(rand()%40);
						msg.mtype=2;
						msg.mem.u=r;
						msgsnd(msgid, &msg, sizeof(struct memoria), 0);
						sleep(1);	
					}
					msg.mtype=2;
					msg.mem.u=-1;
					msgsnd(msgid, &msg, sizeof(struct memoria), 0);
					exit(0);	
				} else if(i==2) {
					for(int j=0;j<20;j++) {
						r=(rand()%40);
						msg.mtype=3;
						msg.mem.c=r;
						msgsnd(msgid, &msg, sizeof(struct memoria), 0);
						sleep(1);	
					}
					msg.mtype=3;
					msg.mem.c=-1;
					msgsnd(msgid, &msg, sizeof(struct memoria), 0);
					exit(0);
				}
		}
	}
	do {
		sleep(2);
		msgrcv(msgid, &mess[0], sizeof(struct memoria), 1, 0);
		msgrcv(msgid, &mess[1], sizeof(struct memoria), 2, 0);
		msgrcv(msgid, &mess[2], sizeof(struct memoria), 3, 0);
		if(mess[0].mem.t!=-1 && mess[1].mem.u!=-1 && mess[2].mem.c!=-1)
			printf("Temperatura: %d\nUmidità: %d\nConsumo: %d\n", mess[0].mem.t, mess[1].mem.u, mess[2].mem.c);
	} while(mess[0].mem.t!=-1 && mess[1].mem.u!=-1 && mess[2].mem.c!=-1);
	for(int i=0;i<3;i++) {
		wait(0);
	}
	msgctl(msgid, IPC_RMID, 0);
	exit(0);
}
