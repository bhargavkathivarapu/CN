struct arraysum_in{ /* input (argument) */
	long arg1;
	long arg2[100];
	
};
 
struct arraysum_out{ /* output (result) */
	long res1;
};

program ARRAYSUM_PROG {
	 version ARRAYSUM_VERS {
	 	arraysum_out ARRAYSUMPROC(arraysum_in) = 1; /* procedure number = 1 */
	} = 1; /* version number */
 } = 0x31230000; /* program number */ 
