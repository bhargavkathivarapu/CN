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
	mkfifo(argv[1],0666);
	//mkfifo(argv[2],0666);
	int n=0;
	printf("came1");
	fflush(stdout);
	//open well known fifo
	int frfd=open(argv[1],O_RDONLY);
	printf("came2");
	fflush(stdout);

	//read the service requested by the client
	bzero(buffer,100);
	read(frfd,buffer,100);
	write(1,buffer,100);
	//execute the service as sub processs and write to its stdin 
	FILE* fp=popen(buffer,"w");
	int pofd=fileno(fp);
	//receive the name of fifo sent from client to communicate with service
	n=read(frfd,buffer,100);
	write(pofd,buffer,n);
	bzero(buffer,100);
	n=read(frfd,buffer,100);
	write(pofd,buffer,n);
	pclose(fp);
	
	
	
	
}
