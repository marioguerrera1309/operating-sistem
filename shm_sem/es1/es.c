/*
Scrivere un programma in C in cui il main thread genera 6 thread secondari, di cui 3 del tipo inc_thread e 3 del tipo
mul_thread.
Tutti i thread condividono una variabile intera, inizializzata a 1.
●Ogni inc_thread, ciclicamente, incrementa la variabile condivisa di 1, se e solo se il suo valore è diverso da 0. In
caso contrario termina la propria esecuzione. Ogni thread inc_thread va in sleep per 1 secondi ad ogni ciclo
(chiamare la funzione sleep fuori dalla sezione critica).
●Ogni mul_thread, ciclicamente, moltiplica la variabile condivisa per un valore casuale compreso tra 1 e 3, se e
solo se il valore della stessa è diverso da 0. In caso contrario termina la propria esecuzione. Ogni thread
mul_thread va in sleep per 2 secondi ad ogni ciclo (chiamare la funzione sleep fuori dalla sezione critica).
●Il main thread, ciclicamente, ad intervalli di 1 secondo, stampa il valore della variabile condivisa, se e solo se è
pari. Dopo 12 iterazioni, il main thread assegna il valore zero alla variabile condivisa, attende la terminazione
dei thread secondari e termina esso stesso.
Utilizzare il meccanismo dei pthread_mutex per regolare l’accesso in sezione critica.
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/ipc.h>
struct memoria {
	int variabile;
};
int up(int semid) {
	struct sembuf s;
	int x;
	s.sem_num=0;
	s.sem_op=1;
	s.sem_flg=0;
	x=semop(semid, &s, 1);
	return x;
}
int down(int semid) {
	struct sembuf s;
	int x;
	s.sem_num=0;
	s.sem_op=-1;
	s.sem_flg=0;
	x=semop(semid, &s, 1);
	return x;
}
void function_inc(int semid, struct memoria *mem) {
	int x;
	while(1) {
		x=down(semid);
		if(x==-1) {
			printf("Errore\n");
		}
		if(mem->variabile==0) {
			x=up(semid);
			if(x==-1) {
				printf("Errore\n");
			}
			exit(0);
		} else {
			(mem->variabile)++;
		}
		x=up(semid);
		if(x==-1) {
			printf("Errore\n");
		}
		sleep(1);
	}	
}
void function_mul(int semid, struct memoria *mem) {
	int x, r;
	srand(getpid());
	while(1) {
		r=(rand()%3)+1;
		x=down(semid);
		if(x==-1) {
			printf("Errore\n");
		}
		if(mem->variabile==0) {
			x=up(semid);
			if(x==-1) {
				printf("Errore\n");
			}
			exit(0);
		} else {
			mem->variabile=(mem->variabile)*r;
		}
		x=up(semid);
		if(x==-1) {
			printf("Errore\n");
		}
		sleep(2);
	}
}
int main() {
	int x;
	pid_t inc[3], mul[3];
	struct memoria *mem;
	int shmid, semid;
	shmid=shmget(IPC_PRIVATE, sizeof(struct memoria), IPC_CREAT | 0666);
	mem=(struct memoria*)shmat(shmid, 0, 0666);
	mem->variabile=1;
	semid=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	x=semctl(semid, 0, SETVAL, 1);
	if(x==-1) {
		printf("Errore\n");
	}
	for(int i=0;i<3;i++) {
		inc[i]=fork();
		switch(inc[i]) {
			case 0:
				function_inc(semid, mem);
			default:
				break;
		}
		mul[i]=fork();
		switch(mul[i]) {
			case 0:
				function_mul(semid, mem);
			default:
				break;
		}
	}
	for(int i=0;i<12;i++) {
		x=down(semid);
		if(x==-1) {
			printf("Errore\n");
		}
		if((mem->variabile)%2==0) {
			printf("Variabile: %d\n", mem->variabile);	
		}
		x=up(semid);
		if(x==-1) {
			printf("Errore\n");
		}
		sleep(1);
	}
	x=down(semid);
	if(x==-1) {
		printf("Errore\n");
	}
	mem->variabile=0;
	x=up(semid);
	if(x==-1) {
		printf("Errore\n");
	}
	for(int i=0;i<6;i++) {
		wait(0);
	}
	exit(0);
}
