#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#define STRDIM 50

struct dati {
    char stringa[STRDIM];
};

struct messaggio {
    long int mtype;
    struct dati info;
};

int main() {
    int msgid;
    struct messaggio msg;
    
    msgid = msgget((key_t)7777, 0666 | IPC_CREAT);
    /* Inserire controllo errore */
    
    while(1) {
        printf("P3 Attendo messaggio...\n");
        msgrcv(msgid, &msg, sizeof(struct dati), 2, 0);
        /* Inserire controllo errore */
        
        printf("Messaggio ricevuto: %s", msg.info.stringa);
        if(strncmp(msg.info.stringa, "esci", 4)==0) {
            break;
        }
    }
}