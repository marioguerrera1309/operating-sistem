/**
 * Scrivere un programma in C che permetta la raccolta dati
 * da una serie di sensori (3 sensori).
 * 
 * Ogni sensore è rappresentato da un processo/thread.
 * - Il primo sensore periodicamente e ogni secondo monitora
 *   la temperatura di una stanza.
 * - Il secondo sensore, periodicamente e ogni secondo, monitora
 *   l'umidità di una stanza.
 * - Il terzo sensore, periodicamente e ogni secondo, monitora
 *   il consumo energetico.
 * 
 * Tutti i sensori inviano i propri valori al
 * main thread/processo padre che si occuperà di stampare a video
 * i valori dei tre sensori ogni 2 secondi.
 * 
 * Dopo 20 secondi di esecuzione tutti i processi/thread sensori
 * imposteranno il valore del sensore a -1 per comunicare al main
 * thread/processo padre che hanno terminato.
 *
 * Il processo padre/main thread, quando leggerà che il valore di
 * tutti e tre i sensori è uguale a -1 termina.
 * 
 * Utilizzare thread/mutex oppure processi/shared memory/semafori
 * per l'implementazione del programma.
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
int t, u, c;
pthread_mutex_t mt, mu, mc;
void *funzione(void *args) {
	int *x, r;
	x=(int*)args;
	srand(getpid());
	if(*x==0) {
		//temperatura
		for(int i=0;i<20;i++) {
			r=(rand()%40)+10;
			pthread_mutex_lock(&mt);
			t=r;
			pthread_mutex_unlock(&mt);
			sleep(1);
		}
		pthread_mutex_lock(&mt);
		t=-1;
		pthread_mutex_unlock(&mt);
		pthread_exit(0);
	} else if(*x==1) {
		//umidità
		for(int i=0;i<20;i++) {
			r=(rand()%100)+1;
			pthread_mutex_lock(&mu);
			u=r;
			pthread_mutex_unlock(&mu);
			sleep(1);
		}
		pthread_mutex_lock(&mu);
		u=-1;
		pthread_mutex_unlock(&mu);
		pthread_exit(0);
	} else if(*x==2) {
		//consumo
		for(int i=0;i<20;i++) {
			r=(rand()%500)+100;
			pthread_mutex_lock(&mc);
			c=r;
			pthread_mutex_unlock(&mc);
			sleep(1);
		}
		pthread_mutex_lock(&mc);
		c=-1;
		pthread_mutex_unlock(&mc);
		pthread_exit(0);
	} else {
		printf("Errore\n");
	}
}
int main() {
	int x, y[3];
	pthread_t thread[3];
	pthread_mutex_init(&mt, 0);
	pthread_mutex_init(&mu, 0);
	pthread_mutex_init(&mc, 0);
	t=0;
	u=0;
	c=0;
	y[0]=0;
	y[1]=1;
	y[2]=2;
	for(int i=0;i<3;i++) {
		pthread_create(&thread[i], 0, funzione, (void*)&y[i]);
	}
	pthread_mutex_lock(&mt);
	pthread_mutex_lock(&mc);
	pthread_mutex_lock(&mu);
	do {
		pthread_mutex_unlock(&mt);
		pthread_mutex_unlock(&mc);
		pthread_mutex_unlock(&mu);
		sleep(2);
		pthread_mutex_lock(&mt);
		pthread_mutex_lock(&mc);
		pthread_mutex_lock(&mu);
		if(t!=-1 && c!=-1 && u!=-1) {
			printf("Temperatura: %d\nUmidità: %d\nConsumo: %d\n", t, u, c);			
		}	
	} while(t!=-1 && c!=-1 && u!=-1);
	for(int i=0;i<3;i++) {
		pthread_join(thread[i], 0);
	}
	exit(0);
}
