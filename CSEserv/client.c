#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<stdlib.h>
typedef struct tt
{
	int num;
	int port;
} ticket;
int main(int argc,char* argv[])
{
	char buffer[100];
	struct sockaddr_in servaddr,malladdr;
	servaddr.sin_family=AF_INET;
	malladdr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	malladdr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	connect(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

		bzero(buffer,100);
		read(0,buffer,100);
		send(sfd,buffer,strlen(buffer),0);
		bzero(buffer,100);
		ticket t1;
		recv(sfd,&t1,sizeof(t1),0);
		if(t1.num>= 0)
		{
			printf("tic num:%d\n",t1.num);
			close(sfd);
			int sfd2=socket(AF_INET,SOCK_STREAM,0);
			malladdr.sin_port=htons(t1.port);
			connect(sfd2,(struct sockaddr*)&malladdr,sizeof(malladdr));
			
			bzero(buffer,100);
			read(0,buffer,100);
			send(sfd2,buffer,strlen(buffer),0);
		
			recv(sfd2,buffer,100,0);
			write(1,buffer,100);
		}
		else
		{
			write(1,"malls full",11);
		}	
}

