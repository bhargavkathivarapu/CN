#include "func.h"
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<rpc/rpc.h>

open_out *
 openproc_1_svc(open_in *inp, struct svc_req *rqstp)
 {
	static open_out out;
	printf("%s %d\n",inp->filename,inp->flags);
	inp->filename[strlen(inp->filename)-1]='\0';
	out.fd=open(inp->filename,inp->flags);
	printf("%d\n",out.fd);
	return(&out);
}
func_out *
 writeproc_1_svc(func_in *inp, struct svc_req *rqstp)
 {
	static func_out out;
	out.n=write(inp->fd,inp->buffer,inp->size);
	return(&out);
}
func_out *
 readproc_1_svc(func_in *inp, struct svc_req *rqstp)
 {
	static func_out out;
	out.n=read(inp->fd,out.buffer,inp->size);
	return(&out);
}
close_out *
 closeproc_1_svc(close_in *inp, struct svc_req *rqstp)
 {
	static close_out out;
	out.res=close(inp->fd);
	return(&out);
}
seek_out *
 lseekproc_1_svc(seek_in *inp, struct svc_req *rqstp)
 {
	static seek_out out;
	out.n=lseek(inp->fd,inp->offset,inp->whence);
	return(&out);
}

