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
	struct sockaddr_in servaddr,plat;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(connect(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		printf("failed to connect with server\n");
	}

	bzero(buffer,100);
	bzero(&plat,sizeof(plat));
	int sp;
	recv(sfd,&sp,sizeof(int),0);
	printf("size %d\n",sp);
	recv(sfd,&plat,sp,0);
	
	close(sfd);
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(connect(sfd,(struct sockaddr*)&plat,sizeof(plat))<0)
	{
		printf("2failed to connect with server\n");
	}
		read(0,buffer,100);
		send(sfd,buffer,strlen(buffer),0);
		bzero(buffer,100);
		recv(sfd,buffer,100,0);
		write(1,buffer,100);
	close(sfd);
}

