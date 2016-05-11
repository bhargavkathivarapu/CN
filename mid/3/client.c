#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<stdlib.h>
#include<pthread.h>
typedef struct mem
{
	int id;
	int sfd;
}par;
void *readp(void* ar)
{
	char buffer[100];int n=0;
	par* t=(par*)ar;
	while(1)
	{
		//printf("write here:");
		//fflush(stdout);
		bzero(buffer,100);
		n=read(0,buffer,100);
		send(t->sfd,buffer,strlen(buffer),0);
		//usleep(1000);
	}
}
void *writep(void* ar)
{
	char buffer[100];int n=0;
	par* t=(par*)ar;
	while(1)
	{
		bzero(buffer,100);
		n=recv(t->sfd,buffer,100,0);
		write(1,buffer,n);
		
		//fflush(stdout);
		//usleep(1000);
	}
}
int main(int argc,char* argv[])
{
	char buffer[100];
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(connect(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		printf("failed to connect with server\n");
	}

	pthread_t reader,writer;
	par g;
	g.id=1;
	g.sfd=sfd;
	
	
	pthread_create(&reader,NULL,&readp,&g);
	pthread_create(&writer,NULL,&writep,&g);
	
	pthread_join(reader,NULL);
	pthread_join(writer,NULL);
	
}

