#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/select.h>
#include<sys/socket.h>
typedef struct mem
{
	int nsfd;
}data;

void* service(void* arg)
{
	
	data* d=(data*)arg;
	char* buffer=new char[100];
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(d->nsfd,&readset);
	select(10,&readset,NULL,NULL,NULL);
	int n=recv(d->nsfd,buffer,100,0);
	for(int i=0;i<n;i++)
	{
		buffer[i]=toupper(buffer[i]);
	}
	send(d->nsfd,buffer,strlen(buffer),0);
	close(d->nsfd);
}

int main(int argc,char** argv)
{
	pthread_t t[10];
	int i=1;
	data tt;
	printf("1\n");
	char buff[100];
	int sfd=atoi(argv[1]);
	tt.nsfd=atoi(argv[2]);
	
	
	pthread_create(&t[0],NULL,service,&tt);
	
	printf("all clear\n");
	sockaddr_in cliaddr;
	socklen_t clen;
	while(1)
	{
		int nsfd=accept(sfd,(struct sockaddr*)&cliaddr,&clen);
		
		data td;
		td.nsfd=nsfd;
		printf("another accpeted %d\n",nsfd);
		pthread_create(&t[i++],NULL,service,&td);
	}
	
	
	
}
