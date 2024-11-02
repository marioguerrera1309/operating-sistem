/*
Scrivere un programma in C in cui un processo principale (P) genera 4 thread, di cui 2 del tipo increment_thread e 2 del tipo decrement_thread.
Tutti i thread condividono una sola variabile intera, inizializzata a 0.
Ogni increment_thread, ciclicamente, 
	- incrementa di un’unità la variabile condivisa se e solo se il valore della stessa è inferiore a 10.

Ogni decrement_thread, ciclicamente, 
	- decrementa di un’unità la variabile condivisa se e solo se il valore della stessa è superiore a 5.

L’iterazione termina dopo 7 cicli, indipendentemente dal fatto che l’operazione di incremento (o decremento) sia stata svolta o meno in ognuno di essi. 
Solo quando la variabile condivisa viene modificata, un thread stampa a video:
-	L’operazione eseguita.
-	Il valore della variabile condivisa, prima e dopo l’operazione.

Tutti i thread ad ogni iterazione vanno in sleep per un secondo (chiamare la funzione sleep fuori dalla sezione critica).

Il processo principale dopo aver creato i thread attende la terminazione di tutti.
Utilizzare il meccanismo dei pthread_mutex per regolare l’accesso in sezione critica.
*/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
//0-1 increment
//2-3 decrement
pthread_mutex_t mutex;
int v;
void *funzione(int i) {
	printf("i:%d\n", i);
	if(i>=1) {
		for(int i=0;i<7;i++) {
			pthread_mutex_lock(&mutex);
			if(v<10) {
				v++;
				printf("Incremento effettuato: %d\n", v);
			}
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	} else {
		for(int i=0;i<7;i++) {
			pthread_mutex_lock(&mutex);
			if(v>5) {
				v--;
				printf("Decremento effettuato: %d\n", v);
			}
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	}
}
int main() {
	int x;
	v=0;
	pthread_t thread[4];
	pthread_mutex_init(&mutex, 0);
	for(int i=0;i<4;i++) {
		x=pthread_create(&thread[i], 0, funzione, (void*)i);
		if(x==0) {
			printf("Thread %d creato\n", i+1);
		}
	}
	for(int i=0;i<4;i++) {
		pthread_join(thread[i], 0);
	}
	exit(0);
}
