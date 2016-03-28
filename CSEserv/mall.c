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
int limit;
int n;

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
	
	int* d=(int*)arg;
	char* buffer=new char[100];
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(*d,&readset);
	select(10,&readset,NULL,NULL,NULL);
	int n=recv(*d,buffer,100,0);
	for(int i=0;i<n;i++)
	{
		buffer[i]=toupper(buffer[i]);
	}
	send(*d,buffer,strlen(buffer),0);
	close(*d);
}
void* fdreceiver(void* arg)
{
	int* usfd=(int*)arg;
	while(1)
	{
		int fd=recvfd(usfd);
		if(n<limit)
		{
			pthread_mutex_lock(&mut);
			n++;
			pthread_mutex_unlock(&mut);
			pthread_t t;
			pthread_create(&t,NULL,service,&fd);
		}
		else
		{
			if(id==3)
			kill(getppid(),SIGUSR1);
			else
			sendfd(usfd,fd);
		}
		
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
	if(listen(sfd,2)<0)
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
int main()
{
		int id=atoi(argv[1]);
		limit=atoi(argv[2]);
		int sfd=inet_socket(atoi(argv[3]));
		int usfd1,usfd2;
		if(id>1)
		 usfd1=unix_socket(argv[4]);//serv sock
		 
		 if(id==1)
		 {
		 usfd2=socket(AF_UNIX,SOCK_STREAM,0);
		 struct sockaddr_un servaddr;
		 servaddr.sun_family=AF_UNIX;
		 strcpy(servaddr.sun_path,argv[5]);
		 int len=sizeof(sa_familt_t)+strlen(servaddr.sun_path);
		 int re=connect(usfd2,(struct sockaddr*)&servaddr,len);
		 printf("res %d\n",re);
		 
		 }
		 
		 
		 nusfd=accept(usfd1,NULL,NULL);
		pthread_create(&t,NULL,fdreceiver,&usfd);

		while(1)
		{
			
			nsfd=accept(nsfd,NULL,NULL);
			n++;
			if(n>=limit)
			{
				sendfd(nusfd,nsfd);
				close(nsfd);
			}
			if(id==3 && n==limit)
			{
				kill(getppid(),SIGUSR1);
			}
			else
			{
				sendfd(usfd,nsfd);
			}
		}
}
