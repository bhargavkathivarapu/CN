#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
int main(int argc,char* argv[])
{
		
		char buffer[100];int n=0;
	
	
	n=read(0,buffer,100);

	int fd=open(buffer,O_WRONLY);
	/*int c=0;
	while((n=read(fd,buffer,100))>0)
	{
		for(int i=0;i<n;i++)
		{
			if(buffer[i]=='\n')
			c++;
		}
	}
	itoa(c,buf,10);*/
	char buff[100]="Hello world";
	write(fd,buff,strlen(buff));
	
	
	return 0;
}
