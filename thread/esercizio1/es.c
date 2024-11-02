/**
* Scrivere un programma in C su Linux in cui il main thread
* genera n thread (con n<=5).
* Al thread i-esimo (con i che va da 0 a n-1) viene passato un intero <arg> pari ad i+1.
* Ogni thread 
*		- stampa le prime 5 potenze del numero passato come argomento
* 		- dopo ogni stampa va in sleep per 2 secondi.
*	Dopo le 5 iterazioni termina con successo
*
* Il main thread 
*		- attende la terminazione dei thread
*		- termina con successo
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/wait.h>
#include<math.h>
#define N 5
void *funzione(void* arg) {
	int x=(int)arg;
	int j;
	for(int i=0;i<5;i++) {
		printf("%d: %.2f\n", i+1, pow(x, i));
		sleep(2);
	}
	pthread_exit(NULL);
}
int main() {
	int x=0, i;
	pthread_t t[N];
	for(i=0;i<N;i++) {
		x=pthread_create(&t[i], (void*)NULL, funzione, (void*)i+1);
		if(x!=0) {
			printf("Errore\n");
		}
	}
	for(int i=0;i<N;i++) {
		pthread_join(t[i], NULL);
	}
	exit(EXIT_SUCCESS);
}
