 #include "arraysum.h"
 #include<stdio.h>
 #include<stdlib.h>
 #include<rpc/rpc.h>
 
 arraysum_out *
 arraysumproc_1_svc(arraysum_in *inp, struct svc_req *rqstp)
 {
	 static arraysum_out out;
	 int i=0;
	 for( i=0;i<inp->arg1;i++)
	 {
	 	out.res1+=inp->arg2[i];
	 }
	
	 return (&out);
}
