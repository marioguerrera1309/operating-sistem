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
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<time.h>
#define N 100
typedef struct {
	unsigned int n;
	float v;
} calcolo;
//semaforo_elementi_vuoti=n=empty (key1)
//semaforo_elementi_pieni=0=full (key2)
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
	int num, dim;
	srand(time(NULL));
	calcolo *c;
	void* punt;
	key_t key, key1, key2;
	//chiave shm
	key=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/produttore_consumatore_senzarray", 0);
	//chiave empty
	key1=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/produttore_consumatore_senzarray", 1);
	//chiave full
	key2=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/produttore_consumatore_senzarray", 2);
	//semget1
	empty=semget(key1, 1, 0);
	if(empty==-1)
		printf("Errore\n");
	//semget2
	full=semget(key2, 1, 0);
	if(full==-1)
		printf("Errore\n");
	//shmget
	shmid=shmget(key, 0, 0);
	if(shmid==-1)
		printf("Errore\n");
	//attach
	punt=shmat(shmid, 0, 0);
	if(punt==(void*)-1)
		printf("Errore\n");
	c=(calcolo*)punt;
	printf("Inserisci N: ");
	scanf("%d", &dim);
	x=up(empty);
	c->n=dim;
	for(int i=0;i<dim;i++) {
		x=down(empty);
		if(x==-1) {
			printf("Errore\n");
		}
		printf("Inserisci numero: ");
		scanf("%d", &num);
		(c->v)=num;
		x=up(full);
		if(x==-1) {
			printf("Errore\n");
		}
	}
	//detach 
	x=shmdt(punt);
	if(x==-1)
		printf("Errore\n");
	exit(0);
}
