/*
Scrivere un programma in C su Linux in cui un processo padre P0 genera N processi figli P1...PN.

Il padre, dopo aver creato gli N  figli, esegue per 10 iterazioni le seguenti operazioni:
	- va in sleep per 2 secondi
	- genera un numero casuali r1 compreso tra 1 e 2 
	- genera un numero casuali r2 compreso tra 1 e N
	- invia il segnale SIGUSR<r1> al figlio P<r2>

In conclusione, il processo padre "uccide" i figli inviando SIGKILL
	 
Ogni processo figlio in un loop infinito
	- attende l'arrivo dei segnali SIGUSR1 o SIGUSR2
	- ogni volta che riceve uno dei due segnali stampa il proprio PID ed il segnale ricevuto
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#define N 20
void gestore(int sig) {
	if(sig==SIGUSR1) {
		printf("Ricevuto PID: %d - SIGUSR1\n", getpid());
	} else {
		printf("Ricevuto PID: %d - SIGUSR2\n", getpid());
	}
}
int main() {
	pid_t pid[N];
	int r1, r2;
	struct sigaction s;
	s.sa_handler=gestore;
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGUSR1);
	sigaddset(&(s.sa_mask), SIGUSR2);
	s.sa_flags=0;
	sigaction(SIGUSR1, &s, NULL);
	sigaction(SIGUSR2, &s, NULL);
	for(int i=0;i<N;i++) {
		pid[i]=fork();
		switch(pid[i]) {
			case 0:
			while(1) {
				pause();
			}
			break;
			default:
			break;
		}
	}
	srand(getpid());
	for(int i=0;i<10;i++) {
		sleep(2);
		r1=(rand()%2)+1;
		r2=(rand()%N)+1;
		if(r1==1) {
			kill(pid[r2], SIGUSR1);
		} else {
			kill(pid[r2], SIGUSR2);
		}
	}
	for(int i=0;i<N;i++) {
		kill(pid[i], SIGKILL);
	}
	exit(0);
}
