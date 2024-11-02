/**
* Scrivere un programma in C in cui un processo padre P0
* crea N = 10 processi figli P1..PN
* 
* P0,
* -  crea gli N processi figli stampando il PID di ogni figlio 
* -  attende la terminazione dell'ultimo figlio
* -  termina
* 
* Ogni processo figlio,
* -  stampa a video il proprio PID
* -  termina con stato di terminazione EXIT_SUCCESS
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 10
int main() {
	pid_t p[N];
	for(int i=0;i<N;i++) {
		p[i]=fork();
		switch(p[i]) {
			case -1: 
				printf("Errore\n");
				break;
			case 0:
				printf("Il mio pid è: %d\n", getpid());
				exit(EXIT_SUCCESS);
			default: 
				printf("p[%d]=%d\n", i, p[i]);
				break;	
		}
	}
	waitpid(p[N-1], NULL, 0);
	exit(EXIT_SUCCESS);
}
