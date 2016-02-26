#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<string.h>
int main()
{
	char buffer[100];
	bzero(buffer,100);
	int n=read(0,buffer,100);
	for(int i=0;i<n;i++)
	{
		buffer[i]=tolower(buffer[i]);
	}
	write(1,buffer,n);
}
