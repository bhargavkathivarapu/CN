
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<rpc/rpc.h>
#include "func.h"

int main(int argc,char* argv[])
{
	CLIENT *cl;
	open_in in;
	
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
	
	
		open_out *outp;
		bzero(in.filename,100);
		in.flags=0;
		read(0,in.filename,100);
		printf("%d\n",O_RDONLY);
		in.flags=in.flags|O_RDONLY;
		printf("%s\n",in.filename);
		if ( (outp = openproc_1(&in, cl)) == NULL)
		{
	
		//err_quit("%s", clnt_sperror(c1, argv[1]));
		}

		printf("result: %d\n", outp->fd);
		//bzero(outp->res1,100);
		
		func_in rin;
		func_out* wot;
		
		int fd=outp->fd;
		rin.fd=fd;
		rin.size=10;
		wot = readproc_1(&rin, cl);
		write(1,wot->buffer,wot->n);
		seek_in sin;
		sin.fd=fd;
		sin.offset=10;
		sin.whence=SEEK_CUR;
		printf("\n");
		seek_out *sout=lseekproc_1(&sin,cl);
		wot = readproc_1(&rin, cl);
		write(1,wot->buffer,wot->n);
		
		
	exit (0) ; 
}
