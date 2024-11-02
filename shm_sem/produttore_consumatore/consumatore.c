/*
Sviluppare due processi (un consumatore e un produttore) che condividono una memoria (shared
memory). Si supponga che i due processi NON siano imparentati.
Si supponga che la memoria condivisa sia realizzata dal seguente struct:
#define N 100
typedef struct {
unsigned int n;
float v[N];
} calcolo;
Si suppone che all’inizio il produttore chieda all'utente di immettere da tastiera un numero intero
n
maggiore di zero. Il processo produttore scrive questo numero nel campo
dello struct condiviso. Il
consumatore legge questo numero n.
Da ora in poi, sia il produttore sia il consumatore eseguiranno esattamente n cicli, dopodiché
termineranno.
All’interno di questi n cicli, deve realizzarsi il seguente scambio informativo:
• Il produttore riempie casualmente il vettore v dello struct condiviso
• Il consumatore legge il vettore, calcola la media e la stampa a video
Come è stato detto, i processi produttore e consumatore terminano esattamente dopo avere
eseguito n cicli.
Si usi la libreria System V per implementare sia la shared memory sia i semafori per la gestione dei
processi produttore consumatore.
Si faccia in modo che il CONSUMATORE sia il responsabile della creazione della memoria condivisa e
dei due semafori, e delle relative deallocazioni.
Si scrivano i due programmi al computer e si consegnino i due seguenti file in c, con i seguenti nomi:
Produttore_COGNOME_NOME.c
Consumatore_COGNOME_NOME.c
*/
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/sem.h>
#define N 100
typedef struct {
	unsigned int n;
	float v[N];
} calcolo;
//semaforo_elementi_vuoti=n=empty (key1)
//emaforo_elementi_pieni=0=full (key2)
int up(int semid) { 
	int x;
	struct sembuf s;
	s.sem_num=0;
	s.sem_op=1;
	s.sem_flg=SEM_UNDO;
	x=semop(semid, &s, 1);
	if(x==-1) {
		return -1;
	} else {
		return 0;
	}
}
int down(int semid) { 
	int x;
	struct sembuf s;
	s.sem_num=0;
	s.sem_op=-1;
	s.sem_flg=SEM_UNDO;
	x=semop(semid, &s, 1);
	if(x==-1) {
		return -1;
	} else {
		return 0;
	}
}
int main() {
	int x, shmid, empty, full;
	int somma=0, num;
	float media;
	calcolo *c;
	void *punt;
	key_t key, key1, key2;
	//chiave shm
	key=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/produttore_consumatore", 0);
	//chiave empty
	key1=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/produttore_consumatore", 1);
	//chiave full
	key2=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/produttore_consumatore", 2);
	//creazione empty
	empty=semget(key1, 1, IPC_CREAT | 0666);
	if(empty==-1) {
		printf("Errore\n");
	}
	//creazione full
	full=semget(key2, 1, IPC_CREAT | 0666);
	if(full==-1) {
		printf("Errore\n");
	}
	//inizializzazione full
	x=semctl(full, 0, SETVAL, 0);
	if(x==-1) {
		printf("Errore\n");
	}
	//shmget
	shmid=shmget(key, (int)sizeof(calcolo), IPC_CREAT | 0666);
	if(shmid==-1) {
		printf("Errore creazione memoria\n");
	}
	//attach
	punt=shmat(shmid, 0, 0);
	if(punt==(void *)-1) {
		printf("Errore attach\n");
	}
	c=(calcolo*)punt;
	x=down(full);
	num=c->n;
	//inizializzazione sem1 
	x=semctl(empty, 0, SETVAL, num);
	if(x==-1) {
		printf("Errore\n");
	}
	for(int i=0;i<num;i++) {
		x=down(full);
		if(x==-1) {
			printf("Errore\n");
		}
		somma+=(c->v[i]);
		x=up(empty);
	}
	media=somma/num;
	printf("La media è: %.2f\n", media);
	//distruzione empty
	x=semctl(empty, 0, IPC_RMID);
	if(x==-1) {
		printf("Errore\n");
	}
	//distruzione full
	x=semctl(full, 0, IPC_RMID);
	if(x==-1) {
		printf("Errore\n");
	}
	//detach
	x=shmdt(punt);
	if(x==-1) {
		printf("Errore\n");
	}
	//distruzione shm
	x=shmctl(shmid, IPC_RMID, 0);
	if(x==-1) {
		printf("Errore\n");
	}
	exit(0);
}
