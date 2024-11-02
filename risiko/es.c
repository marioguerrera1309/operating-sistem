#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/msg.h>
#include<sys/ipc.h>
struct memoria {
	int num[3];
	int punti;
};
struct msgbuf {
	long mtype;
	struct memoria mem;
};
void funzione(int msgid) {
	int num[3], tmp[3];
	struct msgbuf mes;
	int x, y;
	srand(getpid());
	for(int k=0;k<10;k++) {
		for(int i=0;i<3;i++) {
			tmp[i]=(rand()%6)+1;
		}
		for(int j=0;j<3;j++) {
			x=tmp[0];
			y=0;
			for(int i=0;i<3;i++) {
				if(tmp[i]>x) {
					x=tmp[i];
					y=i;
				}
			}
			num[j]=x;
			tmp[y]=0;
		}
		//printf("%d:num[0]:%d num[1]:%d num[2]:%d\n", msgid, num[0], num[1], num[2]);
		mes.mtype=1;
		for(int i=0;i<3;i++) {
			mes.mem.num[i]=num[i];
		}
		msgsnd(msgid, &mes, sizeof(struct memoria), 0);
		msgrcv(msgid, &mes, sizeof(struct memoria), msgid, 0);
		printf("Punteggio totalizzato da %d: %d\n", msgid, mes.mem.punti);
	}
	exit(0);
}
int main() {
	struct msgbuf mes1, mes2;
	int msgid1, msgid2;
	int x, punti1=0, punti2=0;
	pid_t p1, p2;
	msgid1=msgget(IPC_PRIVATE, IPC_CREAT | 0666);
	msgid2=msgget(IPC_PRIVATE, IPC_CREAT | 0666);
	p1=fork();
	switch(p1) {
		case 0:
			funzione(msgid1);
		default:
			break;
	}
	p2=fork();
	switch(p2) {
		case 0:
			funzione(msgid2);
		default:
			break;
	}
	for(int i=0;i<10;i++) {
		msgrcv(msgid1, &mes1, sizeof(struct memoria), 1, 0);
		msgrcv(msgid2, &mes2, sizeof(struct memoria), 1, 0);
		for(int j=0;j<3;j++) {
			if(mes1.mem.num[j]>mes2.mem.num[j]) {
				punti1++;				
			} else if(mes1.mem.num[j]<mes2.mem.num[j]) {
				punti2++;
			}
		}
		mes1.mem.punti=punti1;
		mes1.mtype=msgid1;
		mes2.mem.punti=punti2;
		mes2.mtype=msgid2;
		msgsnd(msgid1, &mes1, sizeof(struct memoria), 0);
		msgsnd(msgid2, &mes2, sizeof(struct memoria), 0);
		sleep(1);
	}
	wait(0);
	wait(0);
	msgctl(msgid1, IPC_RMID, 0);
	msgctl(msgid2, IPC_RMID, 0);
	exit(0);
}
