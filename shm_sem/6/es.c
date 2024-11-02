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
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/wait.h>
union semun {
       int              val;    /* Value for SETVAL */
       struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
       unsigned short  *array;  /* Array for GETALL, SETALL */
       struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                   (Linux-specific) */
};
struct memoria {
	int t, u, c;
};
int down(int semid) {
	int x;
	struct sembuf s;
	s.sem_num=0;
	s.sem_op=-1;
	s.sem_flg=0;
	x=semop(semid, &s, 1);
	return x;
}
int up(int semid) {
	int x;
	struct sembuf s;
	s.sem_num=0;
	s.sem_op=1;
	s.sem_flg=0;
	x=semop(semid, &s, 1);
	return x;
}
int set(int semid, int val) {
	int x;
	x=semctl(semid, 0, SETVAL, val);
	return x;
}
int main() {
	int x, r;
	pid_t figli[3];
	int semid1, semid2, semid3, shmid;
	struct memoria *mem;
	semid1=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	semid2=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	semid3=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);	
	x=set(semid1, 1);
	x=set(semid2, 1);
	x=set(semid3, 1);
	shmid=shmget(IPC_PRIVATE, sizeof(struct memoria), IPC_CREAT | 0666);
	mem=(struct memoria*)shmat(shmid, NULL, 0);
	mem->t=0;
	mem->u=0;
	mem->c=0;
	for(int i=0;i<3;i++) {
		figli[i]=fork();
		switch(figli[i]) {
			case 0:
				srand(getpid());
				if(i==0) {
					for(int j=0;j<20;j++) {
						//temp
						r=(rand()%40);
						x=down(semid1);
						mem->t=r;
						x=up(semid1);
						sleep(1);												
					}
					x=down(semid1);
					mem->t=-1;
					x=up(semid1);
					exit(0);
				} else if(i==1) {
					for(int j=0;j<20;j++) {
						//umidità
						r=(rand()%80)+10;
						x=down(semid2);
						mem->u=r;
						x=up(semid2);
						sleep(1);												
					}
					x=down(semid2);
					mem->u=-1;
					x=up(semid2);
					exit(0);
				} else if(i==2) {
					for(int j=0;j<20;j++) {
						//consumo
						r=(rand()%500)+100;
						x=down(semid3);
						mem->c=r;
						x=up(semid3);
						sleep(1);												
					}
					x=down(semid3);
					mem->c=-1;
					x=up(semid3);
					exit(0);
				}
			default:
				break;
		}
	}
	x=down(semid1);
	x=down(semid2);
	x=down(semid3);
	do {
		x=up(semid1);
		x=up(semid2);
		x=up(semid3);
		sleep(2);
		x=down(semid1);
		x=down(semid2);
		x=down(semid3);
		if(mem->c!=-1 && mem->t!=-1 && mem->u!=-1)
			printf("Temperatura: %d\nUmidità: %d\nConsumo: %d\n", mem->t, mem->u, mem->c);
	} while(mem->c!=-1 && mem->t!=-1 && mem->u!=-1);
	for(int i=0;i<3;i++) {
		wait(0);
	}
	exit(0);
}
