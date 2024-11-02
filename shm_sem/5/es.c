/**
 * Implementare un programma in C che permetta la gestione di un
 * servizio ansa tra processi
 * 
 * Un processo dispatcher che chiede all'utente di inserire una
 * notizia da tastiera e la invia a tutti i processi in ascolto.
 * 
 * Non si sa a priori quanti processi sono in ascolto.
 * Ogni nuovo processo che si mette in ascolto incrementa di 1 una
 * variabile intera condivisa all'interno della shared memory.
 * 
 * Ogni notizia deve essere letta da tutti i processi in ascolto una
 * sola volta e stampata a video.
 * 
 * Il processo dispatcher se legge la notizia (stringa) "termina" la
 * scrive nella shared memory, attende che tutti i processi abbiano
 * letto e dopo termina.
 * 
 * == Dati condivisi ==
 * struct dati {
 *      int id_notizia;
 *      char notizia[200];
 *      int num_processi_lettori_attivi;
 *      int num_letture_effettuate;
 * };
 * 
 * == Operazioni dispatcher ==
 * 1. Creazione semaforo/i
 * 2. Creazione e attach shared memory
 * 3. Richiesta notizia da tastiera
 * 4. Azzeramento num_letture_effettuate
 * 5. controllo num_letture_effettuate == num_processi_lettori_attivi
 *    ossia, attende che tutti lettori abbiano letto
 * 6. controllo notizia == termina, termina, altrimenti
 *    ritorno al punto 3.
 * 
 * * == Operazioni lettore ==
 * 1. Creazione semaforo/i
 * 2. Creazione e attach shared memory
 * 3. incremento num_processi_lettori_attivi
 * 4. lettura notizia
 * 5. se nuova notizia la stampa a video e incrementa
 *    num_letture_effettuate.
 * 6. se notizia == termina, termina, altrimenti va al punto 4.
 */
 #include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/wait.h>
union semun {
       int              val;    /* Value for SETVAL */
       struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
       unsigned short  *array;  /* Array for GETALL, SETALL */
       struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                   (Linux-specific) */
};
struct dati {
	int id_notizia;
	char notizia[200];
	int num_processi_lettori_attivi;
	int num_letture_effettuate;
};
int up(int semid) {
	int x;
	struct sembuf s;
	s.sem_num=0;
	s.sem_op=1;
	s.sem_flg=0;
	x=semop(semid, &s, 1);
	return x;
}
int down(int semid) {
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
void funzione(int semid, struct dati *dato, int i) {
	int x;
	int id_notizia=0;
	x=down(semid);
	(dato->num_processi_lettori_attivi)++;
	x=up(semid);
	while(1) {
		x=down(semid);
		if(strcmp((dato->notizia), "termina")==0) {
			printf("Processo %d terminato\n", i);
			(dato->num_letture_effettuate)++;
			id_notizia++;
			x=up(semid);
			exit(0);
		} else if((dato->id_notizia)!=id_notizia) {
			printf("Nuova notizia processo %d: %s\n", i, dato->notizia);
			(dato->num_letture_effettuate)++;
			id_notizia++;
			x=up(semid);
		}
	}
}
int main() {
	int semid;
	int shmid;
	int x, r;
	pid_t figlio;
	char notizia[200];
	struct dati *dato;
	shmid=shmget(IPC_PRIVATE, sizeof(struct dati), IPC_CREAT | 0666);
	dato=(struct dati*)shmat(shmid, 0, 0666);
	dato->id_notizia=0;
	strcpy((dato->notizia), "");
	dato->num_processi_lettori_attivi=0;
	dato->num_letture_effettuate=0;
	semid=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	x=set(semid, 1);
	srand(getpid());
	r=(rand()%20)+1;
	for(int i=0;i<r;i++) {
		figlio=fork();
		switch(figlio) {
			case 0:	
				funzione(semid, dato, i);
				printf("Errore\n");
				break;
			default: 
				break;
		}
	}
	x=down(semid);
	do {
		printf("Inserisci notizia: ");
		scanf("%s", notizia);
		dato->num_letture_effettuate=0;
		strcpy((dato->notizia), notizia);
		(dato->id_notizia)++;
		x=up(semid);
		x=down(semid);
		while((dato->num_processi_lettori_attivi)>(dato->num_letture_effettuate)) {			
			x=up(semid);
			x=down(semid);
		}
	} while(strcmp(notizia, "termina")!=0);	
	for(int i=0;i<(dato->num_processi_lettori_attivi);i++) {
		wait(0);
	}
	shmdt(dato);
	x=shmctl(shmid, IPC_RMID, 0);
	x=semctl(semid, 0, IPC_RMID);
	exit(0);
}
