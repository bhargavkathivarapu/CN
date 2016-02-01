#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc,char* argv[])
{
	char buffer[100];
	int n=0;
	int id=atoi(argv[1]);
	//open one fifo for mms other to communicate with service requested
	mkfifo(argv[2],0666);
	//mkfifo(argv[3],0666);
	printf("came1");
	fflush(stdout);
	//int frfd1=open(argv[2],O_RDONLY);
	printf("came2");
	fflush(stdout);
	int fwfd1=open(argv[2],O_WRONLY); 
	
	printf("came3");
	fflush(stdout);
	n=read(0,buffer,100);
	//send name of service needed to mss
	write(fwfd1,buffer,n);
	
		scanf("Enter service fifo name:%s",buffer);
		//send fifo name to be open by service to mss
		write(fwfd1,buffer,strlen(buffer));
		
		mkfifo(buffer,0666);
		//mkfifo(b2,0666);
		int frfd2=open(buffer,O_RDONLY);
		//int fwfd2=open(argv[4],O_WRONLY); 
		//start sending messages to service through fifo
		//write(fwfd2,buffer,n);
		//read results of service
		n=read(frfd2,buffer,100);
		//display to screen
		write(1,buffer,n);
	
		
	
	
	
	
	
	return 0;
}
