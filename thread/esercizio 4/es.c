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
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
pthread_mutex_t mutex;
int v;
void *funzione() {
	int x=-1, cont=0;
	for(int i=0;i<10;i++) {
		pthread_mutex_lock(&mutex);
		if(v!=x) {
			v++;
			cont++;
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	pthread_exit((void*)cont);
}
int main() {
	int x, y=0;
	pthread_t t[3];
	void* res;
	int max=0;
	v=0;
	pthread_mutex_init(&mutex, 0);
	for(int i=0;i<3;i++) {
		x=pthread_create(&t[i], 0, funzione, 0);
		if(x!=0) {
			printf("Thread %d creato\n", i);
		}
	}
	for(int i=0;i<10;i++) {
		pthread_mutex_lock(&mutex);
		if(v!=y) {
			printf("Main Thread: %d\n", v);
			y=v;
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	x=-1;
	for(int i=0;i<3;i++) {
		pthread_join(t[i], &res);
		if((int)res>max) {
			max=(int)res;
			x=i;
		}
	}	
	printf("Ha vinto il thread %d con %d modifiche\n", x, max);
}
