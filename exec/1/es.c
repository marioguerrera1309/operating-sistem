/**
*Scrivere un programma in C in cui un processo padre genere 6 processi figli.
Il processo padre, dopo aver generato tutti i processi figli,
* - attende la terminazione dei soli processi figli con PID dispari
* - stampa a video i PID dei processi attesi che hanno terminato.
*Ogni processo figlio
*	- esegue il comando ps -a utilizzando una delle funzioni della famiglia exec(), se il proprio PID è pari
* 	- esegue il comando date utilizzando una delle funzioni della famiglia exec(), se il proprio PID è dispari.
*	- termina.
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main() {
	pid_t figlio[6];
	for(int i=0;i<6;i++) {
		figlio[i]=fork();
		switch(figlio[i]) {
			case 0:
				if(getpid()%2==0) 
					execlp("ps", "ps", "-a", NULL);
				else 
					execlp("date", "date", NULL);
				break;
		}
	}
	for(int i=0;i<6;i++) {
		if(figlio[i]%3==0) { 
			waitpid(figlio[i], NULL, 0);
			printf("Figlio[%d]:%d terminato\n", i, figlio[i]);
		}
	}
}
