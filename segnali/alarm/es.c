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

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
void gestore(int sig) {
	exit(1);
}
int main() {
	pid_t figlio;
	int x;
	figlio=fork();
	char c;
	struct sigaction s;
	switch(figlio) {
		case -1:
			printf("Errore\n");
		case 0:
			s.sa_handler=gestore;
			sigemptyset(&(s.sa_mask));
			sigaddset(&(s.sa_mask), SIGALRM);
			s.sa_flags=0;
			sigaction(SIGALRM, &s, 0);
			printf("Inserisci carattere: ");
			alarm(15);
			scanf("%c", &c);
			//65=A 90=Z
			if(c>=65 && c<=90) {
				alarm(0);
				exit(0);
			} else {
				pause();
			}
		break;
		default:
		break;
	}
	wait(&x);
	if(WIFEXITED(x)) {
		if(WEXITSTATUS(x)==0) {
			printf("Figlio terminato con successo\n");
		} else {
			printf("Fallito\n");
		}
	}
}
