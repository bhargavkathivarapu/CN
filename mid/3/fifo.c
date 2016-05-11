#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc,char* argv[])
{
	char buffer[100];int n=0;
	mkfifo(argv[1],0666);
	//open fifo with alls
	printf("hi\n");
	int ffd=open(argv[1],O_RDWR);
	
	printf("opened fd %d\n",ffd);
	while(1)
	{
		//read from stdin and write to fifo
		n=read(0,buffer,100);
		write(ffd,buffer,n);
	}
	
}
