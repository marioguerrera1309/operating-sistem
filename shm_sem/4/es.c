/*
	Scrivere un programma in C su Linux in cui il main thread genera
	2 thread(giocatore_1 e giocatore_2) per iniziare un gioco.
	I due thread condividono una variabile g che viene inizializzata dal main thread
	con un valore random compreso tra 100 e 150.
	Il gioco si svolge come segue. 
	Ad ogni turno, il giocatore (thread) invitato a giocare:
		-genera un numero casuale r tra 0 e l'attuale valore di g;
		-sottrae r a g;
	Il main thread è l'arbitro del gioco e :
		-sceglie in modo casuale(generando un numero tra 1 e 2) il thread che inizia il gioco;
		- da quel momento in poi l'arbitro invita i giocatori in modo alternato (ad es. 1-2-1-2...);
		-ad ogni "giocata" controlla se vi è stato un vincitore. Ovvero valuta il valore di g,
			se questo è 0, dichiara il vincitore(vince il giocatore che per primo azzera g).
	UTILIZZARE I MUTEX PER REALIZZARE IL GIOCO.
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/types.h>
struct memoria {
	int g;
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
int set(int semid, int val) {
	int x;
	x=semctl(semid, 1, SETVAL, val);
	return x;
}
void funzione(int semid, int smain, int t, struct memoria *mem) {
	int r;
	int x;
	srand(getpid());
	while(1) {
		x=down(semid);
		r=(rand()%(mem->g))+1;
		mem->g=(mem->g)-r;
		printf("Giocatore %d: %d\n", t, mem->g);
		x=up(smain);	
	}
}
int main() {
	int semid1, semid2, semid3;
	int shmid;
	int x;
	int r, turno;
	pid_t g1, g2;
	srand(getpid());
	struct memoria *mem;
	shmid=shmget(IPC_PRIVATE, sizeof(struct memoria), IPC_CREAT | 0666);
	mem=(struct memoria*)shmat(shmid, 0, 0666);
	r=(rand()%150)+100;	
	mem->g=r;
	printf("G: %d\n", mem->g);
	semid1=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	semid2=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	semid3=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	x=set(semid1, 0);
	x=set(semid2, 0);
	x=set(semid3, 0);
	g1=fork();
	switch(g1) {
		case 0:
			funzione(semid1, semid3, 1, mem);
		default: 
			break;	
	}
	g2=fork();
	switch(g2) {
		case 0: 
			funzione(semid2, semid3, 2, mem);
		default:
			break;			
	}
	r=(rand()%2)+1;
	if(r==1) {
		x=up(semid1);
		turno=1;
	} else {
		x=up(semid2);
		turno=2;
	}
	while(1) {
		x=down(semid3);
		if(mem->g==0) {
			x=up(semid1);
			x=up(semid2);
			kill(g1, SIGKILL);
			kill(g2, SIGKILL);
			printf("Ha vinto Giocatore %d\n", turno);
			break;
		} else {
			if(turno==1) {
				x=up(semid2);
				turno=2;
			} else {
				x=up(semid1);
				turno=1;
			}
		}
	}
	shmdt(mem);
	x=shmctl(shmid, IPC_RMID, 0);
	x=semctl(semid1, 1, IPC_RMID);
	x=semctl(semid2, 1, IPC_RMID);
	x=semctl(semid3, 1, IPC_RMID);
	exit(0);
}
