
/**
 * Scrivere un programma in C su Linux in cui il main thread
* genera 4 thread per il calcolo parallelo del prezzo finale di alcuni 
* prodotti dopo aver applicato uno specifico sconto ad ogni prodotto. 
* Ad ogni thread vengono passate le informazioni di un determinato prodotto, cioè:
 *		- prezzoIniziale (float)
 *		- scontoPercentuale (int)
 *		- descrizione (stringa di 20 caratteri)
 * Ogni thread 
 *		- calcola il prezzo finale del prodotto applicando lo sconto
 * 		- stampa la descrizone del prodotto, la percentuale di sconto applicata e il prezzo finale
 *		- termina con successo
*
 * Il main thread, dopo aver acquisito da tastiera le informazione relative i prodotti e aver creato i thread
 *		- attende la terminazione dei thread
 *		- termina con successo
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
struct memoria {
	float prezzoIniziale;
	int scontoPercentuale;
	char descrizione[20];
};
void* funzione(void* args) {
	struct memoria *mem;
	mem=(struct memoria*)args;
	float prezzoFinale;
	prezzoFinale=(mem->prezzoIniziale)-(((mem->prezzoIniziale)*(mem->scontoPercentuale))/100);
	printf("Descrizione Prodotto: %s\n", mem->descrizione);
	printf("Sconto applicato: %d\n", mem->scontoPercentuale);
	printf("Prezzo finale: %.2f\n", prezzoFinale);
	pthread_exit(0);
}
int main() {
	pthread_t thread[4];
	int x;
	struct memoria mem;
	printf("Inserisci prezzo iniziale: ");
	scanf("%f", &mem.prezzoIniziale);
	printf("Inserisci sconto percentuale: ");
	scanf("%d", &mem.scontoPercentuale);
	printf("Inserisci descrizione (MAX 20): ");
	scanf("%s", mem.descrizione);
	for(int i=0;i<4;i++) {
		x=pthread_create(&thread[i], NULL, funzione, (void*)&mem);
		if(x!=0) {
			printf("Errore create thread\n");
		}
	}
	for(int i=0;i<4;i++) {
		pthread_join(thread[i], 0);
	}
	exit(0);
}
