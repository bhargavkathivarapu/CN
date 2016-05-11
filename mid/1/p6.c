#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main()
{
	char buffer[100];
	
	while(1)
	{
		bzero(buffer,100);
		read(0,buffer,100);
		write(1,buffer,100);
	}
	
}
