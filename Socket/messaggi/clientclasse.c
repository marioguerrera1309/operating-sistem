#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORTA 9876

void *thread_func(void *args);

int main() {
    struct sockaddr_in saddr;
    int sock;
    char username[50];
    char buff[250];
    char msg[240];
    pthread_t tid;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORTA);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Inserisci username: ");
    fgets(username, 50, stdin);
    username[strlen(username)-1] = '\0';

    if(connect(sock, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("Errore connect:");
        exit(EXIT_FAILURE);
    }

    pthread_create(&tid, NULL, thread_func, (void *)&sock);

    snprintf(buff, 250, "register %s", username);
    write(sock, buff, strlen(buff)+1);

    while(1) {
        printf("Inserisci messaggio: ");
        fgets(msg, 240, stdin);
        msg[strlen(msg)-1] = '\0';

        if(strncmp(msg, "close", 5) == 0) {
            strcpy(buff, "close");
            write(sock, buff, strlen(buff)+1);
            break;
        } else {
            snprintf(buff, 250, "send %s", msg);
            write(sock, buff, strlen(buff)+1);
        }
    }

    close(sock);
    printf("Bye!\n");
}

void *thread_func(void *args) {
    int sock = *((int *)args);
    char buff[250];
    int r=1, mid=0, rid=0;

    while(1) {
        sleep(1);
        strcpy(buff, "read");
        write(sock, buff, strlen(buff)+1);
        r = 1;
        while(r) {
            if(read(sock, buff, 250) > 0) {
                if(strncmp(buff, "Welcome", 7) != 0 && strncmp(buff, "ok", 2) != 0) {
                    sscanf(buff, "%d %*s", &mid); //Salva l'id del messaggio in una variabile
                    r = 0;
                    if(mid != rid) { //Se l'id è diverso dal precedente stampato lo stampa
		        printf("\n%s\nInserisci messaggio: ", buff);
		        rid = mid;
		    }
                } else {
                    printf("\n%s\n", buff);
                }   
            } else {
                pthread_exit(EXIT_SUCCESS);
            }
        }
    }
}
