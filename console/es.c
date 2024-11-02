#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<unistd.h>
int main() {
	int scelta;
	pid_t figlio;
	while(1) {
		printf("1.LIST DIRECTORY\n");
		printf("2.LIST PROCESS\n");
		printf("3.EXIT\n");
		scanf("%d", &scelta);
		switch(scelta) {
			case 1:
				figlio=fork();
				wait(NULL);
				switch(figlio) {
					case -1:
						printf("Errore");
						exit(1);
					case 0: 
						execlp("ls", "ls", "-al", NULL);
						exit(1);
					default:break;
				}
				break;
			case 2:
				figlio=fork();
				wait(NULL);
				switch(figlio) {
					case -1:
						printf("Errore");
						exit(1);
					case 0: 
						execlp("ps", "ps", "-a", NULL);
						exit(1);
					default:break;
				}
				break;
			case 3: 
				exit(1);
			default: break;		
		}
	}
}
