#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};


int sem_up(int semid);
int sem_down(int semid);
int sem_set(int semid, int val);

#define KEY_SEM (key_t)9876
#define KEY_SHM (key_t)9875

int main() {
    int semid, shmid, i, r;
    char *comando;
    pid_t m1, m2;
    char *cmds[] = {"Avanti", "Indietro", "Destra", "Sinistra", "Stop"};
    char oldcmd[100] = "";

    
    semid = semget(KEY_SEM, 1, 0666 | IPC_CREAT);
    sem_set(semid, 1);
    shmid = shmget(KEY_SHM, sizeof(char)*100, 0666 | IPC_CREAT);

    comando = (char *)shmat(shmid, NULL, 0);

    m1 = fork();
    switch(m1) {
        case -1: break;
        case 0:
            while(1) {
                sem_down(semid);
                if(strncmp(comando, "Termina", 7) == 0) {
                    shmdt(comando);
                    sem_up(semid);
                    exit(0);
                }
                if(strncmp(comando, oldcmd, 100) != 0) {
                    strncpy(oldcmd, comando, 100);
                    printf("Figlio 1: Ricevuto %s\n", comando);
                }
                sem_up(semid);
                sleep(1);
            }
        default:
            break;
    }

    m2 = fork();
    switch(m2) {
        case -1: break;
        case 0:
            while(1) {
                sem_down(semid);
                if(strncmp(comando, "Termina", 7) == 0) {
                    shmdt(comando);
                    sem_up(semid);
                    exit(0);
                }
                if(strncmp(comando, oldcmd, 100) != 0) {
                    strncpy(oldcmd, comando, 100);
                    printf("Figlio 2: Ricevuto %s\n", comando);
                }
                sem_up(semid);
                sleep(1);
            }
        default:
            break;
    }

    srand(getpid());
    for(i=0; i< 20; i++) {
        r = rand() % 5;
        sem_down(semid);
        strcpy(comando, cmds[r]);
        printf("Padre: %s\n", comando);
        sem_up(semid);
        sleep(2);
    }

    sem_down(semid);
    strcpy(comando, "Termina");
    sem_up(semid);

    wait(NULL); wait(NULL);

    shmdt(comando);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, 0);
}

int sem_up(int semid) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;
    if (semop(semid, &sb, 1) == -1) return 0; //se semop() fallisce, la funzione sem_up() restituisce 0 al chiamante
	return 1;
}

int sem_down(int semid) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    if (semop(semid, &sb, 1) == -1) return 0; //se semop() fallisce, la funzione sem_down() restituisce 0 al chiamante
 	return 1;
}

int sem_set(int semid, int val) {
    union semun un;
    un.val = val;
    return semctl(semid, 0, SETVAL, un);
}
