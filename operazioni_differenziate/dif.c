/*
Esercizio 2: Operazioni differenziate
Scrivere un programma multithread in C su Linux.
Il main thread inizializza due variabili float globali due valori a scelta.
In seguito il main thread crea 2 thread secondari (T1 e T2) detached, ognuno dei quali fa compiere un'operazione differente.
T1 somma i due numeri e li stampa a video.
T2 moltiplica i due numeri e li stampa a video.
Il main thread, dopo aver creato i due thread, attende 1 secondo e termina.
*/
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
pthread_t p1, p2;
pthread_attr_t t;
float x=2.5, y=3.4;
void *f1() {
	printf("Somma: %.2f\n", (x+y));
	pthread_exit(0); 
}
void *f2() {
	printf("Prodotto: %.2f\n", (x*y));
	pthread_exit(0); 
}
int main() {
	int z;
	pthread_attr_init(&t);
	pthread_attr_setdetachstate(&t, PTHREAD_CREATE_DETACHED);
	z=pthread_create(&p1, &t, f1, NULL);
	if(z!=0) {
		printf("Errore\n");
	}
	z=pthread_create(&p2, &t, f2, NULL);
	if(z!=0) {
		printf("Errore\n");
	}
	sleep(1);
	pthread_exit(0);	
}
