/*
Esercizio 4: Uso della cancel e della join.
Scrivere un programma multithread in C su Linux.
Il main thread crea due thread secondari T1 e T2.
T1, in un ciclo infinito, ogni secondo stampa il valore restituito dalla funzione time(NULL).
T2 attende la terminazione di T1 e stampa a video "Bye!\n" e termina.
Il main thread, dopo aver creato i T1 e T2, attende 5 secondi, cancella il thread T1 e attende la terminazione
di T2 e termina esso stesso.
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
time_t time(time_t *tloc);
pthread_t t1, t2;
void *f1() {
	int x;
	while(1) {
		x=time(NULL);
		printf("TIME: %d\n", x);
		sleep(1);
	}
}
void *f2() {
	int x;
	x=pthread_join(t1, NULL);
	if(x!=0) {
		printf("Errore\n");
	}
	printf("Bye\n");
	pthread_exit(0);
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
	sleep(5);
	x=pthread_cancel(t1);
	if(x!=0) {
		printf("Errore\n");
	}
	x=pthread_join(t2, NULL);
	if(x!=0) {
		printf("Errore\n");
	}
	pthread_exit(0);
}
