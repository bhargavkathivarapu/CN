struct func_in{ /* input (argument) */
	int fd;
	char buffer[1000];
	int size;
};
 
struct func_out{ /* output (result) */
	int n;
	char buffer[1000];
};
struct open_in
{
	char filename[100];
	int flags;
};
struct open_out
{
	int fd;
};

struct close_in
{
	int fd;
};
struct close_out
{
	int res;
};

struct seek_in
{
	int fd;
	int offset;
	int whence;
};
struct seek_out
{
	int n;
};
program FUNC_PROG {
	 version FUNC_VERS {
	 	open_out OPENPROC(open_in) = 1;
	 	func_out READPROC(func_in) = 2; /* procedure number = 1 */
	 	func_out WRITEPROC(func_in) = 3;
	 	close_out CLOSEPROC(close_in) = 4;
	 	seek_out LSEEKPROC(seek_in) = 5;
	} = 1; /* version number */
 } = 0x31230000; /* program number */ 
