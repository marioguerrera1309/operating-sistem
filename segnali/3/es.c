/*
Scrivere un programma in C su Linux in cui un processo padre P0 genera 1 processo figlio P1.
Successivamente, il padre:
	- va in sleep per 2 secondi
	- ferma l'esecuzione del figlio usando il segnale SIGTSTP
	- va in sleep per 5 secondi
	- invio al figlio il segnale SIGCONT per farlo ripartire
	- attende la terminazione del figlio e ne stampa lo stato di terminazione
	- esce


Il figlio esegue per 5 iterazioni le seguenti operazioni:	
	- va in sleep per 1 secondo
	- stampa "Ciao mondo\n"
Successivamente il figlio genera un numero casuale n compreso tra 1 e 5 ed esce con stato di terminazione n.
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
int main() {
	pid_t figlio;
	figlio=fork();
	int x;
	switch(figlio) {
		case 0:
			for(int i=0;i<5;i++) {
				printf("Ciao\n");
				sleep(1);
			}
			srand(getpid());
			x=(rand()%5)+1;
			exit(x);	
			break;
	}
	sleep(2);
	kill(figlio, SIGTSTP);
	sleep(5);
	kill(figlio, SIGCONT);
	wait(&x);
	printf("Stato di terminazione processo figlio %d: %d\n", figlio, WEXITSTATUS(x));
}
