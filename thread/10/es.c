/*
Scrivere un programma in C in cui un main thread (M) genera 3 thread secondari.
Tutti i thread condividono una sola variabile intera, inizializzata a 0.

Ogni thread secondario, ciclicamente, accede alla variabile condivisa.
Se il valore di tale variabile è cambiato dall’ultimo accesso, il thread incrementa la variabile condivisa di un’unità e incrementa anche il numero di volte che l’ha modificata.
Indipendentemente dal fatto che l’operazione di incremento sia stata svolta o meno, l’iterazione termina dopo 10 cicli. Ogni thread termina con stato di terminazione
pari al numero di volte che ha modificato la variabile.
In questo semplice gioco vince il thread che ha modificato più volte la variabile condivisa.

Il main thread, dopo aver creato i thread secondari, stampa a video il valore della variabile ogni volta che la trova cambiata.

Tutti i thread (compreso il main thread) ad ogni iterazione vanno in sleep per un secondo (chiamare la funzione sleep fuori dalla sezione critica).
Dopo 10 cicli il main thread attende la terminazione dei thread secondari, dichiara il vincitore e termina esso stesso.

Utilizzare il meccanismo dei pthread_mutex per regolare l’accesso alla sezione critica.
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
void *funzione(void *args) {
	int *x;
	x=(int*)args;
	(*x)++;
	pthread_exit((void*)x);
}
int main() {
	int x=7;
	pthread_t t;
	void *y;
	pthread_create(&t, 0, funzione, (void*)&x);
	pthread_join(t, &y);
	printf("x=%d\n", *(int*)y);
}
