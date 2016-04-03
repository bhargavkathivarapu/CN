#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/un.h>


//array of child pids
int pids[3];
//status of child free or not free(1 or 0)
int idle[3];
int psfd[3]={1,1,1};
int nusfd[3];
//update free based on pid of sending process
void freeservice(long cpid)
{
	for(int i=0;i<3;i++)
	{
		if(pids[i]==cpid)
		{
			idle[i]=1;
			
			return;
		}
	}
	
}
int getfree()
{
	for(int i=0;i<3;i++)
	{
		if(idle[i]==1)
		{
			return i;
		}
	}
}
 void handler(int sig, siginfo_t *siginfo, void *context)
{
	printf ("Sending PID: %ld, UID: %ld\n",
			(long)siginfo->si_pid, (long)siginfo->si_uid);
			
			freeservice((long)siginfo->si_pid);
	
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
int inet_serv_socket(int port)
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
int main(int argc,char* argv[])
{
	
	char buffer[100];
	int n=0;
	
	struct sigaction s;
    s.sa_sigaction = &handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags =SA_SIGINFO;
    sigaction (SIGUSR1, &s, NULL);
    
	
	
	//create platforms
	for(int i=0;i<3;i++)
	{
		pids[i]=fork();
		if(pids[i]==0)
		{
			char buf[24];
			sprintf(buf,"%d",32004+i);
			execlp("./pf","./pf",buf,(char*)NULL);
			
		}
	}
	
	//initially each platform is free
	for(int i=0;i<3;i++)
	{
	idle[i]=1;
	}
	
	int rsfd=raw_socket(143);
	printf("SM - rsfd:%d\n",rsfd);
	
	int sfd[3];

	//accept unix domain socket conn from platforms
	int usfd=unix_socket("soc.socket");
	for(int i=0;i<3;i++)
	{
	nusfd[i]=accept(usfd,NULL,NULL);
	printf("nusfd %d : %d\n",i,nusfd[i]);
	}
	
	//tracks for different directions are laid
	int p[3]={32001,32002,32003};
	int maxfd=-1;
	struct sockaddr_in myaddr[3];
	fd_set readset;
	FD_ZERO(&readset);
	for(int i=0;i<3;i++)
	{
		sfd[i]=inet_serv_socket(p[i]);
		if(maxfd<sfd[i])
		{
			maxfd=sfd[i];
		}
	}
	
	sockaddr_in cliaddr;
	socklen_t clen;
	int pid;
	int nsfd;
	int pos=-1;
	
	
	
	while(1)
	{
		
		//poll on tracks from differnt directions for incoming trains
		FD_ZERO(&readset);
		for(int i=0;i<3;i++)
		{
			if(sfd[i]!=-1)
			FD_SET(sfd[i],&readset);
		}
		select(maxfd+1,&readset,NULL,NULL,NULL);
		  pos=-1;
		  for(int i=0;i<3;i++)
		 {
		 	if(FD_ISSET(sfd[i],&readset))
		 	{
		 		pos=i;
		 		break;
		 	}	
		 }
		
		
	printf("all running\n");
	
	//TODO:pass annoncements to all platforms
	
	//get free platform
	int r=getfree();
	
	
	if(r>=0)
	{
		
		 int nsfd=accept(sfd[pos],NULL,NULL);
		 
		 //send announcements to all platforms
		 char buff[1000];char tmp[20];
		struct sockaddr_in servaddr;
		servaddr.sin_family=AF_INET;
		servaddr.sin_port=htons(0);
		servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

		bzero(buff,1000);
		bzero(tmp,20);
		sprintf(tmp,"%d\n",r);
		strcpy(buff,"a train arriving at platform ");
		strcat(buff,tmp);
		sendto(rsfd,buff,1000,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
		 //mark platform selected busy
		 idle[r]=0;
		 //pass fd to selected platform
		 sendfd(nusfd[r],nsfd);
		 close(nsfd);
	
	}
	
	
	}
	
	return 0;
}
