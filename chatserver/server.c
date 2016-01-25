#include<stdio.h>
#include<poll.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
int main(int argc,char* argv[])
{
	char buffer[100];
	mkfifo(argv[1],0666);
	mkfifo(argv[2],0666);
	mkfifo(argv[3],0666);
	mkfifo(argv[4],0666);
	//open popular fifos with clients
	int frfd1=open(argv[1],O_RDONLY);
	int fwfd1=open(argv[2],O_WRONLY);
	//printf("reached here");
	//fflush(stdout);
		int frfd2=open(argv[3],O_RDONLY);
	int fwfd2=open(argv[4],O_WRONLY);
	//int ffd3=open(argv[3],O_RDWR);
	//printf("reached there");
	//fflush(stdout);
	//initial pollfd structure
	struct pollfd fds[2];
	fds[0].fd=frfd1;
	fds[0].events=POLLIN;
	fds[1].fd=frfd2;
	fds[1].events=POLLIN;
	//printf("all done upto here");
	//fflush(stdout);
	//fds[2].fd=ffd3;
	//fds[0].events=POLLIN;
	int n=0;
	//repeat forever
	while(1)
	{
		//printf("poll start");
		//fflush(stdout);
		bzero(buffer,100);
		//start polling the fd
		while(poll(fds,2,0)<=0){ }
		//read from an fd which is ready
		//printf("poll finish");
		//fflush(stdout);
		if(fds[0].revents & POLLIN)
		{
			n=read(fds[0].fd,buffer,100);
			char b2[100];
			strcpy(b2,"c1:");
			strcat(b2,buffer);
			//write(fwfd1,b2,strlen(b2));
			write(fwfd2,b2,strlen(b2));
		}
		else if(fds[1].revents  & POLLIN)
		{
			n=read(fds[1].fd,buffer,100);
			char b2[100];
			strcpy(b2,"c2:");
			strcat(b2,buffer);
			
			write(fwfd1,b2,strlen(b2));
			//write(fwfd2,b2,strlen(b2));
		}
	//	else if(fds[2].revents & POLLIN)
	//	n=read(fds[2].fd,buffer,100);
		
		//write message read to all clients
	}
	
}
