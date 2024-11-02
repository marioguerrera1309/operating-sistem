/*
Esercizio: simulare il gioco “battaglia navale” tra
due processi che emettono coordinate casuali in
una matrice n x n dove n è un intero predefinito.
Si utilizzano i meccanismi IPC che più si
ritengono opportuni per la sincronizzazione delle
giocate, il rilevamento della fine di una partita, e
per lo scambio delle coordinate delle celle
“colpite”.
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/wait.h>
#define n 10
struct memoria {
	int matrice[n][n];
	int vittoria;
};
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
void visualizza(struct memoria *mem) {
	printf("%d | %d | %d | %d | %d | %d | %d | %d | %d | %d\n", mem->matrice[0][0], mem->matrice[0][1], mem->matrice[0][2], mem->matrice[0][3], mem->matrice[0][4], mem->matrice[0][5], mem->matrice[0][6], mem->matrice[0][7], mem->matrice[0][8], mem->matrice[0][9]);
	printf("\n\n");
}
int main() {
	int semid1, semid2, mutex, shmid, x, y, esc=0;
	pid_t p1, p2;
	semid1=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	semid2=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	mutex=semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	shmid=shmget(IPC_PRIVATE, sizeof(struct memoria), IPC_CREAT | 0666);
	struct memoria *mem;
	x=semctl(semid1, 0, SETVAL, 0);
	if(x==-1) {
		printf("Errore\n");
	}
	x=semctl(semid2, 0, SETVAL, 0);
	if(x==-1) {
		printf("Errore\n");
	}
	x=semctl(mutex, 0, SETVAL, 1);
	if(x==-1) {
		printf("Errore\n");
	}
	mem=(struct memoria*)shmat(shmid, 0, 0666);
	x=down(mutex);
	if(x==-1) {
		printf("Errore\n");
	}
	for(int i=0;i<n;i++) {
		for(int j=0;j<n;j++) {
			mem->matrice[i][j]=0;
		}
	}
	mem->vittoria=0;
	x=up(mutex);
	if(x==-1) {
		printf("Errore\n");
	}
	//inizia p1
	x=up(semid1);
	if(x==-1) {
		printf("Errore\n");
	}
	p1=fork();
	switch(p1) {
		case -1:
			printf("Errore\n");
			break;
		case 0:
			srand(getpid());
			x=down(mutex);	
			while(mem->vittoria==0) {
				x=up(mutex);
				x=down(semid1);
				if(x==-1) {
					printf("Errore\n");
				}
				x=(rand()%n)+1;
				y=(rand()%n)+1;
				x=down(mutex);
				if(x==-1) {
					printf("Errore\n");
				}
				esc=0;
				while(esc==0) {
					if(mem->matrice[x][y]==1) {
						x=(rand()%n)+1;
						y=(rand()%n)+1;		
					} else if(mem->matrice[x][y]==0) {
						mem->matrice[x][y]=1;
						visualizza(mem);
						esc=1;
					} else {
						printf("Giocatore 1 ha vinto\n");
						mem->vittoria=1;
						visualizza(mem);
						x=up(semid2);
						esc=1;
					}
				}
				x=up(mutex);
				if(x==-1) {
					printf("Errore\n");
				}
				x=up(semid2);
				if(x==-1) {
					printf("Errore\n");
				}
				x=down(mutex);
			}
			x=up(mutex);
			exit(0);
		default: 	
			break;
	}
	p2=fork();
	switch(p2) {
		case -1:
			printf("Errore\n");
			break;
		case 0:
			srand(getpid());
			x=down(mutex);	
			while(mem->vittoria==0) {
				x=up(mutex);
				x=down(semid2);
				if(x==-1) {
					printf("Errore\n");
				}
				x=(rand()%n)+1;
				y=(rand()%n)+1;
				x=down(mutex);
				if(x==-1) {
					printf("Errore\n");
				}
				esc=0;
				while(esc==0) {
					if(mem->matrice[x][y]==2) {
						x=(rand()%n)+1;
						y=(rand()%n)+1;		
					} else if(mem->matrice[x][y]==0) {
						mem->matrice[x][y]=2;
						visualizza(mem);
						esc=1;
					} else {
						printf("Giocatore 2 ha vinto\n");
						visualizza(mem);
						x=up(semid1);
						mem->vittoria=2;
						esc=1;
					}
				}
				x=up(mutex);
				if(x==-1) {
					printf("Errore\n");
				}
				x=up(semid1);
				if(x==-1) {
					printf("Errore\n");
				}
				x=down(mutex);
			}
			x=up(mutex);
			exit(0);
		default:
			break;
	}
	wait(0);
	wait(0);
	x=semctl(semid1, 1, IPC_RMID);
	x=semctl(semid2, 1, IPC_RMID);
	x=semctl(mutex, 1, IPC_RMID);
	x=shmdt(mem);
	x=shmctl(shmid, IPC_RMID, 0);
	exit(0);
}
