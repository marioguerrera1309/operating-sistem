#include<stdio.h>
#include<unistd.h>
#include<signal.h>
void gestore(int sig) {
	if(sig==SIGINT) {
		printf("Il mio PID e': %d\n", getpid());
		signal(SIGINT, gestore);
	} else {
		printf("Il mio PID e': %d\n", getpid());
		signal(SIGQUIT, gestore);
	}
}
int main() {
	signal(SIGINT, gestore);
	signal(SIGQUIT, gestore);
	while(1) {
		printf("Ciao B\n");
		sleep(1);
	}
}
