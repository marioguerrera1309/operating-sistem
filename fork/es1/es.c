/**
 * Scrivere un programma in C in cui un
 * processo padre P0 crea un processo figlio P1
 * 
 * P0, dopo aver creato P1,
 * -  ne attende la terminazione,
 * -  stampa a video lo stato di uscita di P1,
 * -  termina
 * 
 * P1,
 * -  stampa a video il proprio PID
 * -  stampa il PID del processo padre
 * -  genera un numero casuale n tra 1 e 10
 * -  termina con stato di terminazione n
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
int main() {
	pid_t figlio;
	int n;
	int stato;
	srand(getpid());
	figlio=fork();
	switch(figlio) {
		case 0:
			printf("Pid figlio: %d\n", getpid());
			printf("Pid padre: %d\n", getppid());
			n=(rand()%10)+1;
			printf("Uscita figlio: %d\n", n);
			exit(n);
			break;
		default:
			break;
	}
	wait(&stato);
	printf("Codice uscita: %d\n", WEXITSTATUS(stato));
}
