
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<rpc/rpc.h>
#include "arraysum.h"

int main(int argc,char* argv[])
{
	CLIENT *cl;
	arraysum_in in;
	char path[100];
	char buffer[100];
	strcpy(path,".");
	
	printf("%s\n",argv[1]);
	if (argc != 3)
	{
	//err_quit("usage: client <hostname> <integer-value>");
	}
	printf("hi\n");
	cl = clnt_create(argv[1], 0x31230000, 1, "TCP");
	
	if(cl==NULL)
	{
		clnt_pcreateerror("hi");
	}
	int flag=0;
	
	while(1)
	{
		flag=0;
		arraysum_out *outp;
		char tmppath[100];
		bzero(in.arg1,1000);
		bzero(buffer,100);
		bzero(tmppath,100);
		read(0,buffer,100);

		if(buffer[0]=='c' && buffer[1]=='d' && buffer[2]==' ')
		{
			
		
		memcpy( tmppath, &buffer[3],strlen(buffer)-3 );
		tmppath[strlen(buffer)-4]='\0';
		strcpy(in.arg1,"cd ");
		strcat(in.arg1,path);
		strcat(in.arg1,"\n");
		strcat(in.arg1,"cd ");
		strcat(in.arg1,tmppath);
		strcat(in.arg1,"\npwd");
		printf("%s\nkk",in.arg1);
		flag=1;
		}
		else
		{
			strcpy(in.arg1,"cd ");
			strcat(in.arg1,path);
			strcat(in.arg1,"\n");
			strcat(in.arg1,buffer);
		}
		if ( (outp = arraysumproc_1(&in, cl)) == NULL)
		{
	
		//err_quit("%s", clnt_sperror(c1, argv[1]));
		}

		printf("resu1t: %s\n", outp->res1);
		if(flag==1)
		{
			strcpy(path,outp->res1);
			flag=0;
		}
		//bzero(outp->res1,100);
	}
	exit (0) ; 
}
