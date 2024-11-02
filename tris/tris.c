#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int matrice[3][3];
pthread_mutex_t mutex;
char converti(int i) {
	switch(i) {
		case 1: 
			if(matrice[0][0]==1)
				return 'X';
			else if(matrice[0][0]==0)
				return 'O';
			else
				return '\0';
			break;
		case 2:
			if(matrice[0][1]==1)
				return 'X';
			else if(matrice[0][1]==0)
				return 'O';
			else
				return '\0';
			break;
		case 3:
			if(matrice[0][2]==1)
				return 'X';
			else if(matrice[0][2]==0)
				return 'O';
			else
				return '\0';
			break;
		case 4:
			if(matrice[1][0]==1)
				return 'X';
			else if(matrice[1][0]==0)
				return 'O';
			else
				return '\0';
			break;
		case 5:
			if(matrice[1][1]==1)
				return 'X';
			else if(matrice[1][1]==0)
				return 'O';
			else
				return '\0';
			break;
		case 6:
			if(matrice[1][2]==1)
				return 'X';
			else if(matrice[1][2]==0)
				return 'O';
			else
				return '\0';
			break;
		case 7:
			if(matrice[2][0]==1)
				return 'X';
			else if(matrice[2][0]==0)
				return 'O';
			else
				return '\0';
			break;
		case 8:
			if(matrice[2][1]==1)
				return 'X';
			else if(matrice[2][1]==0)
				return 'O';
			else
				return '\0';
			break;
		case 9:
			if(matrice[2][2]==1)
				return 'X';
			else if(matrice[2][2]==0)
				return 'O';
			else
				return '\0';
			break;
		default:
			printf("Scelta non valida\n");
			break;
	}
	if(i==1) {
		return 'X';
	} else if(i==0) {
		return 'O';
	} else {
		return '\0';
	}
}
void visualizza() {
	printf(" %c | %c | %c \n %c | %c | %c \n %c | %c | %c \n", converti(1), converti(2), converti(3), converti(4), converti(5), converti(6), converti(7), converti(8), converti(9));
}
void inseriscimossax() {
	char s[3];
	int scelta;
	do {
		fflush(stdin);
		fgets(s, 2, stdin);
		sscanf(s, "%d", &scelta);
		switch(scelta) {
			case 1: 
				if(matrice[0][0]==-1) {
					matrice[0][0]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 2:
				if(matrice[0][1]==-1) {
					matrice[0][1]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 3:
				if(matrice[0][2]==-1) {
					matrice[0][2]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 4:
				if(matrice[1][0]==-1) {
					matrice[1][0]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 5:
				if(matrice[1][1]==-1) {
					matrice[1][1]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 6:
				if(matrice[1][2]==-1) {
					matrice[1][2]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 7:
				if(matrice[2][0]==-1) {
					matrice[2][0]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 8:
				if(matrice[2][1]==-1) {
					matrice[2][1]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 9:
				if(matrice[2][2]==-1) {
					matrice[2][2]=1;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;	
			default:
				break;
		}
	} while(scelta<1 || scelta>9);
}
void inseriscimossao() {
	char s[3];
	int scelta;
	do {
		fflush(stdin);
		fgets(s, 2, stdin);
		sscanf(s, "%d", &scelta);
		switch(scelta) {
			case 1: 
				if(matrice[0][0]==-1) {
					matrice[0][0]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 2:
				if(matrice[0][1]==-1) {
					matrice[0][1]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 3:
				if(matrice[0][2]==-1) {
					matrice[0][2]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 4:
				if(matrice[1][0]==-1) {
					matrice[1][0]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 5:
				if(matrice[1][1]==-1) {
					matrice[1][1]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 6:
				if(matrice[1][2]==-1) {
					matrice[1][2]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 7:
				if(matrice[2][0]==-1) {
					matrice[2][0]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 8:
				if(matrice[2][1]==-1) {
					matrice[2][1]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;
			case 9:
				if(matrice[2][2]==-1) {
					matrice[2][2]=0;
				} else {
					printf("Posto occupato\nRiprova\n");
					scelta=-1;
				}
				break;	
			default:
				break;
		}
	} while(scelta<1 || scelta>9);
}
void *funzione() {
	while(1) {
		printf("Giocatore 1\n");
		printf("Inserisci 1/2/3/4/5/6/7/8/9 per giocare\n");
		pthread_mutex_lock(&mutex);
		inseriscimossax();
		pthread_mutex_unlock(&mutex);
		visualizza();
		sleep(1);
		printf("Giocatore 2\n");
		printf("Inserisci 1/2/3/4/5/6/7/8/9 per giocare\n");
		pthread_mutex_lock(&mutex);
		inseriscimossao();
		pthread_mutex_unlock(&mutex);
		visualizza();
		sleep(1);
	}
}

int main(){
	printf("Tris\n 1 | 2 | 3 \n 4 | 5 | 6 \n 7 | 8 | 9 \n");
	pthread_t p;
	int x, j=0, flag;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			matrice[i][j]=-1;
		}
	}
	x=pthread_mutex_init(&mutex, NULL);
	if(x!=0) {
		printf("Errore\n");
	}
	x=pthread_create(&p, NULL, funzione, NULL);
	if(x!=0) {
		printf("Errore\n");
	}
	sleep(1);
	while(1) {
		pthread_mutex_lock(&mutex);
		for(int i=0;i<3;i++) {
			if(matrice[i][j]==0 && matrice[i][j+1]==0 && matrice[i][j+2]==0) {
				printf("Ha vinto la O\n");
				pthread_cancel(p);
				pthread_join(p, NULL);
				pthread_exit(0);	
			} else if(matrice[i][j]==1 && matrice[i][j+1]==1 && matrice[i][j+2]==1) {
				printf("Ha vinto la X\n");
				pthread_cancel(p);
				pthread_join(p, NULL);
				pthread_exit(0);	
			}			
		}
		for(int i=0;i<3;i++) {
			if(matrice[j][i]==0 && matrice[j+1][i]==0 && matrice[j+2][i]==0) {
				printf("Ha vinto la O\n");
				pthread_cancel(p);
				pthread_join(p, NULL);
				pthread_exit(0);	
			} else if(matrice[j][i]==1 && matrice[j+1][i]==1 && matrice[j+2][i]==1) {
				printf("Ha vinto la X\n");
				pthread_cancel(p);
				pthread_join(p, NULL);
				pthread_exit(0);	
			}			
		}
		if(matrice[0][0]==0 && matrice[1][1]==0 && matrice[2][2]==0) {
			printf("Ha vinto la O\n");
			pthread_cancel(p);
			pthread_join(p, NULL);
			pthread_exit(0);
		} else if(matrice[0][0]==1 && matrice[1][1]==1 && matrice[2][2]==1) {
			printf("Hai vinto la X\n");
			pthread_cancel(p);
			pthread_join(p, NULL);
			pthread_exit(0);
		} else if(matrice[0][2]==0 && matrice[1][1]==0 && matrice[2][0]==0) {
			printf("Ha vinto la O\n");
			pthread_cancel(p);
			pthread_join(p, NULL);
			pthread_exit(0);
		} else if(matrice[0][2]==1 && matrice[1][1]==1 && matrice[2][0]==1) {
			printf("Hai vinto la X\n");
			pthread_cancel(p);
			pthread_join(p, NULL);
			pthread_exit(0);
		}
		pthread_mutex_unlock(&mutex); 
		sleep(1);
	}
	flag=0;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			if(matrice[i][j]==-1) {
				flag=1;
			}
		}
	}
	if(flag==0) {
		printf("Pareggio\n");
		pthread_cancel(p);
		pthread_join(p, NULL);
		pthread_exit(0);
	}
	pthread_mutex_destroy(&mutex);
}
