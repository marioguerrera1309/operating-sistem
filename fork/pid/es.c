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
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
int main() {
	pid_t figlio;
	int x, y;
	figlio=fork();
	switch(figlio) {
		case -1:
			printf("Errore\n");
			exit(1);
		case 0:
			srand(getpid());
			y=(rand()%10)+1;
			printf("Y: %d\n", y);
			printf("Il mio PID è: %d\n", getpid());
			printf("Il PID del processo padre è: %d\n", getppid());
			exit(y);
		default:
			wait(&x);
			printf("Lo stato di uscita di %d è: %d\n", figlio, WEXITSTATUS(x));
			break;
	}
	exit(0);
}
