#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
void gestione(int sig) {
	printf("\n\nCiao B e' arrivato SIGINT\n");
}
int main() {
	struct sigaction sact;
	sact.sa_handler=gestione;
	sigemptyset(&(sact.sa_mask));//svuota la maschera
	sigaddset(&(sact.sa_mask), SIGQUIT);
	sact.sa_flags=0;
	sigaction(SIGINT, &sact, NULL);
	while(1) {
		pause();
	}
}
