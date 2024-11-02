#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<string.h>
int main() {
	int tcp_socket, csock, x;
	char buf[50];
	tcp_socket=socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_socket==-1) {
		printf("Errore socket\n");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9876);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	csock=connect(tcp_socket, (struct sockaddr*)&addr, sizeof(addr));
	sprintf(buf, "Ciao");
	x=write(tcp_socket, buf, strlen(buf));
	if(x>0) {
		printf("Messaggio inviato\n");
	}
	close(tcp_socket);
}
