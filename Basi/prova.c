#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#define DIM 2
int main() {
	int fd;
	int numeri[DIM]= {5, 7};
	int numeri2[DIM];
	fd=open("prova.txt", O_CREAT | O_RDWR, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH);
	if (fd>0) {
		printf("File creato correttamente\n");
		write(fd, (void *)numeri, DIM*sizeof(int));
		//sposta il cursore nel file all' inizio
		lseek(fd, 0, SEEK_SET); 
		read(fd, (void *)numeri2, DIM*sizeof(int));
		printf("Numero 1: %d\nNumero 2: %d\n", numeri2[0], numeri2[1]);
		printf("Somma: %d\n", numeri2[0]+numeri2[1]);
		printf("Moltiplicazione: %d\n", numeri2[0]*numeri2[1]); 
	} else {
		printf("Errore");
	}
}
