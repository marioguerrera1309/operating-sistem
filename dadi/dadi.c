/*
Esercizio 2: Lancio dei dadi
Scrivere un programma in C su Linux che simuli il lancio contemporaneo di N dadi.
• Il programma prevede un processo padre (P0) che:
• Genera N processi figli (P1-PN)
• Invia un segnale SIGUSR1 ad ogni processo figlio
• Attende la terminazione di ogni processo figlio e ne legge lo stato di terminazione di ciascuno (quest'ultimo conterrà
il valore di ciascun dado lanciato).
• Stampa a video la somma di tutti i dadi e termina.
• Ciascun processo figlio:
• Attende l'arrivo di SIGUSR1 dal processo padre.
• Quando arriva il segnale calcola un numero intero random (chiamato num) tra 1 e 6
• Termina con stato di terminazione "num".
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<signal.h>
#define N 5
void gestore() {
	srand((unsigned int)getpid());
	int num;
	num=(rand() % 6)+1;
	printf("Figlio: %d -> %d\n", getpid(), num);
	exit(num);
}
int main() {
	pid_t figli[N];
	int x, somma=0;
	struct sigaction s;
	s.sa_handler=gestore;
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGUSR1);
	s.sa_flags=0;
	sigaction(SIGUSR1, &s, NULL);
	for(int i=0;i<N;i++) {
		figli[i]=fork();
		switch(figli[i]) {
			case 0:
				pause();
			default:
				break;
		}
		kill(figli[i], SIGUSR1);
		wait(&x);
		somma=somma+WEXITSTATUS(x);
	}
	printf("Somma: %d\n", somma);
}
