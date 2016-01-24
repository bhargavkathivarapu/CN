#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main()
{
	char buffer[100];
	int n=0;
	bzero(buffer,100);
	
	scanf("%s",buffer);

fprintf(stderr,"got by process 1 as %s \n",buffer);
	printf("%s\n",buffer);
	
	
	
}
