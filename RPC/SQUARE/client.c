
#include<stdio.h>
#include<stdlib.h>
#include<rpc/rpc.h>
#include "square.h"

int main(int argc,char* argv[])
{
	CLIENT *cl;
	square_in in;
	square_out *outp;
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
	printf("kj\n");
	}
	in.arg1 = atol(argv[2]);
	printf("hi\n");
	if ( (outp = squareproc_1(&in, cl)) == NULL)
	{
	printf("gh\n");
	//err_quit("%s", clnt_sperror(c1, argv[1]));
	}
	printf("ph\n");
	printf("resu1t: %ld\n", outp->res1);
	exit (0) ; 
}
