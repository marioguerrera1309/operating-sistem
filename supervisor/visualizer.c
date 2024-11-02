/*
Scrivere un'applicazione composta da tre programmi 
1. Supervisor
Manda in esecuzione gli altri due processi (Monitor e Visualizer) come processi figli, scrive su un file (pidfile.log) il PID di visualizer.
Se un processo tra monitor e visualizer termina per qualsiasi motivo il supervisor lo riavvia.
Se riceve CTRL-C (SIGINT) invia a tutti i processi figli, Monitor e Supervisor, (SIGQUIT) e termina esso stesso.
2. Monitor 
Ciclicamente ad intervalli di 1 secondo apre un file di testo (testo.txt), se il file è cambiato lo segnala al visualizer (SIGUSR1)
(Il Pid del visualizer è nel file pidfile.log) chiude il file (testo.txt) 
Se riceve SIGQUIT Termina. Deve ignorare SIGINT 
3. Visualizer 
Ogni volta che arriva SIGUSR1 stampa a video il contenuto del file (texto.txt) Se riceve SIGQUIT Termina. Deve ignorare SIGINT.
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
#define DIM 1000
void gestione(int sig) {
	int f, len;
	char buffer[DIM];
	if(sig==SIGUSR1) {
		f=open("testo.txt", O_RDONLY);
		len=read(f, (void*)buffer, DIM);
		buffer[len]='\0';
		printf("%s", buffer);
		close(f);
	} else {
		printf("Uscita Visualizer\n");
		exit(0);
	}
}
int main() {
	struct sigaction s;
	s.sa_handler=gestione;
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGQUIT);
	sigaddset(&(s.sa_mask), SIGUSR1);
	s.sa_flags=0;
	sigaction(SIGQUIT, &s, NULL);
	signal(SIGINT, SIG_IGN);	
	sigaction(SIGUSR1, &s, NULL);
	while(1) {
		pause();
	}
}
