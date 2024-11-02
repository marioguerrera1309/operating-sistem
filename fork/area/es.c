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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
struct memoria {
	int base;
	int altezza;
};
int main() {
	printf("Inizio\n");
	key_t keym, keys;
	keym=ftok("/home/mario/Scrivania/sistemioperativi/fork/area", 0);
	keys=ftok("/home/mario/Scrivania/sistemioperativi/fork/area", 1);
	int x;
	struct memoria *mem;
	int shmid, semid;
	shmid=shmget(keym, sizeof(struct memoria), IPC_CREAT | 0666);	
	if(shmid==-1) {
		printf("Errore shmget\n");
		exit(1);
	}
	mem=(struct memoria*)shmat(shmid, 0, 0666);
	semid=semget(keys, 1, IPC_CREAT | 0666);
	if(semid==-1) {
		printf("Errore semget\n");
		exit(1);
	}
	shmdt(mem);
	shmctl(shmid, IPC_RMID, 0);
	semctl(semid, 0, IPC_RMID);
	printf("Fine\n");
}
