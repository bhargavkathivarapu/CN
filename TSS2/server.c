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
#include<sys/un.h>
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
int unix_socket(char* sockname)
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
	}
	return sfd;
}
int main(int argc,char* argv[])
{

	int usfd=unix_socket("soc.socket");
	 
	int sfd=inet_socket(atoi(argv[1]));
	
	sockaddr_in cliaddr;
	sockaddr_un cliuaddr;
	socklen_t clen,cen;
	int pid;
	int nsfd,nusfd;
	
	int flag=1;
	
	while(1)
	{
		//sleep(2);
		printf("accepting\n");
		 nsfd=accept(sfd,NULL,NULL);
		
		if(flag==1)
		{
			flag=0;
			pid=fork();
			if(pid==0)
			{
				close(sfd);
				close(usfd);
				execlp("./s1","./s1",(char*)0);
			}
			else
			{
				 nusfd=accept(usfd,NULL,NULL);
	 			printf("nusfd%d\n",nusfd);
	 			
				sendfd(nusfd,nsfd);
				close(nsfd);
			}

		}
		else
		{
			
			sendfd(nusfd,nsfd);
			close(nsfd);
		}
		
	}
	
}

