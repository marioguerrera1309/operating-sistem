/*Scrivere un programma che dimostri che:
1) i processi padre e figlio hanno due copie di file descriptor con lo stesso numero identificativo.
2) Il file pointer viene modificato da entrambi i processi padre e figlio
3) se uno dei due processi chiude il file, l'altro rimane aperto e può essere utilizzato dell'altro processo */
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
int main(void) {
	int f=open("file.txt", O_CREAT | O_WRONLY, 0666);
	pid_t figlio;
	figlio=fork();
	switch(figlio) {
		case -1:
			printf("Errore");
			exit(EXIT_FAILURE);
		case 0:
			printf("Sono il figlio\nIl file descriptor è: %d\n", f);
			write(f, "Hello ", 6);
			close (f);
			exit(EXIT_SUCCESS);
		default:
		break;
	}
	printf("Sono il padre\nIl file descriptor è: %d\n", f);
	printf("Premere un tasto per continuare\n");
	getchar();
	write(f, "world\n", 6);
	close(f);
	exit(EXIT_SUCCESS);
}
