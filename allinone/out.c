#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc,char* argv[])
{
	char buffer[100];
	//open fifo with alls
	//alls writes contents to this fifo 
	int ffd=open(argv[1],O_RDONLY);
	printf("opened fd %d\n",ffd);
	int n=0;
	
	while(1)
	{
		//read from fifo
		n=read(ffd,buffer,100);
		//write to output
		write(1,buffer,n);
	}
	
}
