/*
Scrivere un programma in C  in cui viene aperto un file (input.txt) in sola lettura.
Viene inoltre creato un ulteriore file (ouput.txt) in cui vengono copiati tutti i caratteri corrispondenti a lettere minuscole dell'alfabeto contenuti nel file input.txt.
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define DIM 50
int main() {
	int i, o;
	int len, x;
	char buf[DIM];
	i=open("input.txt", O_RDONLY);
	o=open("output.txt", O_WRONLY);
	len=read(i, (void*)buf, DIM);
	buf[len]='\0';
	while(len!=0) {}
	for(int i=0;i<len;i++) {
		if(buf[i]>97) {
			x=write(o, (void*)buf[i], 0);
			printf("x=%d\n", x);
		}
	}
	close(i);
	close(o);
}
