#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
typedef struct mem
{
int rsfd;
} data;
void* writep(void* arg)
{
	data* t=(data*)arg;
	char buffer[1000];
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(0);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	while(1)
	{
		bzero(buffer,1000);
		read(0,buffer,1000);
		sendto(t->rsfd,buffer,1000,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
	}
}
int main()
{
	
	int rsfd=socket(AF_INET,SOCK_RAW,133);
	printf("%d\n",rsfd);
	const int on=0;
	struct ip* iph;
	
	if(setsockopt(rsfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))<0)
	{
		printf("option setting failed\n");
	}
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(0);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(rsfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		printf("bind failed\n");
	}
	pthread_t t1;
	data g;
	g.rsfd=rsfd;
	
	pthread_create(&t1,NULL,writep,&g);
	
	pthread_join(t1,NULL);
	
}
