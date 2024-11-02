/**
* Scrivere un programma in C su Linux in cui il main thread
* genera 2 thread (giocatore_1 e giocatore_2) per iniziare un gioco.  
* I due thread condividono una variabile g che viene inizializzata dal main thread con un valore random compreso tra 100 e 150. 
* Il gioco si svolge come segue: ad ogni turno, il giocatore (thread) invitato a giocare:
*	- genera un numero causale r tra 0 e l'attuale valore di g
* 	- sottrae r a g
*   - l'arbitro valuta il nuovo valore di g. Se questo è 0, dichiara il vincitore (vince il giocatore che per primo azzera g) 
* Il main thread è l'arbitro del gioco. 
*		- Sceglie in modo casuale (generando un numero tra 1 e 2) il thread che inizia il gioco.
* 		- Da quel momento in poi l'arbitro invita i giocatori in modo alternato (ad es. 1-2-1-2-1-2-...)
*		- Ad ogni "giocata" controlla se vi è stato un vincitore.
*		- Se viene dichiarato un vincitore, il main thread cancella i due thread giocatori.
*	Utilizzare i mutex per realizzare il gioco.
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
int g;
pthread_mutex_t gmain, mg1, mg2;
void *funzione(void *args) {
	int *giocatore=(int*)args;
	int r;
	srand(getpid());
	while(1) {
		if(*giocatore==1) {
			pthread_mutex_lock(&mg1);
		} else {
			pthread_mutex_lock(&mg2);
		}
		sleep(1);
		r=(rand()%(g+1));
		g=g-r;
		printf("Giocatore %d g=%d\n", *giocatore, g);
		pthread_mutex_unlock(&gmain);
	}
}
int main() {
	int x, y, turno;
	pthread_t g1, g2;
	srand(getpid());
	g=(rand()%150)+100;
	turno=(rand()%2)+1;
	pthread_mutex_init(&gmain, 0);
	pthread_mutex_init(&mg1, 0);
	pthread_mutex_init(&mg2, 0);
	pthread_mutex_lock(&mg1);
	pthread_mutex_lock(&mg2);
	pthread_mutex_lock(&gmain);//blocco me stesso così solo un giocatore quando finisce può sbloccarmi
	x=1;
	pthread_create(&g1, 0, funzione, (void*)&x);
	y=2;
	pthread_create(&g2, 0, funzione, (void*)&y);
	if(turno==1) {
		pthread_mutex_unlock(&mg1);
	} else {
		pthread_mutex_unlock(&mg2);
	}
	while(1) {
		pthread_mutex_lock(&gmain); //aspetto che un giocatore mi sblocchi
		if(g==0) {
			break;
		} else {
			if(turno==1) {
				turno=2;
				pthread_mutex_unlock(&mg2);
			} else {
				turno=1;
				pthread_mutex_unlock(&mg1);
			}
		}
	}
	printf("Ha vinto Giocatore %d\n", turno);
	pthread_cancel(g1);
	pthread_cancel(g2);
	pthread_mutex_unlock(&mg1);
	pthread_mutex_unlock(&mg2);
	pthread_mutex_destroy(&gmain);
	pthread_mutex_destroy(&mg1);
	pthread_mutex_destroy(&mg2);
	pthread_join(g1, 0);
	pthread_join(g2, 0);
	exit(0);
}
