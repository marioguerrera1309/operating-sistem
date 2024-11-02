#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define DIM 50
int main(int argc, char *argv[]) {
	int f1, f2, i, len;
	pid_t figlio;
	char s[DIM], buffer[DIM];
	if( argc<2 ) {
		printf("Argomenti insufficienti");
		exit(EXIT_FAILURE);
	}
	figlio=fork();
	fflush(stdin);
	switch(figlio) {
		case -1: 
			printf("Errore fork");
			exit(EXIT_FAILURE);
		case 0: 
			f1=open(argv[1], O_CREAT | O_WRONLY, 0666);
			//lseek(f1, 0, SEEK_SET);
			for(i=0; i<3; i++) {
				fflush(stdin);
				printf("Inserisci stringa: ");
				fgets(s, DIM, stdin);
				fflush(stdin);
				write(f1, (void* )s, strlen(s));	
			} 
			exit(EXIT_SUCCESS);
		default:
		break;
	}
	i=0;
	len=1;
	f2=open(argv[1], O_RDONLY);
	while(i<3) {
		len=read(f2, (void *)buffer, DIM);
		buffer[len]= '\0';
		if(len>0) {
			printf("\nHai scritto: %s\n", buffer);
			i++;
		}
	}
	close (f1);
	close (f2);
}
