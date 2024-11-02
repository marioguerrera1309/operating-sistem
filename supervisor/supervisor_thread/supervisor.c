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
#include<sys/select.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#define DIM 1000
pthread_t v, m;
void *ret, *ret1;
char buffer[DIM];
void *visualizer() {
	while(1) {
		pause();
	}
}
void *monitor() {
	int f1, f2, x=-1, len;
	char buffer2[DIM], buffer3[DIM]={""};
	while(1) {
		f2=open("testo.txt", O_RDONLY);
		len=read(f2, (void*)buffer, DIM);
		buffer[len]='\0';
		if((strcmp(buffer, buffer3))!=0) {
			pthread_kill(v, SIGUSR1);
		}	
		strcpy(buffer3, buffer);		
		close(f2);
		sleep(1);
	}
}
void tvisualizer() {
	int a;
	a=pthread_create(&v, NULL, visualizer, (void*)ret);	
	if(a!=0) {
		printf("Errore\n");
	} else {
		printf("Thread monitor %ld\n", v);
	}
}
void tmonitor() {
	int a;
	a=pthread_create(&m, NULL, monitor, (void*)ret1);
	if(a!=0) {
		printf("Errore\n");
	} else {
		printf("Thread Visualizer %ld\n", m);
	}
}
void gestione(int sig) {
	int f, len;
	if(sig==SIGUSR1) {
		printf("%s", buffer);
	} else if(sig==SIGINT ) {
		pthread_cancel(v);
		pthread_cancel(m);
		printf("\nUscita in corso...\n");
		sleep(1);
		exit(0);
	}
}
int main() {
	int x, y, a;
	printf("Padre:%d\n", getpid());
	struct sigaction s;
	s.sa_handler=gestione;
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGINT);
	sigaddset(&(s.sa_mask), SIGUSR1);
	sigaddset(&(s.sa_mask), SIGQUIT);	
	s.sa_flags=0;
	sigaction(SIGINT, &s, NULL);
	sigaction(SIGQUIT, &s, NULL);
	sigaction(SIGUSR1, &s, NULL);
	tvisualizer();
	tmonitor();
	sleep(1);
	pthread_join(m, NULL);
	pthread_join(v, NULL);
}
