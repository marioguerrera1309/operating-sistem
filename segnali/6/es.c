/*
Si realizzino due processi padre/figlio tali che il padre invia il segnale SIGUSR1 al processo figlio. Il processo
figlio cattura il segnale inviato dal padre tramite la funzione di Signal Handler che reagisce al segnale
SIGUSR1 inviando un segnale SIGUSR2 al processo padre. Appena il processo padre riceve il segnale
SIGUSR2 inviato dal figlio (che deve essere gestito anche lui da una funzione di Signal Handler scritta a
piacere), termina il figlio.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
pid_t figlio;
void funzione(int sig) {
	if(sig==SIGUSR1) {
		kill(getppid(), SIGUSR2);
	}
	if(sig==SIGUSR2) {
		kill(figlio, SIGKILL);			
	}
}
int main() {
	struct sigaction s;
	s.sa_handler=funzione;
	sigemptyset(&s.sa_mask);
	sigaddset(&s.sa_mask, SIGUSR1);
	sigaddset(&s.sa_mask, SIGUSR2);
	s.sa_flags=0;
	sigaction(SIGUSR1, &s, 0);
	sigaction(SIGUSR2, &s, 0);
	figlio=fork();
	switch(figlio) {
		case 0:
			pause();
			break;
	}
	sleep(1);
	kill(figlio, SIGUSR1);
	pause();
	printf("Fine\n");
	wait(0);
	exit(0);
}
