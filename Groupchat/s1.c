#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
pthread_mutex_t mut;
int fd[10];
int tfd[10];
int allocate(int nsfd)
{
	for(int i=0;i<10;i++)
	{
		if(tfd[i]==-1)
		{
			tfd[i]=nsfd;
			break;
		}
	}
}
void* messenger(void* arg)
{
	char buffer[100],buf[100];
	int pos=-1;
	fd_set readset;
	int maxfd=-1;
	printf("name\n");
	while(1)
	{
		bzero(buffer,100);
		bzero(buffer,100);
		maxfd=-1;
		
		FD_ZERO(&readset);
		pthread_mutex_lock(&mut);
		for(int i=0;i<10;i++)
		{
			fd[i]=tfd[i];
			if(fd[i]==-1)
			continue;
			else
			{
				FD_SET(fd[i],&readset);
				if(maxfd<fd[i])
				{
					maxfd=fd[i];
				}
			}
		}
		
		pthread_mutex_unlock(&mut);
		printf("nam\n");
		
		select(maxfd+1,&readset,NULL,NULL,NULL);
		
		
		printf("namegot\n");

		for(int i=0;i<10;i++)
		{
			if(fd[i]!=-1 && FD_ISSET(fd[i],&readset))
			{
				recv(fd[i],buffer,100,0);
				//strcat(buffer,buf);
				pos=i;
				break;
			}
		
		}
		
		printf("%s\n",buffer);
		for(int i=0;i<10;i++)
		{
			if(fd[i]!=-1 && i!=pos)
			{
			int tt=send(fd[i],buffer,strlen(buffer),0);
			printf("%d\n",tt);
			}
		}
		
		
	}
	
}
int main(int argc,char** argv)
{
	pthread_mutex_init(&mut,NULL);
	pthread_t t;
	for(int i=0;i<10;i++)
	{
		tfd[i]=-1;
		fd[i]=-1;
	}
	int i=1;
	printf("1\n");
	int nsfd1=atoi(argv[2]);
	printf("2-%d\n",nsfd1);
	allocate(nsfd1);
	pthread_create(&t,NULL,messenger,NULL);
	
	printf("all clear\n");
	sockaddr_in cliaddr;
	socklen_t clen;
	while(1)
	{
		int nsfd=accept(atoi(argv[1]),(struct sockaddr*)&cliaddr,&clen);
		printf("accepted another\n");
		pthread_mutex_lock(&mut);
		allocate(nsfd);
		pthread_mutex_unlock(&mut);
		
	}
	
	
}
