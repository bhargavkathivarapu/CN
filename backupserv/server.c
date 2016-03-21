#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<signal.h>
#include<sys/un.h>
#include<pthread.h>
#include<ctype.h>
typedef struct mem
{
int nsfd;
} data;
int bspid;
void handler(int sig)
{
	printf("sig received\n");
	kill(bspid,SIGUSR1);
	
	exit(0);
	printf("con\n");
}
int sendfd(int usfd, int fd)
{
	struct iovec iov[1];
	struct msghdr msg;
	char buf[2];
	iov[0].iov_base = buf;
	iov[0].iov_len = 2;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	struct cmsghdr cmsg;
	cmsg.cmsg_level=SOL_SOCKET;
	cmsg.cmsg_type=SCM_RIGHTS;
	cmsg.cmsg_len=CMSG_LEN(sizeof(int));
	
	
	msg.msg_control=&cmsg;
	msg.msg_controllen=CMSG_LEN(sizeof(int));
	*((int*)CMSG_DATA(&cmsg))=fd;
	buf[1] = 0;
	buf[0] = 0;
	printf("%d\n",*((int*)CMSG_DATA(&cmsg))); 
	if(sendmsg(usfd,&msg,0)<0)
	{
		//printf("failed to send fd\n");
	}
	else
	{
		printf("fd sent\n");
	}
	
}
void* service(void* arg)
{
	
	data* d=(data*)arg;
	char* buffer=new char[100];
	while(1)
	{
	int n=recv(d->nsfd,buffer,100,0);
	for(int i=0;i<n;i++)
	{
		buffer[i]=toupper(buffer[i]);
	}
	send(d->nsfd,buffer,strlen(buffer),0);
	}
	close(d->nsfd);
}
int unix_socket(const char* sockname)
{
	sockaddr_un myuaddr;
	bzero(&myuaddr,sizeof(myuaddr));
	myuaddr.sun_family=AF_UNIX;
	strcpy(myuaddr.sun_path,sockname);
	unlink(sockname);
	int usfd=socket(AF_UNIX,SOCK_STREAM,0);
	int len=sizeof(sa_family_t)+strlen(myuaddr.sun_path);
	if(bind(usfd,(const struct sockaddr*)&myuaddr,len)<0)
	{
		printf("bind failed\n");
	}
	
	if(listen(usfd,2)<0)
	{
		printf("listen failed\n");
	}
	return usfd;
}
int inet_socket(int port)
{
	struct sockaddr_in myaddr;
	int sfd;
	myaddr.sin_family=AF_INET;
	myaddr.sin_port=htons(port);
	myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	sfd=socket(AF_INET,SOCK_STREAM,0);
	
	int er=bind(sfd,(const struct sockaddr*)&myaddr,sizeof(myaddr));
	if(er==-1)
	{	
		printf("Bind failed\n");
		exit(0);
	}
	if(listen(sfd,5)<0)
	{
		printf("listen failed\n");
		exit(0);
	}
	return sfd;
}
int main(int argc,char* argv[])
{
	char buffer[100];
	
	bspid=fork();
	if(bspid==0)
	{
		execlp("./bs","./bs",(char*)0);
	}
	else
	{
		pthread_t t[10];
		signal(SIGUSR1,&handler);
		int usfd=unix_socket("soc.socket");
		 printf("hi\n");
		int nusfd=accept(usfd,NULL,NULL);
		
		int i=0;
		int sfd=inet_socket(atoi(argv[1]));
		 printf("gi%d\n",sfd);
		sockaddr_in cliaddr;
		socklen_t clen;
		int pid;
		sendfd(nusfd,sfd);
		while(1)
		{
			
			int  nsfd=accept(sfd,NULL,NULL);
			printf("nsfd:%d\n",nsfd);
			sendfd(nusfd,nsfd);
			data td;
			td.nsfd=nsfd;
			pthread_create(&t[i++],NULL,service,&td);
			
		}
	}
}

