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
int pv, pm;
void visualizer() {
	int f=open("pidfile.log", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	pid_t visualizer;
	char buffer[DIM];
	visualizer=fork();
	pv=visualizer;
	switch(visualizer) {
		case -1:
			printf("Errore\n");
			exit(0);
		case 0:
			printf("Visualizer:%d\n", getpid());
			sprintf(buffer, "%d\n", getpid());
			write(f, (void*)buffer, strlen(buffer));
			sleep(1);
			execl("/home/mario/Scrivania/sistemioperativi/supervisor/visualizer", "visualizer", NULL);
			printf("Errore exec\n");
			exit(0);
		default: 
			break;
	}
	close(f);
}
void monitor() {
	pid_t monitor;
	monitor=fork();
	pm=monitor;
	switch(monitor) {
		case -1:
			printf("Errore\n");
			exit(0);
		case 0:
			printf("Monitor:%d\n", getpid());
			sleep(1);
			execl("/home/mario/Scrivania/sistemioperativi/supervisor/monitor", "monitor", NULL);
			printf("Errore exec\n");
			exit(0);
		default: 
			break;
	}
	sleep(1);
}
void gestione(int sig) {
	kill(pv, SIGQUIT);
	kill(pm, SIGQUIT);
	sleep(1);
	wait(NULL);
	wait(NULL);
	printf("Uscita Supervisor\n");
	exit(0);
}
int main() {
	int x, y;
	printf("Padre:%d\n", getpid());
	struct sigaction s;
	s.sa_handler=gestione;
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGINT);	
	s.sa_flags=0;
	sigaction(SIGINT, &s, NULL);
	sleep(1);
	visualizer();
	sleep(1);
	monitor();
	do {
		x=wait(&y);
		if(x==pv && WIFEXITED(y)==0) {
			printf("Visualizer si è arrestato in modo anomalo...Riavvio in corso\n");
			visualizer();
		} else if(x==pm && WIFEXITED(y)==0){
			printf("Monitor si è arrestato in modo anomalo...Riavvio in corso\n");
			monitor();
		} 
	} while(1);
}
