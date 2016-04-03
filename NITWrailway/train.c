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
	//destination is NITW railway junction
	struct sockaddr_in servaddr,plat;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	//reached NITW railway junction
	if(connect(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		printf("failed to connect with server\n");
	}

	bzero(buffer,100);
	//servicing at a given platform
	read(0,buffer,100);
	send(sfd,buffer,strlen(buffer),0);
	bzero(buffer,100);
	recv(sfd,buffer,100,0);
	write(1,buffer,100);
	close(sfd);
}

