/*
Si vogliano realizzare due processi NON IMPARENTATI (ossia INDIPENDENTI) che
producono e consumano informazioni condivise tramite il meccanismo della Memoria Condivisa.
In particolare i due processi condividono un’area di memoria che contiene uno struct costituito
da: un vettore di 10 float e una stringa di 10 caratteri. Inoltre entrambi i due processi utilizzano
uno stesso file di nome “output”.
I due processi sono sincronizzati, nel senso che le azioni che compiono seguono un ordine ben
preciso: prima opera il primo processo e quando finisce opera il secondo processo. I due processi
lavorano secondo un ciclo infinito che può essere interrotto solo dal primo processo. In particolare
il primo processo, chiede all’utente se vuole continuare; nel caso in cui l’utente non voglia
continuare deve inserire la parola “fine”, che verrà scritta nel campo stringa dello struct condiviso
tra i due processi. L’inserimento di questa stringa comporta la fine del primo processo e, visto
che lo struct è condiviso tra i due processi, anche il secondo processo termina quando legge il
valore “fine” all’interno dello struct condiviso.
Il primo processo inizia per prima; la prima cosa che fa è quello di aprire il file “output” e se esso
contiene elementi (alla prima esecuzione non conterrà nulla ovviamente), devono essere tutti letti
(a partire dall’inizio del file fino alla fine del file) e stampati a video. Poi il primo processo riempie
in modo casuale il vettore di 10 elementi contenuto nello struct condiviso, e passa il turno al
secondo processo.
Il secondo processo legge il campo stringa dello struct condiviso e se contiene il valore “fine”,
termina. Altrimenti, il secondo processo legge tutto il vettore dello struct condiviso e scrive tutti
gli elementi nel file di nome “output”. Si suppone che il secondo processo apra il file con
l’opzione O_TRUNC in modo da svuotarne il contenuto e riempirlo ex novo. Il file viene così
riempito di tutti i 10 elementi contenuti nel vettore condiviso.
A questo punto il turno ripassa al primo processo che ripete quanto detto prima per poi passare il
turno al secondo processo, come detto, e così via fino a quando i due processi terminano perché
l’utente del primo processo ha inserito la parola “fine”, come già detto.
Per lo svolgimento dell’esercizio si utilizzino la memoria condivisa, i semafori SystemV e i
file.
*/
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
union semun {                   /* Used in calls to semctl() */
	int                 val;
	struct semid_ds *   buf;
	unsigned short *    array;
	#if defined(__linux__)
		struct seminfo *    __buf;
	#endif
};
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
struct memoria {
	float v[10];
	char s[10];
};
int main() {
	int shmid, semid;
	int x;
	int f;
	char buff[10];
	struct memoria *m; 
	key_t sem, shm;
	sem=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/shm1", 0);
	shm=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/shm1", 1);
	semid=semget(sem, 0, 0);
	if(semid==-1) {
		printf("Errore\n");
		exit(1);
	} else {
		printf("Semaforo acquisito\n");
	}
	shmid=shmget(shm, 0, 0);
	if(shmid==-1) {
		printf("Errore\n");
		exit(1);
	} else {
		printf("Memoria acquisita\n");
	}
	m=(struct memoria*)shmat(shmid, 0, 0);
	if(m==(void*)-1) {
		printf("Errore\n");
		exit(1);
	} else {
		printf("Attach effettuata\n");
	}
	while(1) {
		x=down(semid);
		if(x==-1) {
			printf("Errore down\n");
		}
		if(strncmp(m->s, "fine", 4)==0) {
			x=shmdt(m);
			if(x==-1) {
				printf("Errore detach\n");
			} else {
				printf("Detach effettuato\n");
			}
			exit(0);
		}
		f=open("output.txt", O_WRONLY | O_TRUNC);
		for(int i=0;i<10;i++) {
			sprintf(buff, " %f", m->v[i]);
			write(f, (void*)buff, sizeof(float));
		}
		close(f);
		x=up(semid);
		if(x==-1) {
			printf("Errore up\n");
		}
	}
}
