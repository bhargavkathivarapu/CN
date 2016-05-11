#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc,char* argv[])
{
	char buffer[100];char c;
	//open fifo with alls
	printf("hi\n");
	mkfifo(argv[1],0666);
	int ffd=open(argv[1],O_RDWR);
	printf("opened fd %d\n",ffd);
	//read pid of alls
	int n=read(ffd,buffer,100);
	printf("%d\n",n);
	int pid=atoi(buffer);
	printf("%d\n",pid);
	fflush(stdout);
	//unlink(argv[1]);
	while(1)
	{
		//send signal to alls by pressing any character
		printf("%d\n",pid);
		read(0,&c,1);
		printf("%d\n",pid);
		printf("read %c\n",c);
		fflush(stdout);
		kill(pid,SIGUSR1);
	}
	
}
