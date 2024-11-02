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
    int turno = 0;
    struct messaggio msg;
    
    msgid = msgget((key_t)7777, 0666 | IPC_CREAT);
    /* Inserire controllo errore */
    
    while(1) {
        msg.mtype = (turno % 3) + 1;
        printf("Inserisci stringa: ");
        fgets(msg.info.stringa, STRDIM, stdin);
        
        if(strncmp(msg.info.stringa, "esci", 4) == 0) {
            msg.mtype = 1;
            msgsnd(msgid, &msg, sizeof(struct dati), 0);
            /* Controllo errore */
            msg.mtype = 2;
            msgsnd(msgid, &msg, sizeof(struct dati), 0);
            /* Controllo errore */
            msg.mtype = 3;
            msgsnd(msgid, &msg, sizeof(struct dati), 0);
            /* Controllo errore */
            break;
        }
        
        msgsnd(msgid, &msg, sizeof(struct dati), 0);
        turno++;
    }
    
    msgctl(msgid, IPC_RMID, 0);
}