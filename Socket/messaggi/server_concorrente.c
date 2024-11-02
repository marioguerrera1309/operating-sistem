#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

void *worker_thread(void *args);

struct dati {
    char messaggio[140];
    char utente[50];
    int id;
};

struct dati ultimo_msg;
pthread_mutex_t mutex;

int main() {
    int ssock, csock;
    struct sockaddr_in saddr;
    pthread_t tid;
    pthread_attr_t tattr;

    /** Inizializziamo la struttura dati condivisa */
    ultimo_msg.id = 0;
    ultimo_msg.messaggio[0] = '\0';
    ultimo_msg.utente[0] = '\0';

    /** Inizializziamo il mutex */
    pthread_mutex_init(&mutex, NULL);

    /** Inizializziamo gli attributi del thread */
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);

    /** Creazione socket stream dominio Internet */
    ssock = socket(AF_INET, SOCK_STREAM, 0);
    /** controllo errore */

    saddr.sin_family = AF_INET;
    /** htons converte uno short (16bit) in formato
     * network (big-endian) */
    saddr.sin_port = htons(9876);

    /** INADDR_ANY indica che il server accetta
     * connessioni da qualsiasi indizzo IP.
     * htonl converte un long (32 bit) in formato network */
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /** Chiamata bind per assegnare un indirizzo al socket */
    if(bind(ssock, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        perror("Errore: ");
        exit(EXIT_FAILURE);
    }

    /** Server in ascolto di nuove connessioni */
    listen(ssock, 5);

    while(1) {
        /** accetta connesione */
        csock = accept(ssock, NULL, NULL);
        pthread_create(&tid, &tattr, worker_thread, (void *)csock);
    }

}

void *worker_thread(void *args) {
    int sock = (int)args;
    char buff[255];
    char utente[50];
    int len;

    while(1) {
        /* Il worker thread riceve il comando */
        len = read(sock, buff, 255);
        if(strncmp("read", buff, 4) == 0) {
            pthread_mutex_lock(&mutex);
            snprintf(buff, 255, "%d %s: %s", ultimo_msg.id,
                ultimo_msg.utente, ultimo_msg.messaggio);
            pthread_mutex_unlock(&mutex);
            /* Invio risposta */
            write(sock, buff, strlen(buff)+1);
        } else if(strncmp("close", buff, 5) == 0) {
            close(sock);
            break;
        } else if(strncmp("send", buff, 4) == 0) {
            pthread_mutex_lock(&mutex);
            ultimo_msg.id++;
            strcpy(ultimo_msg.utente, utente);
            strncpy(ultimo_msg.messaggio, &buff[5], len-5);
            pthread_mutex_unlock(&mutex);
            sprintf(buff, "ok");
            /* Invio risposta */
            write(sock, buff, strlen(buff)+1);
        } else if(strncmp("register", buff, 8) == 0) {
            strncpy(utente, &buff[9], len-9);
            sprintf(buff, "Welcome %s!", utente);
            /* Invio risposta */
            write(sock, buff, strlen(buff)+1);
        }
    }
}