#include<stdio.h>
#include<unistd.h>
#include<poll.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	
	int fd[4];int fd6;char buffer[100];
	FILE* fp1=popen("./P2","r");
	fd[0]=fileno(fp1);
	FILE* fp2=popen("./P2","r");
	fd[1]=fileno(fp2);
	FILE* fp3=popen("./P2","r");
	fd[2]=fileno(fp3);
	FILE* fp4=popen("./P2","r");
	fd[3]=fileno(fp4);
	FILE* fp5=popen("./P6","w");
	fd6=fileno(fp5);
	struct pollfd pfd[4];
	for(int i=0;i<4;i++)
	{
		pfd[i].fd=fd[i];
		pfd[i].events=POLLIN;
	}
	
	while(1)
	{
		bzero(buffer,100);
		int r=poll(pfd,4,0);
		if(r>0)
		{
			int pos=-1;
			for(int i=0;i<4;i++)
			{
				if(pfd[i].revents&POLLIN)
				{
					pos=i;
					break;
				}
			}
			
			//fprintf(stderr,"%d\n",pos);
			read(fd[pos],buffer,100);
			write(2,"poll1\n",7);
			write(fd6,buffer,100);
			
			
		}
		else
		{
			struct pollfd ppfd;
			ppfd.fd=1;
			ppfd.events=POLLIN;
			int r2=poll(&ppfd,1,0);
			if(r2>0)
			{
				read(ppfd.fd,buffer,100);
				write(2,"poll2\n",7);
				write(fd6,buffer,100);
			}
			
		}
		for(int i=0;i<4;i++)
		{
			pfd[i].revents=0;
		}
	}
}
