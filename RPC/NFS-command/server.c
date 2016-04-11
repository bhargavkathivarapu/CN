 #include "arraysum.h"
 #include<stdio.h>
 #include<unistd.h>
 #include<wait.h>
 #include<stdlib.h>
 #include<string.h>
 #include<fcntl.h>
 #include<rpc/rpc.h>
 
 arraysum_out *
 arraysumproc_1_svc(arraysum_in *inp, struct svc_req *rqstp)
 {
	 
	 
	 //printf("%d\n",fd);
	 int pid=vfork();
	 if(pid==0)
	 {
	 	
	 	int fd=open("data.txt",O_WRONLY|O_TRUNC);
	 	dup2(fd,1);
	 	int ff=open("fi.sh",O_WRONLY|O_TRUNC);
	 	write(ff,"#!/bin/sh\n",10);
	 	write(ff,inp->arg1,strlen(inp->arg1));
	 	close(ff);
	 	execlp("./fi.sh","./fi.sh",(char*)0);
	 }
	 else
	 {
	 
	 	static arraysum_out out;
	 	printf("hi\n");
	 	wait(NULL);
	 	printf("hii\n");
	 	bzero(out.res1,10000);
	 	int fd=open("data.txt",O_RDONLY);
	 	read(fd,out.res1,10000);
	 	close(fd);
	 	
	 	return (&out);
	 }
	
	 
}
