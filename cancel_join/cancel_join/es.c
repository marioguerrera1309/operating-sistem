/*
Esercizio 3: Uso della cancel e della join.
Scrivere un programma multithread in C su Linux.
Il main thread crea due thread secondari T1 e T2.
T1, in un ciclo infinito, ogni secondo stampa il valore restituito dalla funzione time(NULL).
T2 in un ciclo infinito, ogni 2 secondi stampa il numero di cicli effettuati.
Il main thread, dopo aver creato i due thread, attende 10 secondi, cancella i due thread creati e ne attende la
terminazione. Dopo termina esso stesso.
#include <time.h>
time_t time(time_t *tloc);
//Restituisce il numero di secondi trascorsi dal 1/1/1970 alle ore 00.00 (UNIX Epoch).
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>
pthread_t t1, t2;
void *f1() {
	int x;
	time_t time(time_t *tloc);
	while(1) {
		x=time(NULL);
		printf("TIME:%d\n", x);
		sleep(1);
	}
}
void *f2() {
	int x=0;
	while(1) {
		printf("X:%d\n", x);
		x++;
		sleep(2);
	}
}
int main() {
	int x;
	x=pthread_create(&t1, NULL, f1, NULL);	
	if(x!=0) {
		printf("Errore\n");
	}
	x=pthread_create(&t2, NULL, f2, NULL);
	if(x!=0) {
		printf("Errore\n");
	}
	sleep(10);
	x=pthread_cancel(t1);
	if(x!=0) {
		printf("Errore\n");
	}
	x=pthread_cancel(t2);
	if(x!=0) {
		printf("Errore\n");
	}
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
