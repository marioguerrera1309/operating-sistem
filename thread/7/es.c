/*
Scrivere un programma in C in cui il main thread genera 3 thread secondari (Thread A, Thread B e Thread C).
Tutti i thread condividono tre variabili intere (VarA, VarB e VarC), inizializzate a 0.

Il main thread, ciclicamente, esegue la sottrazione [VarC-(VarA+VarB)] 
se e solo se VarC > (VarA+VarB). 

Ogni volta che il main thread esegue tale sottrazione, stampe il rsultato a video e resetta le variabili condivise al valore 0. 
Il main thread ad ogni ciclo va in sleep per 2 secondo.

Il thread A, ciclicamente, assegna alla variabile VarA un valore random tra 2 e 8. 
Il thread A ad ogni ciclo va in sleep per 1 secondo.

Il thread B, ciclicamente, assegna alla variabile VarB un valore pari all’iterazione eseguita 
(es. 1 al primo ciclo, 2 al secondo ciclo, 3 al terzo, e cosi via). 
Il thread B ad ogni ciclo va in sleep per 1 secondi.

Il thread C, ciclicamente, assegna alla variabile VarC un valore random tra 5 e 15. 
Il thread C ad ogni ciclo va in sleep per 1 secondo.


Il main thread dopo 6 cicli attende la terminazione di tutti i thread e termina esso stesso.
I thread A e B dopo 10 cicli ciascuno terminano.
Utilizzare il meccanismo dei pthread_mutex per regolare l’accesso in sezione critica.
Il main thread resetta solo le variabili condivise (VarA, VarB e VarC), non i contatori dei cicli.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
pthread_mutex_t ma, mb, mc;
int a, b, c;
void *funzionea(void *args) {
	int x;
	srand(getpid());
	for(int i=0;i<10;i++) {
		x=(rand()%8)+2;
		pthread_mutex_lock(&ma);
		a=x;
		pthread_mutex_unlock(&ma);
		sleep(1);
	}
	pthread_exit(0);
}
void *funzioneb(void *args) {
	for(int i=0;i<10;i++) {
		pthread_mutex_lock(&mb);
		b=i+1;
		pthread_mutex_unlock(&mb);
		sleep(1);
	}
	pthread_exit(0);
}
void *funzionec(void *args) {
	int x;
	srand(getpid());
	for(int i=0;i<10;i++) {
		x=(rand()%15)+5;
		pthread_mutex_lock(&mc);
		c=x;
		pthread_mutex_unlock(&mc);
		sleep(1);
	}
	pthread_exit(0);
}
int main() {
	pthread_t ta, tb, tc;
	a=0;
	b=0;
	c=0;
	pthread_mutex_init(&ma, 0);
	pthread_mutex_init(&mb, 0);
	pthread_mutex_init(&mc, 0);
	pthread_create(&ta, 0, funzionea, NULL);
	pthread_create(&tb, 0, funzioneb, NULL);
	pthread_create(&tc, 0, funzionec, NULL);
	sleep(1);
	for(int i=0;i<6;i++) {
		pthread_mutex_lock(&ma);
		pthread_mutex_lock(&mb);
		pthread_mutex_lock(&mc);
		if(c>(a+b)) {
			c=c-(a+b);
			printf("Risultato: %d\n", c);
			a=0;
			b=0;
			c=0;
		}
		printf("a:%d b:%d c:%d\n", a, b, c);
		pthread_mutex_unlock(&ma);
		pthread_mutex_unlock(&mb);
		pthread_mutex_unlock(&mc);
		sleep(2);
	}
	pthread_join(ta, 0);
	pthread_join(tb, 0);
	pthread_join(tc, 0);
	pthread_mutex_destroy(&ma);
	pthread_mutex_destroy(&mb);
	pthread_mutex_destroy(&mc);
}
