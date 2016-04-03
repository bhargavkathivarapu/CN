#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#include<signal.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<sys/un.h>

int ppid;
int nsfd;
int id;
typedef struct mem
{
int rsfd;
} data;

void handler(int sig)
{
	signal(SIGUSR1,handler);
	
	
	char buff[100];
	bzero(buff,100);
	recv(nsfd,buff,100,0);
	//printf("platform %d",id);
	write(1,buff,strlen(buff));
	send(nsfd,"received",9,0);
	close(nsfd);
	
	kill(ppid,SIGUSR1);
	
}
void* recvads(void* arg)
{
	//print ads to cable tv
	data* t=(data*)arg;
	char buffer[1000];
	struct ip* iph;
	while(1)
	{
		bzero(buffer,1000);
		recv(t->rsfd,&buffer,1000,0);
		iph=(struct ip*)buffer;
		//printf("platfrom %d:",id);
		write(1,buffer+(iph->ip_hl)*4,1000-(iph->ip_hl)*4);
	}
}
void* recvannoun(void* arg)
{
	//print ads from cable tv
	data* t=(data*)arg;
	char buffer[1000];
	struct ip* iph;
	while(1)
	{
		bzero(buffer,1000);
		recv(t->rsfd,&buffer,1000,0);
		iph=(struct ip*)buffer;
		//printf("platfrom %d:",id);
		write(1,buffer+(iph->ip_hl)*4,1000-(iph->ip_hl)*4);
	}
}

int raw_socket(int protocol)
{
	int rsfd=socket(AF_INET,SOCK_RAW,protocol);
	const int on=0;
	
	if(setsockopt(rsfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))<0)
	{
		printf("option setting failed\n");
	}
	return rsfd;
}
int unix_cli_socket(const char* sockname)
{
	struct sockaddr_un servuaddr;
	bzero(&servuaddr,sizeof(servuaddr));
	servuaddr.sun_family=AF_UNIX;
	strcpy(servuaddr.sun_path,sockname);
	int usfd=socket(AF_UNIX,SOCK_STREAM,0);
	int len=sizeof(sa_family_t)+strlen(servuaddr.sun_path);
	while(connect(usfd,(struct sockaddr*)&servuaddr,len)<0)
	{
		close(usfd);
		 usfd=socket(AF_UNIX,SOCK_STREAM,0);
		//printf("conn failed\n");
	}
	return usfd;
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
		//printf("failed to receive%d\n",res);
	}
}
int main(int argc,char* argv[])
{
	id=atoi(argv[1]);
	ppid=getppid();
	//initialize signal handler
	printf("%d\n",getpid());
	signal(SIGUSR1,handler);
	
	//for crossing into this platform
	int usfd=unix_cli_socket("soc.socket");
	printf("platform %d- usfd:%d\n",id,usfd);
	
	//install loud speaker on platform for announcements
	pthread_t t2;
	int rsfd2=raw_socket(143);
	printf("platform %d - arsfd:%d\n",id,rsfd2);
	data g2;
	g2.rsfd=rsfd2;
	pthread_create(&t2,NULL,recvannoun,&g2);
	
	//install ad receiver from cable tv
	pthread_t t1;
	int rsfd1=raw_socket(133);
	printf("platform %d - rsfd:%d\n",id,rsfd1);
	data g1;
	g1.rsfd=rsfd1;
	pthread_create(&t1,NULL,recvads,&g1);
	
	//wait for trains to come	
	while(usfd>0)
	{
		nsfd=recvfd(usfd);
		//if a train arrived service it
		if(nsfd!=-1)
		raise(SIGUSR1);
	}
	
	pthread_join(t1,NULL);
	
	return 0;
}
