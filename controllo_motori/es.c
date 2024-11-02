#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/wait.h>
#ifdef _SEM_SEMUN_UNDEFINED
union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };
#endif
int set(int semid, int val) {
	int x;
	union semun s;
	s.val=val;
	x=semctl(semid, 0, SETVAL, s);
	if(x==-1) {
		return -1;
	} else {
		return 0;
	}
}
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
	key_t keys, keym;
	pid_t figlio;
	void *punt;
	char *memoria;
	char oldmemoria[11];
	char comandi[5][11] = {"Avanti", "Indietro", "Destra", "Sinistra", "Stop"};
	int x, y, semid, shmid;
	keys=ftok("/home/mario/Scrivania/sistemioperativi/controllo_motori", 0);
	keym=ftok("/home/mario/Scrivania/sistemioperativi/controllo_motori", 1);
	semid=semget(keys, 1, IPC_CREAT | 0666);
	if(semid==-1) {
		printf("Errore creazione semaforo\n");
	}
	x=set(semid, 1);
	if(x==-1) {
		printf("Errore set semaforo\n");
	}
	shmid=shmget(keym, 20*sizeof(char), IPC_CREAT | 0666);
	if(shmid==-1) {
		printf("Errore creazione memoria\n");
	}
	punt=shmat(shmid, 0, 0);
	if(punt==(void*)-1) {
		printf("Errore attach\n");
	}
	memoria=(char*)punt;
	for(int i=0;i<2;i++) {
		figlio=fork();
		switch(figlio) {
			case -1: 
				printf("Errore fork\n");
			break;
			case 0:
				while(1) {
					x=down(semid);
					if(x==-1) {
						printf("Errore down\n");
					}
					if(strcmp(memoria, "Stop")==0) {
						x=shmdt(punt);
						if(x==-1) {
							printf("Errore detach\n");
						}
						x=up(semid);
						if(x==-1) {
							printf("Errore up\n");
						}						
						exit(0);						
					} else if(strcmp(oldmemoria, memoria)!=0) {
						printf("%s\n", memoria);
						strcpy(oldmemoria, memoria);
					}
					x=up(semid);
					if(x==-1) {
						printf("Errore up\n");
					}
					sleep(1);
				}
			break;
			default:
			break; 
		}
	}
	srand(getpid());
	for(int i=0;i<20;i++) {
		y=rand()%5;
		x=down(semid);
		if(x==-1) {
			printf("Errore down\n");
		}
		strcpy(memoria, comandi[y]);
		x=up(semid);
		if(x==-1) {
			printf("Errore up\n");
		}
		sleep(2);
	}
	x=down(semid);
	if(x==-1) {
		printf("Errore down\n");
	}
	strcpy(memoria, "Stop");
	x=up(semid);
	if(x==-1) {
		printf("Errore up\n");
	}
	wait(NULL);
	wait(NULL);
	x=shmdt(punt);
	if(x==-1) {
		printf("Errore detach\n");
	}
	x=shmctl(shmid, IPC_RMID, 0);
	if(x==-1) {
		printf("Errore rimozione memoria\n");
	}
	x=semctl(semid, IPC_RMID, 0);
	if(x==-1) {
		printf("Errore rimozione semaforo\n");
	}
	exit(0);
}
