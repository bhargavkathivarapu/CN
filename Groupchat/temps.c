#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/select.h>
int main()
{
	char buffer[100];
	char path[4][100];
	int sfd[4];
	for(int i=0;i<4;i++)
	{
		bzero(path[i],100);
		strcpy(path[i],"./s1");
	}
	
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
	int pid;
	int nsfd;
	int pos=-1;
	struct timeval timer;
	timer.tv_sec=0;
	timer.tv_usec=0;
	while(1)
	{
		printf("Selecting\n");
		FD_ZERO(&readset);
		for(int i=0;i<4;i++)
		{
			if(sfd[i]!=-1)
			FD_SET(sfd[i],&readset);
		}
		select(maxfd+1,&readset,NULL,NULL,NULL);
		  pos=-1;
		 for(int i=0;i<4;i++)
		 {
		 	if(sfd[i]!=-1 && FD_ISSET(sfd[i],&readset))
		 	{
		 		pos=i;
		 		break;
		 	}	
		 }
		 printf("accepted %d\n",pos);
		 nsfd=accept(sfd[pos],(struct sockaddr*)&cliaddr,&clen);
	
		 pid=fork();
		 if(pid==0)
		 {
		 	bzero(buffer,100);
		 	for(int i=0;i<4;i++)
		 	{
		 		if(i!=pos)
		 		close(sfd[i]);
		 	}
		 	
		 	recv(nsfd,buffer,100,0);
		 	printf("%s\n",buffer);
			
			printf("about to fork\n");
			char tmp[24]={0x0};
			char tmp2[24]={0x0};
			sprintf(tmp,"%d",sfd[pos]);
			sprintf(tmp2,"%d",nsfd);
			printf("sfd %d nsfd %d\n",sfd[pos],nsfd);
		 	if(execlp(buffer,buffer,tmp,tmp2,(char*)0)<0)
		 	{
		 		printf("sdj\n");
		 	}
			
		}
		else
		{	
			close(sfd[pos]);
			sfd[pos]=-1;
			close(nsfd);
		}
	}
}

