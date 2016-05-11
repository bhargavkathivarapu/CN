#include<stdio.h>
#include<stdlib.h>
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
#include<sys/stat.h>
#include<sys/types.h>


int table[50][3];
int ptr=0;
int count=0;
int pipepid;
void handler2(int sig)
{
	
	signal(SIGCHLD,handler2);
	int pid=wait(NULL);
	if(pid!=pipepid)
	{
	printf("PID :%d %dended\n",pid,pipepid);
	for(int i=0;i<ptr;i++)
	{
		if(table[i][2]==pid)
		{
			table[i][2]=-1;
			table[i][1]=1;
			break;
		}
	}
	}

}
void handler(int sig)
{
	//reestablish signal handler
	signal(SIGUSR1,handler);
	printf("signal received\n");
	fflush(stdout);
	char buf[10];
	sprintf(buf,"%d",table[ptr][0]);
	int pid=fork();
	if(pid==0)
	{	
	
			dup2(table[ptr][0],0);
			dup2(table[ptr][0],1);
			execlp("./s1","./s1",buf,(char*)0);

	}
	else
	{
		table[ptr][1]=0;
		table[ptr][2]=pid;
		ptr++;
	}
	
}
int main(int c,char* argv[])
{
	//create a pipe 
	
	signal(SIGUSR1,handler);
		signal(SIGCHLD,handler2);
	int pfd[2];
	pipe(pfd);
	char buffer[100];int j=0;
	
	
	 pipepid=fork();
	
	if(pipepid==0)
	{
		close(pfd[0]);
		char buf[100]="hello world!\n";
		//write to pipe something .....
		write(pfd[1],buf,strlen(buf));
		exit(0);
	}
	else
	{
		printf("pip:%d\n",pipepid);
		close(pfd[1]);
		mkfifo(argv[1],0666);
		mkfifo(argv[2],0666);
		printf("done\n");
		//install signal handler
		
		//send pid of process to signaler(P4)
		int sifd=open(argv[2],O_RDWR);
		FILE* pp=fdopen(sifd,"w");
		int a=getpid();
		 fprintf(pp,"%d",a);
		fclose(pp);
		printf("done2\n");
		//open popular fifo with P2
		int ffd=open(argv[1],O_RDWR);
		printf("done3\n");
		
		//open pofd with P3
		printf("done4\n");
		FILE* fp=popen("./P3","r");
		int pofd=fileno(fp);
		
		struct sockaddr_in myaddr;
		
		myaddr.sin_family=AF_INET;
		myaddr.sin_port=htons(32020);
		myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		int sfd=socket(AF_INET,SOCK_STREAM,0);
		
		int er=bind(sfd,(const struct sockaddr*)&myaddr,sizeof(myaddr));
		if(er==-1)
		{	
			printf("Bind failed\n");
			exit(0);
		}
		listen(sfd,5);
	
	
		sockaddr_in cliaddr;
		socklen_t clen;
		printf("done5\n");
		int fd[5];
		//initial pollfd structure
		fd[0]=0;
		fd[1]=pfd[0];
		fd[3]=ffd;
		fd[2]=pofd;
		fd[4]=sfd;
		
		fd_set readset;
		int pos=-1;
		int maxfd=-1;
		//repeat forever
		while(1)
		{
			//printf("gi\n");
			bzero(buffer,100);
			FD_ZERO(&readset);
			for(int i=3;i<5;i++)
			{
				FD_SET(fd[i],&readset);
				if(fd[i]>maxfd)
				{
					maxfd=fd[i];
				}
			}
			//start polling the fd
			printf("polling\n");
			int res=select(maxfd+1,&readset,NULL,NULL,NULL);
			if(res>0)
			{
			pos=-1;
			//read from an fd which is ready
			for(int i=3;i<5;i++)
			{
				if(FD_ISSET(fd[i],&readset))
				{
					pos=i;
	
				}
			}
			if(pos==4)
			{
				table[count][0]=accept(fd[pos],(struct sockaddr*)&cliaddr,&clen);
				table[count][1]=1;
				table[count][2]=-1;
				count++;
			}
			else if(pos!=-1)
			{
			
				read(fd[pos],buffer,100);
				if(strcmp(buffer,"")!=0)
				{
				printf("g%d %s\n",count,buffer);
				for(int i=0;i<count;i++)
				{
					if(table[i][1]==1)
					send(table[i][0],buffer,100,0);
				}
				}
			}
		}
		}
		
	}
	
}
