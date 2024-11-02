#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
void p(int sig) {
	printf("Pong\n");
}
void f(int sig) {
	if(sig==SIGUSR1) {
		printf("Ping..");
		fflush(stdout);
		kill(getppid(), SIGUSR1);
	} else {
		exit(0);
	}
}
int main() {
	pid_t figlio;
	figlio=fork();
	struct sigaction s;
	s.sa_flags=0;
	sigemptyset(&s.sa_mask);
	switch(figlio) {
		case 0:
			sigaddset(&s.sa_mask, SIGUSR1);
			sigaddset(&s.sa_mask, SIGQUIT);
			s.sa_handler=f;
			sigaction(SIGUSR1, &s, NULL);
			sigaction(SIGQUIT, &s, NULL);
			while(1) {
				pause();
			}
			break;
		default:
			break;
	}
	sigaddset(&s.sa_mask, SIGUSR1);
	s.sa_handler=p;
	sigaction(SIGUSR1, &s, NULL);
	sleep(1);
	kill(figlio, SIGUSR1);
	for(int i=0;i<5;i++) {
		pause();
		sleep(1);
		kill(figlio, SIGUSR1);
	}
	kill(figlio, SIGQUIT);
	wait(0);
	exit(0);
}
