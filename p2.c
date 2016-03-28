#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
#define offset(TYPE,MEM)  ( (int) & ( (TYPE *) 0 ) -> MEM)
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
	char buffer[100];int fd;
	sockaddr_un servaddr,myaddr;
	bzero(&myaddr,sizeof(myaddr));
	myaddr.sun_family=AF_UNIX;
	unlink("soc2.socket");
	strcpy(myaddr.sun_path,"soc2.socket");
	int usfd=socket(AF_UNIX,SOCK_STREAM,0);
	int len=sizeof(sa_family_t)+1+strlen(myaddr.sun_path);
	if(bind(usfd,(struct sockaddr*)&myaddr,len)<0)
	{
		printf("bind failed\n");
	}
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sun_family=AF_UNIX;
	strcpy(servaddr.sun_path,"soc.socket");
	len=sizeof(sa_family_t)+1+strlen(servaddr.sun_path);
	int r;
	if(connect(usfd,(struct sockaddr*)&servaddr,len)>=0)
	{
		while(1)
		{
		bzero(buffer,100);
		fd=recvfd(usfd);
		 r=readline(fd,buffer,100);
		 if(r==0)
		{
			sendfd(usfd,fd);
			close(fd);
			break;
		}
		write(1,buffer,100);
		sleep(3);
		sendfd(usfd,fd);
		close(fd);
		}
	}
	else
	{
		printf("connect failed\n");
	}	
		
}
