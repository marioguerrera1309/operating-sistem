/*
	Scrivere un programma in C su Linux in cui il main thread genera
	2 thread(giocatore_1 e giocatore_2) per iniziare un gioco.
	I due thread condividono una variabile g che viene inizializzata dal main thread
	con un valore random compreso tra 100 e 150.
	Il gioco si svolge come segue. 
	Ad ogni turno, il giocatore (thread) invitato a giocare:
		-genera un numero casuale r tra 0 e l'attuale valore di g;
		-sottrae r a g;
	Il main thread è l'arbitro del gioco e :
		-sceglie in modo casuale(generando un numero tra 1 e 2) il thread che inizia il gioco;
		- da quel momento in poi l'arbitro invita i giocatori in modo alternato (ad es. 1-2-1-2...);
		-ad ogni "giocata" controlla se vi è stato un vincitore. Ovvero valuta il valore di g,
			se questo è 0, dichiara il vincitore(vince il giocatore che per primo azzera g).
	UTILIZZARE I MUTEX PER REALIZZARE IL GIOCO.
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
int g;
pthread_mutex_t m1, m2, mmain;
void *funzione1(void *args) {
	int r;
	srand(getpid());
	while(1) {
		pthread_mutex_lock(&m1);
		sleep(1);
		r=(rand()%(g+1));//rand%2 ritorna o 0 o 1 mai 2
		g=g-r;
		printf("G1: %d\n", g);
		pthread_mutex_unlock(&mmain);
	}
}
void *funzione2(void *args) {
	int r;
	srand(getpid());
	while(1) {
		pthread_mutex_lock(&m2);
		sleep(1);
		r=(rand()%(g+1));//rand%2 ritorna o 0 o 1 mai 2
		g=g-r;
		printf("G2: %d\n", g);
		pthread_mutex_unlock(&mmain);
	}
}
int main() {
	srand(getpid());
	int x, turno;
	x=(rand()%150)+100;
	g=x;
	pthread_mutex_init(&m1, 0);
	pthread_mutex_init(&m2, 0);
	pthread_mutex_init(&mmain, 0);
	pthread_t t1, t2;
	pthread_mutex_lock(&m1);
	pthread_mutex_lock(&m2);
	pthread_mutex_lock(&mmain);
	pthread_create(&t1, 0, funzione1, NULL);
	pthread_create(&t2, 0, funzione2, NULL);
	x=(rand()%2)+1;
	if(x==1) {
		turno=1;
		pthread_mutex_unlock(&m1);
	} else {
		turno=2;
		pthread_mutex_unlock(&m2);
	}
	while(1) {
		pthread_mutex_lock(&mmain);
		if(g==0) {
			if(turno==1) {
				printf("Ha vinto il giocatore 1\n");
			} else {
				printf("Ha vinto il giocatore 2\n");
			}
			pthread_mutex_unlock(&m1);
			pthread_mutex_unlock(&m2);
			pthread_cancel(t1);
			pthread_cancel(t2);
			break;
		} else {
			if(turno==1) {
				pthread_mutex_unlock(&m2);
				turno=2;
			} else {
				pthread_mutex_unlock(&m1);
				turno=1;
			}		
		}
	}
	pthread_join(t1, 0);
	pthread_join(t2, 0);
	pthread_mutex_destroy(&m1);
	pthread_mutex_destroy(&m2);
	pthread_mutex_destroy(&mmain);
}
