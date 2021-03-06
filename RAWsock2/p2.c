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
void printline()
{
	for(int i=0;i<32;i++)
	{
		printf("-");
	}
	printf("\n");
}
void packet(struct ip* iph)
{
	
	printline();
	printf("|%-4d|%-4d|%-4d|%-15d|\n",iph->ip_v,(iph->ip_hl),iph->ip_tos,ntohs(iph->ip_len));
	printline();
	printf("|%-14d|%-15d|\n",iph->ip_id,iph->ip_off);	
	printline();
	printf("|%-7d|%-6d|%-15d|\n",iph->ip_ttl,(iph->ip_p),(iph->ip_sum));
	printline();
	printf("|%-30s|\n",inet_ntoa(iph->ip_src));
	printline();
	printf("|%-30s|\n",inet_ntoa(iph->ip_dst));
		//printf("MESSAGE:\n");
	printline();
}
void* readp(void* arg)
{
	data* t=(data*)arg;
	struct ip* iph;
	char buffer[1000];
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(0);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	socklen_t len;
	while(1)
	{
		bzero(buffer,1000);
		recvfrom(t->rsfd,&buffer,1000,0,(struct sockaddr*)&servaddr,&len);
		
		iph=(struct ip*)buffer;
		printf("\nIP HEADER(exact version):\n\n");
		packet(iph);
		printf("\nIP HEADER:\n\n");
		printf("v:%d\n",iph->ip_v);
		printf("ipl:%d\n",(iph->ip_hl));
		printf("tos:%d\n",iph->ip_tos);
		printf("len:%d\n",ntohs(iph->ip_len));
		printf("id:%d\n",iph->ip_id);
		printf("off:%d\n",iph->ip_off);
		printf("ttl:%d\n",iph->ip_ttl);
		printf("protocol:%d\n",(iph->ip_p));
		printf("checksum:%d\n",(iph->ip_sum));
		printf("src:%s\n",inet_ntoa(iph->ip_src));
		printf("dst:%s\n",inet_ntoa(iph->ip_dst));
		printf("MESSAGE:\n");
		write(1,buffer+(iph->ip_hl)*4,1000-(iph->ip_hl)*4);
	}
}
void* writep(void* arg)
{
	data* t=(data*)arg;
	char buffer[1000];
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(0);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	while(1)
	{
		bzero(buffer,1000);
		struct ip* iph;
		iph=(struct ip*)buffer;
	
		iph->ip_v=4;
		iph->ip_hl=5;
		iph->ip_tos=0;
		iph->ip_id = 0;
		iph->ip_off=0;
		iph->ip_sum=0;
		iph->ip_ttl=128;
		iph->ip_p=132;
		inet_aton("127.0.2.1",&iph->ip_src);
		iph->ip_dst=servaddr.sin_addr;
		//strcpy(buffer+20,"sdfg");
		int n=read(0,buffer+(iph->ip_hl)*4,1000);
		iph->ip_len=htons(((iph->ip_hl)*4)+n);
		sendto(t->rsfd,buffer,1000,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
	}
}
int main()
{
	char buffer[1000];
	int rsfd=socket(AF_INET,SOCK_RAW,132);
	const int on=1;
	if(setsockopt(rsfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))<0)
	{
		printf("option setting failed\n");
	}
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(0);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(bind(rsfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		printf("bind failed\n");
	}
	pthread_t t1,t2;
	data g[2];
	g[0].rsfd=rsfd;
	g[1].rsfd=rsfd;
	pthread_create(&t1,NULL,readp,&g[0]);
	pthread_create(&t2,NULL,writep,&g[1]);
	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	
}
