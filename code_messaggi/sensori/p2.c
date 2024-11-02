/*
Scrivere un programma in C  in cui vi sono 4 processi (P0, P1, P2 e P3) non imparentati tra loro che si scambiano dei messaggi contenenti i valori di temperatura, pressione e umidità 
letti da tre sensori. In particolare, i processi P1, P2 e P3 inviano rispettivamente i dati relativi i sensori di temperatura, pressione e umidità.
Ogni messaggio contiene: 
- un dato di tipo float, chiamato "value" col valore di temperatura/pressione/umidità letto da un sensore
- un intero (cmd) con valore 0,1 o 2 rispettivamente se si tratta di una richiesta, di una risposta, o di una comunicazione di fine attività.

Il processo P0, che rappresenta il collettore dati, ciclicamente, per 30 volte:
- interroga P1 (cmd=0)
- interroga P2 (cmd=0) se il numero delle iterazioni svolte è pari (a turni alterni)
- interroga P3 (cmd=0) se il numero delle iterazioni svolte è multiplo di 3 (2 cicli no, 1 ciclo si)
- rimane in attesa delle risposte di P1, P2 e P3 in base all'iterazione
- stampa a video i valori di temperatura/pressione/umidità ricevuti specificando a cosa si riferisce il valore stampato
- va in pausa per 2 secondi

Dopo le 30 iterazioni, 
- P0 invia ai processi-sensore il comando 2 per comunicare di aver finito la propria attività di rilevazione dati
- rimuove la coda di messaggi
- termina

Ogni processo-sensore (P1, P2 e P3), ciclicamente:
- rimane in attesa di ricevere un comando
- se cmd vale 0
	- invia un messagio con cmd pari a 1 e value pari a un numero random tra 10 e 40. 
- se cmd vale 2
	-termina
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<unistd.h>
#include<sys/ipc.h>
struct dati {
	int cmd;
	float t, p, u; //temp, press, umidi
};
struct messaggio {
	long mtype;
	struct dati dato;
};
int main() {
	int msqid;
	struct messaggio msg;
	key_t key=ftok("/home/mario/Scrivania/sistemioperativi/code_messaggi/sensori", 0);
	msqid=msgget(key, 0);
	if(msqid==-1) {
		printf("Errore memoria");
	}		
	srand(getpid());
	while(1) {
		msgrcv(msqid, &msg, sizeof(struct messaggio), 3, 0);
		if(msg.dato.cmd==0) {
			msg.dato.cmd=1;
			msg.mtype=1;
			msg.dato.p=(rand()%30)+10;
			msgsnd(msqid, &msg, sizeof(struct messaggio), 0);
		} else if(msg.dato.cmd==2) {
			exit(0);
		}
	}
}
