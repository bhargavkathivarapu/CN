#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<stdlib.h>
int main(int argc,char* argv[])
{
	char buffer[100];
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	connect(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

		bzero(buffer,100);
		read(0,buffer,100);
		send(sfd,buffer,strlen(buffer),0);
		recv(sfd,buffer,100,0);
		write(1,buffer,100);
	
}

