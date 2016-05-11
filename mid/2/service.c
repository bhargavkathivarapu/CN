#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#include<signal.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
/*************semaphore implementations*****************/

int sem_init(int nsems,unsigned short int* values)
{

	 key_t k=ftok(".", 's');

	 int sem_id=semget(k, nsems, IPC_CREAT|0666);
	 
	 if(sem_id==-1)
	 {
	  printf("Semaphore init error\n");
	  exit(1);
	 }
	 
	 union semun 
	 {
	 	int val;
	 	struct semid_ds *buf;
	 	unsigned short int *array;
	 	
	 } t;
	
	 t.array=values;

	 
	 semctl(sem_id,0,SETALL,t);
	 
	 return sem_id;
}

void sem_remove(int sem_ID)
{

	semctl(sem_ID,0,IPC_RMID);

}

void signal(int* sem_ID,int m)
{
	struct sembuf tmp={m,1,SEM_UNDO};
	semop(*sem_ID, &tmp, 1);
}

void wait(int* sem_ID,int m)
{
	struct sembuf tmp={m,-1,SEM_UNDO};
	int x=semop(*sem_ID, &tmp, 1);
	if(x==-1)
	{
		printf("already locked waiting\n");
	}
	else
	{
	printf("got the lock on %d \n",m);
	}
}

/*************END of semaphore implementations*****************/

int ppid;
char buffer[100];
int sem_id,shmid,sfd;
void handler(int sig)
{
	signal(SIGUSR1,handler);
	
	/*wait(&sem_id,0);
	int port;
	
	int* a=(int*)shmat(shmid,0,0);
	port=*a;
	printf("%d\n",port);
	shmdt(a);
	
	signal(&sem_id,0);
	*/
	
	//myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//printf("%s\n",inet_ntoa(myaddr.sin_addr));
	
	struct sockaddr_in cliaddr;
	socklen_t clen=sizeof(cliaddr);
	printf("fggg\n");
	int nsfd=accept(sfd,(struct sockaddr*)&cliaddr,&clen);
	printf("fggg\n");
	char buff[100];
	bzero(buff,100);
	recv(nsfd,buff,100,0);
	write(1,buff,strlen(buff));
	send(nsfd,"received",9,0);
	close(nsfd);
	
	kill(ppid,SIGUSR1);
	
}
int main(int argc,char* argv[])
{
		
	int n=0;
	 sem_id=semget(43765,1, IPC_CREAT|0666);
	 shmid=shmget(12345,100,IPC_CREAT|0666);
	 sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in myaddr;
	inet_aton("127.0.0.1",(struct in_addr *)&myaddr.sin_addr);
	myaddr.sin_family=AF_INET;
	myaddr.sin_port=htons(atoi(argv[1]));
	bind(sfd,(const struct sockaddr*)&myaddr,sizeof(myaddr));
	listen(sfd,5);
	//initialize signal handler
	ppid=getppid();
	signal(SIGUSR1,handler);
	printf("%d\n",getpid());
	fflush(stdout);
	while(1){}
	
	return 0;
}
