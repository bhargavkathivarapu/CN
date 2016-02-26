#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<stdlib.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<wait.h>
/*************semaphore implementations*****************/

int sem_init(int nsems,unsigned short int* values)
{

	 key_t k=ftok(".", 's');

	 int sem_id=semget(43254, nsems, IPC_CREAT|0666);
	 
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
	//if(x!=-1)
	//printf("released\n");
}

void wait(int* sem_ID,int m)
{
	struct sembuf tmp={m,-1,SEM_UNDO};
	int x=semop(*sem_ID, &tmp, 1);
	if(x==-1)
	{
		//printf("already locked waiting\n");
	}
	else
	{
	//printf("got the lock on %d \n",m);
	}
}

/*************END of semaphore implementations*****************/
int main()
{
	char buffer[100];
	char path[4][100];
	int sfd[4];
	int activce[4];
	
	int maxlimit[4]={1,3,4,2};
	int shmid=shmget(12343,sizeof(maxlimit),IPC_CREAT|0666);
	int* initt=(int*)shmat(shmid,NULL,0);
	for(int i=0;i<4;i++)
	{
		initt[i]=maxlimit[i];
	}
	shmdt(initt);
	
	unsigned short int values[]={1};
 	int sem_ID=sem_init(1, values);
	for(int i=0;i<4;i++)
	{
		bzero(path[i],100);
		strcpy(path[i],"./s1");
	}
	
	int p[4]={32001,32002,32003,32004};
	int maxfd=-1;
	struct sockaddr_in myaddr[4];
	fd_set readset;
	FD_ZERO(&readset);
	for(int i=0;i<4;i++)
	{
		myaddr[i].sin_family=AF_INET;
		myaddr[i].sin_port=htons(p[i]);
		myaddr[i].sin_addr.s_addr=htonl(INADDR_ANY);
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
	struct timeval timer;
	timer.tv_sec=0;
	timer.tv_usec=0;
	while(1)
	{
		printf("Selecting\n");
		FD_ZERO(&readset);
		for(int i=0;i<4;i++)
		{
			FD_SET(sfd[i],&readset);
		}
		select(maxfd+1,&readset,NULL,NULL,NULL);
		  pos=-1;
		 for(int i=0;i<4;i++)
		 {
		 	if(FD_ISSET(sfd[i],&readset))
		 	{
		 		pos=i;
		 		break;
		 	}	
		 }
		 printf("accepted %d\n",pos);
		 nsfd=accept(sfd[pos],(struct sockaddr*)&cliaddr,&clen);
		
		 pid=fork();
		 if(pid==0)
		 {
		 	for(int i=0;i<4;i++)
		 	close(sfd[i]);
		 	bzero(buffer,100);
		 	recv(nsfd,buffer,100,0);
		 	printf("requestes %s\n",buffer);
			
			int shmid=shmget(12343,sizeof(maxlimit),IPC_CREAT|0666);
	
			int semID=semget(43254,1,IPC_CREAT|0666);
			
			int *mlimit=(int*)shmat(shmid,NULL,0);
			if(mlimit[pos]==0)
			{
				printf("MAxlimit reached\n");
				send(nsfd,"unsuccess",8,0);
			}
			else
			{
				send(nsfd,"success",8,0);
				wait(&semID,0);
				
				mlimit[pos]--;
				printf("updated %d\n",mlimit[pos]);
				
				signal(&semID,0);
				
				int cp=fork();
				if(cp==0)
				{
					dup2(nsfd,0);
					dup2(nsfd,1);
			
					execlp(path[pos],path[pos],(char*)0);
					exit(0);
				}
				else
				{
					wait(NULL);
					wait(&semID,0);
					
					mlimit[pos]++;
					printf("released- %d\n",mlimit[pos]);
					signal(&semID,0);
				}
			}
			shmdt(mlimit);
			exit(0);
		}
		else
		{
			close(nsfd);
		}
	}
}

