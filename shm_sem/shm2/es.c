/*
Un sistema di monitoraggio rileva periodicamente temperatura e pressione da due sensori (un sensore di temperatura ed uno di pressione). 
Tale coppia di sensori è caratterizzata da un ID e dal valore di temperatura/pressione, ossia:

struct sh_mem {
    int id;
    int temperatura;
    int pressione;
};

Il sistema è composto da 2 processi (P0 e P1). 
P0 è il processo di controllo.
P1 è il processo che campiona i dati dai sensori.
P0, dopo aver creato e inizializzato gli attributi dei sensori (impostando l'ID a 1, il valore di temperatura a 50 e il valore della pressione a 800)
	- crea il processo figlio P1

P0, ciclicamente e per 10 volte, con pause di 1 secondo, elabora il valore dei sensori ed in particolare svolge le seguenti operazioni 
	- visualizza i valori di temperatura e pressione
	- scala del 2% il valore della temperatura, se supera la soglia di 58
	- aumenta del 2% il valore della pressione, se inferiore a 760
Al termine dei 10 cicli, P0 
	- imposta il valore della temperatura a -100
	- imposta il valore della pressione a 0
	- attende la terminazione di P1
	- elimina shared memory e semafori
	- termina

Il processo P1 ciclicamente, con pause di 2 secondi
	- aggiorna il valore di temperatura, incrementandolo di una certa percentuale random tra il 2% e il 4%
	- aggiorna il valore di pressione, decrementandolo di una certa percentuale random tra l'1% e il 3%
	- se il valore di temperatura è uguale a -100 e il valore di pressione è uguale a 0, termina

Utilizzare il meccanismo la shared memory per la comunicazione tra processi e i semafori per regolare l’accesso alle risorse condivise.
*/
#include<stdio.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
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
struct sh_mem {
	int id;
	int temperatura;
	int pressione;
};
int main() {
	struct sh_mem *mem;
	int x;
	pid_t figlio;
	int keym, keys;
	int shmid, semid;
	keym=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/shm2", 0);
	keys=ftok("/home/mario/Scrivania/sistemioperativi/shm_sem/shm2", 1);
	semid=semget(keys, 1, IPC_CREAT | 0666);
	x=semctl(semid, 0, SETVAL, 1);
	shmid=shmget(keym, sizeof(struct sh_mem), IPC_CREAT | 0666);
	mem=(struct sh_mem*)shmat(shmid, 0, 0666);
	x=down(semid);
	if(x==-1) {
		printf("Errore down\n");
	}
	mem->id=1;
	mem->temperatura=50;
	mem->pressione=800;
	//printf("%d\n%d\n%d\n", mem->id, mem->temperatura, mem->pressione);
	x=up(semid);
	if(x==-1) {
		printf("Errore up\n");
	}
	figlio=fork();
	switch(figlio) {
		case -1:
			printf("Errore fork\n");
			break;
		case 0:
			srand(getpid());
			while(1) {
				x=down(semid);
				if(x==-1) {
					printf("Errore down\n");
				}
				if(mem->temperatura==-100 && mem->pressione==0) {
					shmdt(mem);
					exit(0);
				}
				x=(rand()%4)+2;
				mem->temperatura=mem->temperatura+(((mem->temperatura)*x)/100);
				x=(rand()%3)+1;
				mem->pressione=mem->pressione-(((mem->pressione)*x)/100);
				x=up(semid);
				if(x==-1) {
					printf("Errore up\n");
				}
				sleep(2);								
			}
			printf("Errore\n");
			break;
	}
	for(int i=0;i<10;i++) {
		x=down(semid);
		if(x==-1) {
			printf("Errore down\n");
		}
		printf("Temperatura:%d\nPressione:%d\n", mem->temperatura, mem->pressione);
		if(mem->temperatura>58) {
			mem->temperatura=mem->temperatura-(((mem->temperatura)*2)/100);
		}
		if(mem->pressione<760) {
			mem->pressione=mem->pressione+(((mem->pressione)*2)/100);
		}
		x=up(semid);
		if(x==-1) {
			printf("Errore up\n");
		}
		sleep(1);
	}
	x=down(semid);
	if(x==-1) {
		printf("Errore down\n");
	}
	mem->temperatura=-100;
	mem->pressione=0;
	x=up(semid);
	if(x==-1) {
		printf("Errore up\n");
	}
	wait(0);
	shmdt(mem);
	semctl(semid, 0, IPC_RMID);
	shmctl(shmid, IPC_RMID, 0);
}
