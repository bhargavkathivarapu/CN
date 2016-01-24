#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc,char* argv[])
{
	
	char buff[100];
	int fd,fd2;
	
	for(int i=1;i<argc;i++)
	{
		if(i%2!=0)
		{
			 fd=open("tmp.txt",O_RDONLY);
			 fd2=open("tmp2.txt",O_WRONLY);
			dup2(fd2,1);
		
			FILE* f2=fdopen(fd,"r");
			FILE* f=popen(argv[i],"w");
			//f=f2;
			while(fgets(buff,100,f2)>0)
			{
				fputs(buff,f);
			}
	
			pclose(f);
			close(fd);
			close(fd2);
		}
		else
		{
			fd2=open("tmp.txt",O_WRONLY);
			fd=open("tmp2.txt",O_RDONLY);
			dup2(fd2,1);
		
			FILE* f2=fdopen(fd,"r");
			FILE* f=popen(argv[i],"w");
			//f=f2;
			while(fgets(buff,100,f2)>0)
			{
				fputs(buff,f);
			}
	
			pclose(f);
			close(fd);
			close(fd2);
		}
		
	}
}
