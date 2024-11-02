/*
Si voglia realizzare un sistema Client/Server con un Server e diversi Client. Si supponga di definire una
struttura dati composta da:
typedef struct {
float vettore1[MAX], vettore2[MAX]; //MAX è una costante numerica scelta a piacere
char fine; //può assumere solo due valori: ‘s’ per finire e ‘n’ per continuare.
} message;
Ogni Client invia un messaggio di tipo message, avendo riempito i due campi vettore1 e vettore 2 (vettori di
float). Il Server riceve questo messaggio, e moltiplica ogni elemento di un vettore per il corrispondente
elemento del secondo vettore. Il risultato viene messo nel primo vettore. Ad esempio se il Client invia i due
vettori:
 vettore1=[1,2,3]
 vettore2=[4,5,6]
il vettore risultato sarà:
 vettore1=[4,10,18]
Il messaggio così ottenuto viene ritornato al Client che stampa a video il contenuto del vettore1, ossia del
risultato.
Se il Client vuole terminare la connessione, allora, quando deve spedire l’ultimo messaggio al Server,
inserisce il valore ‘s’ nel campo fine (che normalmente deve essere messo a ‘n’). Se il server legge il valore
‘s’ nel campo fine, allora anche lui termina la connessione con il client.
Si definisca un programma Server.c e un programma Client.c che realizzi quanto detto utilizzando i Sockets
e i threads. Si deve supporre che per ogni client connesso, il server crei un thread apposito che gestisce la
comunicazione con il client. Quando il client si disconnette, il thread finisce. Per evitare che il Server debba
invocare la pthread_join, si deve realizzare ciascun thread in modo detached. Questo è molto
importante perché in questo modo le risorse del thread vengono rilasciate dal SO alla terminazione
del thread.
Il Server deve essere lanciato in modo background. Il Client in modo foreground. Si può supporre anche di
aprire diversi terminali (applicativo Terminal) e da ogni terminale aperto si può lanciare il programma client
sempre in foreground. In questo modo si possono eseguire tanti programmi client. Ricordarsi alla fine di
killare il processo Server (kill %1).
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#define MAX 20
typedef struct {
	float vettore1[MAX], vettore2[MAX];
	char fine;
}message;
int main() {
	int sock, x;
	message mes;
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9876);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock=socket(AF_INET, SOCK_STREAM, 0);
	x=connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	srand(getpid());
	mes.fine='n';
	do {
		for(int i=0;i<MAX;i++) {
		mes.vettore1[i]=(rand()%10)+1;
		mes.vettore2[i]=(rand()%10)+1;
		}
		printf("Vettore1:\n");
		for(int i=0;i<MAX;i++) {
			printf("%.2f ", mes.vettore1[i]);
		}
		printf("\nVettore2:\n");
		for(int i=0;i<MAX;i++) {
			printf("%.2f ", mes.vettore2[i]);
		}
		printf("\n");
		write(sock, (void*)&mes, sizeof(message));
		read(sock, (void*)&mes, sizeof(message));
		printf("Vettore risultato: ");
		for(int i=0;i<MAX;i++) {
			printf("%.2f ", mes.vettore1[i]);
		}
		printf("\n");
		printf("Se vuoi continuare inserisci n, altrimenti s: ");
		x=0;
		do {
			if(x>0)
				printf("Inserire un carattere valido\n");
			scanf("%c", &(mes.fine));
			while(getchar()!='\n');
			x++;
		} while(mes.fine!='n' && mes.fine!='s');
	} while(mes.fine=='n');
	write(sock, (void*)&mes, sizeof(message));
	close(sock);
	printf("Connessione conclusa\n");
}
