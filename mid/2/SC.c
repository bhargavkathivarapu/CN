#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/sem.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/shm.h>
#include<poll.h>
/*************semaphore implementations*****************/

int sem_init(int nsems,unsigned short int* values)
{

	 key_t k=ftok(".", 's');

	 int sem_id=semget(43765, nsems, IPC_CREAT|0666);
	 
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
	int x=semop(*sem_ID, &tmp, 1);
	if(x!=-1)
	printf("released\n");
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


//array of child pids
int pids[3];
//status of child free or not free(1 or 0)
int idle[3];
int psfd[3]={1,1,1};
//update free based on pid of sending process
void freeservice(long cpid)
{
	for(int i=0;i<3;i++)
	{
		if(pids[i]==cpid)
		{
			idle[i]=1;
			
			return;
		}
	}
	
}
int getfree()
{
	for(int i=0;i<3;i++)
	{
		if(idle[i]==1)
		{
			return i;
		}
	}
}
 void handler(int sig, siginfo_t *siginfo, void *context)
{
	printf ("Sending PID: %ld, UID: %ld\n",
			(long)siginfo->si_pid, (long)siginfo->si_uid);
			
			freeservice((long)siginfo->si_pid);
	
}
int main(int argc,char* argv[])
{
	char buffer[100];
	int n=0;
	int shmid=shmget(12345,100,IPC_CREAT|0666);
	//initialize signal handler
	struct sigaction s;
    s.sa_sigaction = &handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags =SA_SIGINFO;
    sigaction (SIGUSR1, &s, NULL);
    //initialize semaphore
  
    unsigned short int values[]={1};
 	int sem_ID=sem_init(1, values);
	//initially all servicing process are free
	//printf("%d\n",x);
	for(int i=0;i<3;i++)
	{
	idle[i]=1;
	}
	//fork 5 processes and store their pids
	for(int i=0;i<3;i++)
	{
		pids[i]=fork();
		if(pids[i]==0)
		{
			char buf[24];
			sprintf(buf,"%d",32004+i);
			execlp("./serv","./serv",buf,(char*)NULL);
			//exit(0);
		}
	}
	
	
	
	char path[3][100];
	int sfd[3];
	for(int i=0;i<3;i++)
	{
		bzero(path[i],100); 
		strcpy(path[i],"./s1");
	}
	
	int p[3]={32001,32002,32003};
	int maxfd=-1;
	struct sockaddr_in myaddr[3],plataddr;
	inet_aton("127.0.0.1",(struct in_addr *)&plataddr.sin_addr);
	plataddr.sin_family=AF_INET;
	plataddr.sin_port=htons(32004);
	fd_set readset;
	FD_ZERO(&readset);
	for(int i=0;i<3;i++)
	{
		inet_aton("127.0.0.1",(struct in_addr *)&myaddr[i].sin_addr);
		myaddr[i].sin_family=AF_INET;
		myaddr[i].sin_port=htons(p[i]);
		//myaddr[i].sin_addr.s_addr=htonl(INADDR_ANY);
		sfd[i]=socket(AF_INET,SOCK_STREAM,0);
		if(maxfd<sfd[i])
		{
			maxfd=sfd[i];
		}
		int er=bind(sfd[i],(const struct sockaddr*)&myaddr[i],sizeof(myaddr[i]));
		if(er==-1)printf("Bind failed\n");
		listen(sfd[i],1);
	}
	
	sockaddr_in cliaddr;
	socklen_t clen;
	int pid;
	int nsfd;
	int pos=-1;
	
	
	while(1)
	{
		
		FD_ZERO(&readset);
		for(int i=0;i<3;i++)
		{
			if(sfd[i]!=-1)
			FD_SET(sfd[i],&readset);
		}
		select(maxfd+1,&readset,NULL,NULL,NULL);
		  pos=-1;
		  for(int i=0;i<3;i++)
		 {
		 	if(FD_ISSET(sfd[i],&readset))
		 	{
		 		pos=i;
		 		break;
		 	}	
		 }
		
		
	printf("all running\n");
	//in parent process
	//mkfifo(argv[1],0666);
	//int fd=open(argv[1],O_RDWR);
	
	//signal one  of the free services-update free
	int r=getfree();
	
	if(r>=0)
	{
		
		 int nsfd=accept(sfd[pos],NULL,NULL);
		plataddr.sin_port=htons(32004+r);
		idle[r]=0;
		
		kill(pids[r],SIGUSR1);
		int sp=sizeof(plataddr);
		send(nsfd,&sp,sizeof(int),0);
		send(nsfd,&plataddr,sizeof(plataddr),0);
		printf("allocated process %d - %d\n",r,pids[r]);
		/*wait(&sem_ID,0);
		
	
			printf("hi %d\n",shmid);
			int *a;
		a = (int*)shmat(shmid, NULL, 0);
    
		
		*a=plat[r];*/
		
		
		close(nsfd);
		//shmdt(a);
		
		//signal(&sem_ID,0);
	}
	
	
	}
	
	return 0;
}
