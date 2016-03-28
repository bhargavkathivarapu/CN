#include<stdio.h>
#include<ctype.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<stdlib.h>
#include<signal.h>
#include<pthread.h>
#include<sys/un.h>
int limit;
int n;
pthread_mutex_t mut;
typedef struct mem
{
int nsfd;
} data;
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
void* service(void* arg)
{
	
	data* d=(data*)arg;
	char* buffer=new char[100];
	int nl=recv(d->nsfd,buffer,100,0);
	for(int i=0;i<nl;i++)
	{
		buffer[i]=toupper(buffer[i]);
	}
	send(d->nsfd,buffer,strlen(buffer),0);
	close(d->nsfd);
	pthread_mutex_lock(&mut);
	n--;
	pthread_mutex_unlock(&mut);
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
	
	pthread_mutex_init(&mut,NULL);
	 n=0;limit=1;
	 int usfd;
	 usfd=socket(AF_UNIX,SOCK_STREAM,0);
	 struct sockaddr_un servaddr;
	 servaddr.sun_family=AF_UNIX;
	 strcpy(servaddr.sun_path,"soc2");
	 int len=sizeof(sa_family_t)+strlen(servaddr.sun_path);
	 int re=-1;
	
	 
	
	
	int sfd=inet_socket(atoi(argv[1]));
	
	re=connect(usfd,(struct sockaddr*)&servaddr,len);
	 printf("res1 %d\n",re);
	 if(re==-1)
	 {
	 	re=connect(usfd,(struct sockaddr*)&servaddr,len);
	 printf("res1 %d\n",re);
	 	
	 }
		while(1)
		{
			
			int nsfd=accept(sfd,NULL,NULL);
			pthread_mutex_lock(&mut);
			n++;
			pthread_mutex_unlock(&mut);
			printf("one is entring mall1\n");
			if(n>limit)
			{
				printf("m1 full\n");
				sendfd(usfd,nsfd);
				close(nsfd);
			}
			else
			{
				data td;
				td.nsfd=nsfd;
				pthread_t t;
				pthread_create(&t,NULL,&service,&td);
			}
		}
}
