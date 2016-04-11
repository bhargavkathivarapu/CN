
#include<stdio.h>
#include<stdlib.h>
#include<rpc/rpc.h>
#include "arraysum.h"

int main(int argc,char* argv[])
{
	CLIENT *cl;
	arraysum_in in;
	arraysum_out *outp;
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
	
	in.arg1 = atol(argv[2]);

	int j=0;
	for(j=0;j<in.arg1;j++)
	{
		scanf("%ld",&in.arg2[j]);
	}
	
	if ( (outp = arraysumproc_1(&in, cl)) == NULL)
	{
	
	//err_quit("%s", clnt_sperror(c1, argv[1]));
	}

	printf("resu1t: %ld\n", outp->res1);
	exit (0) ; 
}
