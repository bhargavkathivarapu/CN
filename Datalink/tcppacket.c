#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include <net/if.h>
#include <netinet/if_ether.h>
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
	struct tcphdr* tcph;
	char buffer[1000];
	struct sockaddr_in servaddr;
	
	socklen_t len;
	
	while(1)
	{
		bzero(buffer,1000);
		recvfrom(t->rsfd,&buffer,1000,0,(struct sockaddr*)&servaddr,&len);
		iph=(struct ip*)buffer;
		if(iph->ip_p==132)
		{
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
		
		tcph=(struct tcphdr*)(buffer+4*iph->ip_hl);
		printf("\nTCP HEADER:\n\n");
		
		printf("source port:%d\n",tcph->source);
		printf("dest port:%d\n",tcph->dest);
		printf("seq:%d\n",tcph->seq);
		printf("ack:%d\n",(tcph->ack_seq));
		printf("window:%d\n",(tcph->window));
		printf("checksum:%d\n",(tcph->check));
		printf("ugrentptr :%d\n",tcph->urg_ptr);
		
		//printf("MESSAGE:\n");
		//write(1,buffer+(iph->ip_hl)*4,1000-((iph->ip_hl)*4));
		}
		else
		{
			printf("protocol:%d\n",(iph->ip_p));
		}
	}
}
void* writep(void* arg)
{

	setuid(getuid());
	data* t=(data*)arg;
	char buffer[1000];
	struct sockaddr_in servaddr;
	servaddr.sin_family=PF_PACKET;
	servaddr.sin_port=htons(0);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	while(1)
	{
		bzero(buffer,1000);
		
		struct tcphdr* tcph;
		tcph=(struct tcphdr*)buffer;
	
		tcph->source=32001;
		tcph->dest=32001;
		tcph->seq=0;
		tcph->ack_seq = 0;
		tcph->check=0;
		tcph->window=0;
		tcph->urg_ptr=0;
		tcph->doff=0;
		
		
		//strcpy(buffer+20,"sdfg");
		
		int n=read(0,buffer+(5)*4,1000);
		
		printf("ffg\n");
		sendto(t->rsfd,buffer,1000,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
	}
}
int main()
{
	
	int rsfd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	printf("%d\n",rsfd);
	printf("%d\n",htons(ETH_P_IP));
	printf("%d\n",(ETH_P_IP));
	printf("%d\n",htons(ETH_P_ALL));
	const int on=0;
	
	struct sockaddr_in servaddr;
	servaddr.sin_family=PF_PACKET;
	servaddr.sin_port=htons(0);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(rsfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		printf("bind failed\n");
	}
	
	if(setsockopt(rsfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))<0)
	{
		printf("option setting failed\n");
	}
	/*char opt[8];
	if(setsockopt(rsfd,IPPROTO_IP,IP_OPTIONS,&opt,sizeof(opt))<0)
	{
		printf("option setting failed\n");
	}
	strcpy(opt,"sdfdfdd");
	
*/
	
	
	
	
	
	pthread_t t1,t2;
	data g[2];
	g[0].rsfd=rsfd;
	g[1].rsfd=rsfd;
	pthread_create(&t1,NULL,readp,&g[0]);
	pthread_create(&t2,NULL,writep,&g[1]);
	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	
}

