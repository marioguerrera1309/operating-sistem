/*
Scrivere un programma in C su Linux in cui un processo principale (P0) genera un processo figlio P1.
P0 e P1 condividono tramite shared memory due variabili intere (base e altezza) che rappresentano, rispettivamente, la lunghezza della base e dell'altezza di un triangolo.

P0, ciclicamente e per 4 volte, 
	- chiede all’utente di inserire due interi da tastiera (base e altezza)
	- attende che P1 abbia calcolato l'area del triangolo
Dopo le 4 iterazioni, P0 
	- attende la terminazione del processo figlio
	- elimina shared memory e semafori
	- termina esso stesso
	
Il processo P1, ciclicamente, per 4 volte
	- attende che P0 abbia aggiornato le variabili condivise (l’attesa deve essere implementata utilizzando i semafori)
	- calcola l’area del triangolo e la stampa a video
Dopo 4 iterazioni P1 termina.

Utilizzare il meccanismo dei semafori sia per sincronizzare i processi e sia per regolare l’accesso in sezione critica.
*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
int down(int semid) {
	struct sembuf s;
	int x;
	s.sem_num=0;
	s.sem_op=-1;
	s.sem_flg=0;
	x=semop(semid, &s, 1);
	return x;
}
int up(int semid) {
	struct sembuf s;
	int x;
	s.sem_num=0;
	s.sem_op=1;
	s.sem_flg=0;
	x=semop(semid, &s, 1);
	return x;
}
struct memoria {
	int base;
	int altezza;
};
int main() {
	pid_t figlio;
	struct memoria *mem;
	int x, b, a;
	int shmid, semid1, semid2;
	key_t key[3];
	for(int i=0;i<3;i++) {
		key[i]=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/shm3", i);
	}
	shmid=shmget(key[0], sizeof(struct memoria), IPC_CREAT | 0666);
	mem=(struct memoria*)shmat(shmid, 0, 0666);
	semid1=semget(key[1], 1, IPC_CREAT | 0666);//sezione critica
	semctl(semid1, 0, SETVAL, 1);
	semid2=semget(key[2], 1, IPC_CREAT | 0666);//sinc
	semctl(semid2, 0, SETVAL, 0);
	figlio=fork();
	switch(figlio) {
		case 0:
			for(int i=0;i<4;i++) {
				x=down(semid2);
				if(x==-1) {
					printf("Errore down sinc\n");	
				}
				x=up(semid1);
				if(x==-1) {
					printf("Errore up\n");
				}
				printf("Area: %d\n", (mem->base)*(mem->altezza));
				x=down(semid1);
				if(x==-1) {
					printf("Errore down\n");
				}
				x=up(semid2);
				if(x==-1) {
					printf("Errore up sinc\n");
				}
			}
			shmdt(mem);
			exit(0);
			break;
	}
	x=up(semid2);
	for(int i=0;i<4;i++) {
		x=down(semid2);
		if(x==-1) {
			printf("Errore down sinc\n");	
		}
		printf("Inserisci base: ");
		scanf("%d", &b);
		printf("Inserisci altezza: ");
		scanf("%d", &a);
		x=up(semid1);
		if(x==-1) {
			printf("Errore up\n");
		}
		mem->base=b;
		mem->altezza=a;
		x=down(semid1);
		if(x==-1) {
			printf("Errore down\n");
		}
		x=up(semid2);
		if(x==-1) {
			printf("Errore up sinc\n");	
		}
	}
	wait(0);
	shmdt(mem);
	shmctl(shmid, IPC_RMID, 0);
	semctl(semid1, IPC_RMID, 0);
	semctl(semid2, IPC_RMID, 0);
}
