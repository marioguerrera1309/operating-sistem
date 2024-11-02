/*
Scrivere un programma in C su Linux in cui un processo padre P0 genera 1 processo figlio P1.

Il processo figlio:
	- imposta, mediante la funzione alarm, l'invio a se stesso del segnale SIGALRM dopo 15 secondi	
	- chiede all'utente l'inserimento da tastiera di una lettera dell'alfabeto 
	- se la lettera viene inserita in maiuscolo, il processo figlio annulla l'impostazione della alarm() e conclude la proprio esecuzione restituendo 0.
	- in caso contrario attende l'arrivo di un segnale (arriverà SIGALRM)

Il processo padre:
	- attende la terminazione del figlio
	- stampa a video un messaggio che indica se il figlio ha terminato normalmente oppure no.
*/
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
void funzione(int SIG) {
	printf("Figlio morto\n");
	exit(1);
}
int main() {
	int wstatus;
	pthread_t figlio;
	char c;
	struct sigaction s;
	sigemptyset(&s.sa_mask);
	sigaddset(&s.sa_mask, SIGALRM);
	s.sa_flags=0;
	figlio=fork();
	switch(figlio) {
		case 0:
			s.sa_handler=funzione;
			sigaction(SIGALRM, &s, NULL);
			alarm(15);
			scanf("%c", &c);
			if(c<97) {
				alarm(0);
				printf("Figlio morto\n");
				exit(0);
			} else {
				pause();
			}
			break;
		default:
			break;
	}
	wait(&wstatus);
	if(WEXITSTATUS(wstatus)==0) {
		printf("Figlio morto normalmente\n");
	} else if(WEXITSTATUS(wstatus)==1) {
		printf("Figlio ucciso da SIGALRM\n");
	} else {
		printf("Errore\n");
	}
	
}
