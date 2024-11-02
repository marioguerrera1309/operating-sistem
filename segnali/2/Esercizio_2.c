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
#include <sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>



int main() {

    pid_t pid;
    int i, n, stat_val;
    char carattere;
    

	pid=fork();
	switch(pid){
		case -1:
			perror ("fork failed");
			exit(1);
		case 0:
			/* Processo figlio */
			alarm(15);
			printf("Inserisci una lettera: ");
			scanf("%c",&carattere);
			if(carattere>='A' && carattere<='Z'){
				alarm(0);
				exit(0);
			}
			else{
				while(1) pause();
			}
			//break;
		default:
			wait(&stat_val);
			if(WIFEXITED(stat_val)) printf("Mio figlio ha concluso la sua esecuzione normalmente.\n");
			else printf("Mio figlio NON ha concluso la sua esecuzione normalmente.\n");
           	break;
	}		
	
}
   


