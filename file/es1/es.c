#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define DIM 50
// $ ./nomeprog arg1 arg2 arg3
int main(int argc, char *argv[]) {
	if( argc<3 ) {
		printf("Numero argomenti insufficienti");
		exit(EXIT_FAILURE);
	}
	int i=0;
	int len=0;	
	char buffer[DIM];
	int f1=open(argv[1], O_RDONLY);
	int f2=open(argv[2], O_CREAT | O_WRONLY, 0666);
	if( f1>0 ) {
		printf("File aperto\n");
	} else {
		printf("Errore");
		exit(EXIT_FAILURE);
	}
	if( f2>0 ) {
		printf("File creato");
	} else {
		printf("Errore");
		exit(EXIT_FAILURE);
	}
	lseek(f2, 0, SEEK_SET);
	do {
		len=read(f1, (void *)buffer, DIM);
		for(i=0; i<len; i++) {
			if(buffer[i]==' ') {
				buffer[i]= '\n';
			}
		}
		write(f2, (void *)buffer, len);
	} while( len>0 );
	close(f1);
	close(f2);
	printf("\nFile scritto con successo\n");
	exit(EXIT_SUCCESS);
}
