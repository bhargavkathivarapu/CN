#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
typedef struct mem
{
	int id;
	int ffd;
}par; 
void *readp(void* ar)
{
	char buffer[100];int n=0;
	par* t=(par*)ar;
	while(1)
	{
		//printf("write here:");
		//fflush(stdout);
		n=read(0,buffer,100);
		write(t->ffd,buffer,n);
		usleep(1000);
	}
}
void *writep(void* ar)
{
	char buffer[100];int n=0;
	par* t=(par*)ar;
	while(1)
	{
		n=read(t->ffd,buffer,100);
		write(1,buffer,n);
		
		fflush(stdout);
		usleep(1000);
	}
}
int main(int argc,char* argv[])
{
	
	int id=atoi(argv[1]);
	mkfifo(argv[2],0666);
	mkfifo(argv[3],0666);
	
	int fwfd=open(argv[2],O_WRONLY);
	int frfd=open(argv[3],O_RDONLY);
	//printf("hi");
	//fflush(stdout);
	pthread_t reader,writer;
	par g[2];
	g[0].id=id;
	g[0].ffd=fwfd;
	g[1].id=id;
	g[1].ffd=frfd;
	
	pthread_create(&reader,NULL,&readp,&g[0]);
	pthread_create(&writer,NULL,&writep,&g[1]);
	
	pthread_join(reader,NULL);
	pthread_join(writer,NULL);
	
	return 0;
}
