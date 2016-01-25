#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<poll.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>



char buf[50][100];
int m=0;int ofd;

void handler(int sig)
{
	//reestablish signal handler
	signal(SIGUSR1,handler);
	printf("signal received\n");
	fflush(stdout);
	//write stored info to P5/out on receiving signal
	for(int i=0;i<m;i++)
	{
		write(ofd,buf[i],strlen(buf[i]));
	}
	m=0;
	
}
int main(int c,char* argv[])
{
	//create a pipe 
	
	
	int pfd[2];
	pipe(pfd);
	char buffer[100];int j=0;
	
	
	int pid=fork();
	
	if(pid==0)
	{
		close(pfd[0]);
		char buf[100]="hello world!\n";
		//write to pipe something .....
		write(pfd[1],buf,strlen(buf));
		exit(0);
	}
	else
	{
		close(pfd[1]);
		mkfifo(argv[1],0666);
		mkfifo(argv[2],0666);
		mkfifo(argv[3],0666);
		printf("done\n");
		//install signal handler
		signal(SIGUSR1,handler);
		//send pid of process to signaler(P4)
		int sfd=open(argv[2],O_RDWR);
		FILE* pp=fdopen(sfd,"w");
		int a=getpid();
		 fprintf(pp,"%d",a);
		fclose(pp);
		printf("done2\n");
		//open popular fifo with P2
		int ffd=open(argv[1],O_RDWR);
		printf("done3\n");
		//open popular fifo with P5
		 ofd=open(argv[3],O_RDWR);
		//open pofd with P3
		printf("done4\n");
		FILE* fp=popen("./P3","r");
		int pofd=fileno(fp);

		printf("done5\n");
		//initial pollfd structure
		struct pollfd fds[3];
		fds[0].fd=0;
		fds[0].events=POLLIN;
		fds[1].fd=pfd[0];
		fds[1].events=POLLIN;
		fds[2].fd=ffd;
		fds[2].events=POLLIN;
		fds[3].fd=pofd;
		fds[3].events=POLLIN;
	

		//repeat forever
		while(1)
		{
			//printf("gi\n");
			bzero(buffer,100);
			//start polling the fd
			while(poll(fds,3,0)<=0){ }
			//read from an fd which is ready
			if(fds[0].revents & POLLIN)
			read(fds[0].fd,buffer,100);
			else if(fds[1].revents  & POLLIN)
			read(fds[1].fd,buffer,100);
			else if(fds[2].revents & POLLIN)
			read(fds[2].fd,buffer,100);
			else if(fds[3].revents & POLLIN)
			read(fds[3].fd,buffer,100);
			
			if(strcmp(buffer,"")!=0)
			{
				printf("g%s\n",buffer);
			strcpy(buf[m++],buffer);
			}
		}
		
	}
	
}
