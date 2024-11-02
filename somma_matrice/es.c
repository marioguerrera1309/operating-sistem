#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define N 10
#define M 10
pthread_mutex_t mutex; 
int somma=0;
int matrice[N][M];
void *funzione(void* r) {
	int riga = (int) r;
	int x=0;
	for(int i=0; i<M; i++) {
		x+=matrice[riga][i];
	}
	pthread_mutex_lock(&mutex);
	somma+=x;
	pthread_mutex_unlock(&mutex);
	pthread_exit()
}
int main() {
	pthread_t tid[N];
	int x;
	for(int i=0;i<N;i++) {
		for(int j=0;j<M;j++) {
			matrice[i][j]=(i*10)+j+1;
			printf("%d ", matrice[i][j]);
		}
		printf("\n");
	}
	x=pthread_mutex_init(&mutex, NULL);
	for(int i=0;i<N;i++) {
		pthread_create(&(tid[i]), NULL, *funzione, (void*)i);
	}
	for(int i=0;i<N; i++) {
		pthread_cancel(tid[i]);
		pthread_join(tid[i], NULL);
	}
	printf("Somma=%d\n", somma);
}
