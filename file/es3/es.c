/*
Scrivere un programma in C  in cui viene aperto un file (input.txt) in sola lettura.
Viene inoltre creato un ulteriore file (ouput.txt) in cui vengono copiati tutti i caratteri corrispondenti a lettere minuscole dell'alfabeto contenuti nel file input.txt.
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
int main() {
	int input, output, x, i;
	char buf[50];
	input=open("input.txt", O_RDONLY);
	output=open("output.txt", O_CREAT | O_WRONLY, 0666);
	do {
		x=read(input, (void*)buf, 50);
		if(x>0) {
			buf[x]='\0';
			
		}
		printf("buf: %s\nx: %d\n", buf, x);
		i=0;
		while(i==(x+1)) {
			if(buf[i]>97) {
				buf[i]="";
			}
			i++;
		}
		write(output, (void*)buf, strlen(buf));
	}while(x>0);
}
