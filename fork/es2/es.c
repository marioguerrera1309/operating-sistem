/*
Scrivere un programma in C in cui il processo principale (P0) apre un file di testo (se non esiste, lo crea).
Successivamente crea due processi figli P1 e P2.

P1 e P2 ciclicamente, per 10 volte, con pause di 1 secondo
	- scrivono sul file di testo il proprio PID e un numero generato casualmente compreso tra 0 e 10
Dopo le dieci iterazioni terminano con successo.

Il processo padre, dopo aver creato i processi figlio, attende la terminazione dei figli, chiude il file e termina esso stesso.

Utilizzare i semafori per regolare l’accesso alle risorse condivise.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/wait.h>
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
	s.sem_op=-1;
	s.sem_flg=0;
	x=semop(semid, &s, 1);
	return x;
}
int main() {
	int f, x, y;
	int semid;
	semid=semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);
	f=open("file.txt", O_CREAT | O_RDWR, 0666);
	x=semctl(semid, 0, SETVAL, 1);
	pid_t p1, p2;
	char buf[50];
	p1=fork();
	switch(p1) {
		case 0:
			srand(getpid());
			for(int i=0;i<10;i++) {
				x=(rand()%10)+1; 
				printf("PID: %d x: %d\n", getpid(), x);
				sprintf(buf, "PID: %d x: %d\n", getpid(), x);
				y=down(semid);
				if(y==-1) {
					printf("Errore down\n");
				}
				write(f, buf, strlen(buf));
				y=up(semid);
				if(y==-1) {
					printf("Errore up\n");
				}
				sleep(1);
			}
			break;
		case -1:
			printf("Errore\n");
			break;
		default: 
			p2=fork();
			switch(p2) {
				case 0:
					srand(getpid());
					for(int i=0;i<10;i++) {
						x=(rand()%10)+1; 
						printf("PID: %d x: %d\n", getpid(), x);
						sprintf(buf, "PID: %d x: %d\n", getpid(), x);
						y=down(semid);
						if(y==-1) {
							printf("Errore down\n");
						}
						write(f, buf, strlen(buf));
						y=up(semid);
						if(y==-1) {
							printf("Errore up\n");
						}
						sleep(1);
					}
					break;
				case -1:
					printf("Errore\n");
					break;
				default:
					break;
			}
			break;
	}
	wait(0);
	wait(0);
	close(f);
	x=semctl(semid, 0, IPC_RMID);
}
