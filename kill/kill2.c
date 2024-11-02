#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
void gestore(int sig) {
	if(sig==SIGALRM) {
		printf("Hai perso\n");
		exit(1);
	}
}
char nome[50];
int main() {
	signal(SIGALRM, gestore);
	while(1) {
		printf("Hai 10 secondi per digitare il tuo nome!\n");
		alarm(10);
		scanf("%s", nome);
		while(1) {
			if(strcmp(nome, "Mario")==0) {
				alarm(0);
				printf("Bravo\n");
				exit(1);
			}
			sleep(1);
		}
	}
}
