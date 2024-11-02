#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
void funzione(int sig) {
	if(sig==SIGQUIT) {
		exit(0);
	} else {
		if(getpid()%2==0) printf("Pari: %d\n", getpid());
		else printf("Dispari: %d\n", getpid());
	}
}
int main() {
	pid_t figli[5];
	struct sigaction s;
	s.sa_handler=funzione;
	sigemptyset(&s.sa_mask);
	sigaddset(&s.sa_mask, SIGUSR1);
	sigaddset(&s.sa_mask, SIGQUIT);
	sigaction(SIGUSR1, &s, NULL);
	sigaction(SIGQUIT, &s, NULL);
	for(int i=0;i<5;i++) {
		figli[i]=fork();
		switch(figli[i]) {
			case 0:
				while(1) {
					pause();
				}
			default:
				break;
		}
	}
	for(int i=0;i<5;i++) {
		for(int j=0;j<5;j++) {
			kill(figli[j], SIGUSR1);
		}
		printf("\n");
		sleep(1);
	}	
	for(int i=0;i<5;i++) {
		kill(figli[i], SIGQUIT);
		wait(0);
	}
}
