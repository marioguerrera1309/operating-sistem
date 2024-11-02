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
int g;
pthread_mutex_t mutex;
void *funzione(void *args) {
	int oldg=-1;
	int *cont;
	cont=(int*)malloc(sizeof(int));
	*cont=0;
	for(int i=0;i<10;i++) {
		pthread_mutex_lock(&mutex);
		if(g!=oldg) {
			g++;
			oldg=g;
			(*cont)++;
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	pthread_exit((void*)cont);
}
int main() {
	int max, pos, oldg=-1;
	void *x[3];
	g=0;
	pthread_t t[3];
	for(int i=0;i<3;i++) {
		pthread_create(&t[i], 0, funzione, NULL);
	}
	for(int i=0;i<10;i++) {
		pthread_mutex_lock(&mutex);
		if(g!=oldg) {
			printf("G: %d\n", g);
			oldg=g;
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	for(int i=0;i<3;i++) {
		pthread_join(t[i], &x[i]);
		printf("x[%d]:%d ", i, *(int*)x[i]);
	}
	max=*(int*)x[0];
	pos=0;
	for(int i=0;i<3;i++) {
		if(*(int*)x[i]>max) {
			max=*(int*)x[i];
			pos=i;
		}
	}
	pthread_mutex_destroy(&mutex);
	printf("\nVincitore Giocatore %d con %d cambi\n", pos, max);	
}
