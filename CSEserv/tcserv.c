#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<stdlib.h>
#include<signal.h>
typedef struct tt
{
	int num;
	int port;
} ticket;

void handler(int sig)
{
	signal(SIGUSR1,&handler);
	printf("sorry no tickets\n");
	sleep(10);
}

int main()
{
	char buffer[100];
	int pid[3];
	int sfd[4];
	int mallport[3]={32005,32006,32007};
	for(int i=0;i<3;i++)
	{
		pid[i]=fork();
		if(pid[i]==0)
		{
			if(i==0)
			execlp("./m1","./m1","32005",(char*)0);
			else if(i==1)
			execlp("./m2","./m2","32006",(char*)0);
			else if(i==2)
			execlp("./m3","./m3","32007",(char*)0);
		}
	
	}
	signal(SIGUSR1,&handler);
	int p[4]={32001,32002,32003,32004};
	int maxfd=-1;
	struct sockaddr_in myaddr[4];
	fd_set readset;
	FD_ZERO(&readset);
	for(int i=0;i<4;i++)
	{
		myaddr[i].sin_family=AF_INET;
		myaddr[i].sin_port=htons(p[i]);
		myaddr[i].sin_addr.s_addr=htonl(INADDR_ANY);
		sfd[i]=socket(AF_INET,SOCK_STREAM,0);
		if(maxfd<sfd[i])
		{
			maxfd=sfd[i];
		}
		int er=bind(sfd[i],(const struct sockaddr*)&myaddr[i],sizeof(myaddr[i]));
		if(er==-1)printf("Bind failed\n");
		listen(sfd[i],1);
	}
	
	sockaddr_in cliaddr;
	socklen_t clen;
	int nsfd;
	int pos=-1;

	struct timeval timer;
	timer.tv_sec=0;
	timer.tv_usec=0;
	int n=0;
	while(1)
	{
		bzero(buffer,100);
		maxfd=-1;
		printf("Selecting\n");
		FD_ZERO(&readset);
		for(int i=0;i<4;i++)
		{
			FD_SET(sfd[i],&readset);
			if(maxfd<sfd[i])
			maxfd=sfd[i];
		}
		int res=select(maxfd+1,&readset,NULL,NULL,NULL);
		if(res>0)
		{
		  pos=-1;
		 for(int i=0;i<4;i++)
		 {
		 	if(FD_ISSET(sfd[i],&readset))
		 	{
		 		pos=i;
		 		break;
		 	}	
		 }
		 
		 printf("accepted %d\n",pos);
		 nsfd=accept(sfd[pos],(struct sockaddr*)&cliaddr,&clen);
	
		 recv(nsfd,buffer,100,0);
		 ticket t1;
		 t1.num=n++;
		 t1.port=atoi(buffer)+32004;
		 send(nsfd,&t1,sizeof(t1),0);
		 printf("ticket %d to port %d\n",t1.num,t1.port);
		 
		close(nsfd);
		}
	}
}

