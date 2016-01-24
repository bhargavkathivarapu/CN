#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
//#include <sys/mman.h>
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
		//printf("already locked waiting\n");
	}
	else
	{
	//printf("got the lock on %d \n",m);
	}
}


/*************END of semaphore implementations*****************/




void p1(int sem_ID,int* x,int* y)
{
	int temp=0;
	while(1)
	{
		wait(&sem_ID,1);
		temp=*y;
		printf("P1:got y %d \n",temp);
		signal(&sem_ID,1);
		
		wait(&sem_ID,0);
		printf("P1:placed in x=%d\n",temp+1);
		*x=temp+1;
		signal(&sem_ID,0);
		usleep(100000);
		
	}
}

void p2(int sem_ID,int* x,int* y)
{
	int temp=0;
	while(1)
	{
		wait(&sem_ID,0);
		temp=*x;
		printf("P2:got x %d \n",temp);
		signal(&sem_ID,0);
		
		wait(&sem_ID,1);
		printf("P2:placed in y=%d\n",temp+1);
		*y=temp+1;
		signal(&sem_ID,1);
		usleep(100000);
		
	}
}
int main()
{
	
    unsigned short int values[]={1,1};
 	int sem_ID=sem_init(2, values);
 	int shm1=shmget(12355,sizeof(int),IPC_CREAT|0666);
 	int shm2=shmget(13456,sizeof(int),IPC_CREAT|0666);
 	int* x=(int*)shmat(shm1,0,0);
 	int* y=(int*)shmat(shm2,0,0);
 	
	 //int* x = (int*)mmap(NULL,sizeof(int),PROT_READ |PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
	// int* y = (int*)mmap(NULL,sizeof(int),PROT_READ |PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
	pid_t pid=vfork();
	
	if(pid==0)
	{
	
		
			pid=fork();
			if(pid!=0)
			{
				p1(sem_ID,x,y);
			}
			else
			{
				p2(sem_ID,x,y);
			}
	
	}
	else
	sem_remove(sem_ID);
	shmdt(x);
	shmdt(y);
	 return 0;
}

