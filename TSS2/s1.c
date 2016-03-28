#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<signal.h>
#include<poll.h>
#include<stdlib.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<sys/un.h>
typedef struct mem
{
	int nsfd;
}data;
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
void* service(void* arg)
{
	
	data* d=(data*)arg;
	char* buffer=new char[100];
	struct pollfd pfd;
	bzero(&pfd,sizeof(pfd));
	pfd.fd=d->nsfd;
	pfd.events=POLLIN;
	pfd.revents=0;
	int re=poll(&pfd,1,-1);
	if(pfd.revents & POLLIN)	
	printf("poll %d %d\n",d->nsfd,re);
	int n=recv(d->nsfd,buffer,100,0);
	printf("rev %d %d\n",d->nsfd,n);
	for(int i=0;i<n;i++)
	{
		buffer[i]=toupper(buffer[i]);
	}
	send(d->nsfd,buffer,strlen(buffer),0);
	close(d->nsfd);
}

int main(int argc,char** argv)
{
	pthread_t t[10];int i=0;
	
	sockaddr_un myaddr,servaddr;
	bzero(&servaddr,sizeof(servaddr));
	bzero(&myaddr,sizeof(myaddr));
	myaddr.sun_family=AF_UNIX;
	strcpy(myaddr.sun_path,"soc2.socket");
	unlink("soc2.socket");
	servaddr.sun_family=AF_UNIX;
	strcpy(servaddr.sun_path,"soc.socket");
	int usfd=socket(AF_UNIX,SOCK_STREAM,0);
	socklen_t len=sizeof(sa_family_t)+strlen(myaddr.sun_path);
	bind(usfd,(const struct sockaddr*)&myaddr,len);
	 len=sizeof(sa_family_t)+strlen(servaddr.sun_path);
	 int re=-1;
	 while(re<0)
	 {
	 re=connect(usfd,(struct sockaddr*)&servaddr,len);
	printf("connect%d\n",re);
		sleep(1);
	}
	
	while(1)
	{
		int nsfd=recvfd(usfd);
		/****5*/
		data* td=new data();
		td->nsfd=nsfd;
		printf("another accpeted %d\n",nsfd);
		pthread_create(&t[i++],NULL,&service,td);
	}
	
	
	
	
	
}
