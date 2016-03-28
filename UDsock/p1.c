#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/un.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
#define offset(TYPE,MEM)  ( (int) &(( (TYPE* ) 0)->MEM))
int readline(int fd,char *buffer,int n)
{
	for(int i=0;i<n;i++)
	{
		if(read(fd,&buffer[i],1)>0)
		{
			if(buffer[i]=='\n')
			return i+1;
			
		}
		else
		{
			buffer[i]='\0';
			return i;
		}
	}
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
int recvfd(int usfd)
{
	char buf[10];
	struct iovec iov[1];
	struct msghdr msg;
	int status = -1;
	iov[0].iov_base = buf;
	iov[0].iov_len = sizeof(buf);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	struct cmsghdr cmsg;
	
	msg.msg_control=&cmsg;
	msg.msg_controllen=CMSG_LEN(sizeof(int));
	int res;
	if((res=recvmsg(usfd,&msg,0))>0)
	{
	int fd=*((int*)CMSG_DATA(&cmsg));
		printf("received fd:%d\n",fd);
	return fd;
	}
	else
	{
		printf("failed to receive%d\n",res);
	}
}
int main()
{
	char buffer[100];
	int fd=open("data.txt",O_RDONLY);
	printf("%d\n",fd);
	sockaddr_un myaddr,cliaddr;
	bzero(&myaddr,sizeof(myaddr));
	bzero(&cliaddr,sizeof(cliaddr));
	myaddr.sun_family=AF_UNIX;
	unlink("soc.socket");
	strcpy(myaddr.sun_path,"soc.socket");
	int usfd=socket(AF_UNIX,SOCK_STREAM,0);
	int len=sizeof(sa_family_t)+strlen(myaddr.sun_path);
	if(bind(usfd,(const struct sockaddr*)&myaddr,len)<0)
	{
		printf("bind failed\n");
	}
	listen(usfd,2);
	socklen_t clilen;
	int nsfd=accept(usfd,(struct sockaddr*)&cliaddr,&clilen);
	printf("%s %d\n",cliaddr.sun_path,nsfd);
	int r;
	while(1)
	{
		bzero(buffer,100);
		r=readline(fd,buffer,100);
		if(r==0)
		{
			sendfd(nsfd,fd);
			close(fd);
			break;
		}
		write(1,buffer,100);
		sleep(3);
		sendfd(nsfd,fd);
		close(fd);
		fd=recvfd(nsfd);
	}	
		while(1){}
}
