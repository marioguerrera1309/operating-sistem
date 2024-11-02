#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/msg.h>
struct memoria {
	int num[3];
	int punti;
};
struct msgbuf {
	long mtype;
	struct memoria mem;
};
void funzione(int msqid) {
	int tmp[3];
	int x, y;
	srand(getpid());
	struct msgbuf mes;
	for(int i=0;i<10;i++) {
		for(int j=0;j<3;j++) {
			tmp[j]=(rand()%6)+1;
		}
		x=tmp[0];
		y=0;
		for(int j=0;j<3;j++) {
			for(int k=0;k<3;k++) {
				if(tmp[k]>x) {
					x=tmp[k];
					y=k;
				}
			}
			mes.mem.num[j]=x;
			tmp[y]=0;
		}
		mes.mtype=1;
		msgsnd(msqid, (void*)&mes, sizeof(struct memoria), 0);
		msgrcv(msqid, (void*)&mes, sizeof(struct memoria), msqid, 0);
		printf("Punteggio totalizzato da %d : %d\n", msqid, mes.mem.punti);
	}
}
int main() {
	int msqid1, msqid2, x;
	pid_t figlio1, figlio2;
	struct msgbuf mes1, mes2; 
	int p1=0, p2=0;
	msqid1=msgget(IPC_PRIVATE, IPC_CREAT | 0666);
	msqid2=msgget(IPC_PRIVATE, IPC_CREAT | 0666);
	figlio1=fork();
	switch(figlio1) {
		case 0:
			funzione(msqid1);
			exit(0);
		default:	
			break;
	}
	figlio2=fork();
	switch(figlio2) {
		case 0:
			funzione(msqid2);
			exit(0);
		default:	
			break;
	}
	for(int i=0;i<10;i++) {
		msgrcv(msqid1, (void*)&mes1, sizeof(struct memoria), 1, 0);
		msgrcv(msqid2, (void*)&mes2, sizeof(struct memoria), 1, 0);
		for(int j=0;j<3;j++) {
			if(mes1.mem.num[j]>mes2.mem.num[j]) {
				p1++;
			} else if(mes1.mem.num[j]<mes2.mem.num[j]) {
				p2++;
			}
		}
		mes1.mem.punti=p1;
		mes1.mtype=msqid1;
		msgsnd(msqid1, (void*)&mes1, sizeof(struct memoria), 0);
		mes2.mem.punti=p2;
		mes2.mtype=msqid2;
		msgsnd(msqid2, (void*)&mes2, sizeof(struct memoria), 0);
	}
	wait(0);
	wait(0);
	msgctl(msqid1, IPC_RMID, 0);
	msgctl(msqid2, IPC_RMID, 0);
}
