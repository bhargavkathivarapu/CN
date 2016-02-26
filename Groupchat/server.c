#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/select.h>
#include<signal.h>
#include<fcntl.h>
#include<stdlib.h>
#include<wait.h>
int main()
{
	int sfd;
	int p=32020;
	struct sockaddr_in myaddr;

		myaddr.sin_family=AF_INET;
		myaddr.sin_port=htons(p);
		myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		sfd=socket(AF_INET,SOCK_STREAM,0);
		
		int er=bind(sfd,(const struct sockaddr*)&myaddr,sizeof(myaddr));
		if(er==-1)
		{	
			printf("Bind failed\n");
			exit(0);
		}
		listen(sfd,5);
	
	
	sockaddr_in cliaddr;
	socklen_t clen;
	int pid;
	int nsfd;

	int flag=0;
	while(1)
	{
		printf("accepting\n");
		 nsfd=accept(sfd,(struct sockaddr*)&cliaddr,&clen);
		pid=fork();
		if(pid==0)
		{
			printf("about to fork\n");
			char tmp[24]={0x0};
			char tmp2[24]={0x0};
			sprintf(tmp,"%d",sfd);
			sprintf(tmp2,"%d",nsfd);
			printf("sfd %d nsfd %d\n",sfd,nsfd);
		 	if(execlp("./s1","./s1",tmp,tmp2,(char*)0)<0)
		 	{
		 		printf("sdj\n");
		 	}
		}
		else
		{
			wait(NULL);
			close(nsfd);
		}
		
	}
}

