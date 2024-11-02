#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
int main() {
	int x, addr, len;
	int tcp_socket, csock;
	char buf[50];
	tcp_socket=socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_socket==-1) {
		printf("Errore socket\n");
		exit(1);
	}
	struct sockaddr_in addrserver;
	addrserver.sin_family=AF_INET;
	addrserver.sin_port=htons(9876);
	addrserver.sin_addr.s_addr=htonl(INADDR_ANY);
	x=bind(tcp_socket, (struct sockaddr*)&addrserver, sizeof(addrserver));
	if(x==-1) {
		printf("Errore bind\n");
		exit(1);
	}
	x=listen(tcp_socket, 5);
	if(x==-1) {
		printf("Errore listen\n");
		exit(1);
	}
	struct sockaddr_in addrclient;
	csock=accept(tcp_socket, NULL, NULL);
	printf("Richiesta avvenuta \n");
	//sleep(5);
	len=read(csock, buf, 50);
	if(len>0) {
		buf[len]='\0';
		printf("Messaggio ricevuto\n%s\n", buf);
	}
	close(tcp_socket);
}
