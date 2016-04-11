struct arraysum_in{ /* input (argument) */
	char arg1[1000];
};
 
struct arraysum_out{ /* output (result) */
	char res1[10000];
};

program ARRAYSUM_PROG {
	 version ARRAYSUM_VERS {
	 	arraysum_out ARRAYSUMPROC(arraysum_in) = 1; /* procedure number = 1 */
	} = 1; /* version number */
 } = 0x31230000; /* program number */ 
