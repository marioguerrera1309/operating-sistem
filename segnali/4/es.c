/*
Scrivere un programma in C su Linux che emuli una struttura di 2 processi organizzati in modo master-slave.

Un processo padre (master) P0 genera 1 processo figlio (slave) P1

Il master
	- va in sleep per 1 secondo
	- autorizza lo slave alla trasmissione mediante invio di un segnale SIGUSR1
	- attende la ricezione del segnale SIGUSR2 dallo slave
	- quando il master riceve il segnale SIGUSR2 effettua una stampa a video 
	- "uccide" lo slave inviando SIGKILL
	 
Lo slave
	- attende l'autorizzazione alla trasmissione del master (ricezione del segnale SIGUSR1)
	- quando lo slave riceve il segnale SIGUSR1 effettua una stampa a video 
	- genera un numero causale r compreso tra 1 e 10
	- va in sleep per r secondi
	- invia un segnale di tipo SIGUSR2 al master
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>
void funzione(int sig) {
	if(sig==SIGUSR1) {
		printf("SIGUSR1 ricevuto dal padre\n");
	}
	if(sig==SIGUSR2) {
		printf("SIGUSR2 ricevuto dal figlio\n");
	}
}
int main() {
	struct sigaction s;
	int x;
	s.sa_handler=funzione;
	sigemptyset(&s.sa_mask);
	sigaddset(&s.sa_mask, SIGUSR1);
	sigaddset(&s.sa_mask, SIGUSR2);
	s.sa_flags=0;
	sigaction(SIGUSR1, &s, 0);
	sigaction(SIGUSR2, &s, 0);
	pid_t figlio;
	figlio=fork();
	switch(figlio) {
		case 0:
			pause();
			srand(getpid());
			x=(rand()%10)+1;
			sleep(x);
			kill(getppid(), SIGUSR2);
			break;
	}
	sleep(1);
	kill(figlio, SIGUSR1);
	pause();
	kill(figlio, SIGKILL);
}
