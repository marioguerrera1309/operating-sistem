#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#define N 10
void gfiglio(int sig) {
	if(sig==SIGUSR1) {
		printf("Ping -> ");
		fflush(stdout);
		kill(getppid(), SIGUSR1);
	} else {
		printf("Bye\n");
		exit(1);
	}
}
void gpadre(int sig) {
	printf("Pong\n");
}
int main() {
	pid_t figlio;
	figlio=fork();
	struct sigaction s;
	sigemptyset(&(s.sa_mask));
	sigaddset(&(s.sa_mask), SIGUSR1);
	sigaddset(&(s.sa_mask), SIGQUIT);
	s.sa_flags=0;
	switch(figlio) {
		case -1:
			printf("Errore\n");
			exit(1);
		case 0:
			//codice figlio
			s.sa_handler=gfiglio;
			sigaction(SIGUSR1, &s, NULL);
			sigaction(SIGQUIT, &s, NULL);
			while(1) {
				pause();
			}
			exit(1);
		default:
			break;
	}
	s.sa_handler=gpadre;
	sigaction(SIGUSR1, &s, NULL);
	sleep(1);
	kill(figlio, SIGUSR1);
	for(int i=0;i<N;i++) {
		pause();
		sleep(1);
		kill(figlio, SIGUSR1);
	}
	kill(figlio, SIGQUIT);	
	wait(NULL);
	exit(1);	
}
