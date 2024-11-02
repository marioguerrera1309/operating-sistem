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
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
void *giocatore1() {
	
}
void *giocatore2() {
	
}
int g;
int main() {
	int x;
	pthread_t g1, g2;
	srand(getpid());
	g=(rand()%150)+100;
	x=(rand()%2)+1;
	if(x==1) {
		pthread_create(g1, 0, giocatore1, 0);
	} else {
		pthread_create(g2, 0, giocatore2, 0);
	}
	
}
