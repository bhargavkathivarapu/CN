#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pcap.h>
#include <netinet/in.h>
 #include <netinet/ip.h>
#include<arpa/inet.h>
#include<sys/socket.h>
 #include <netinet/tcp.h>
#include<netdb.h>
 #include <net/if.h>
#include <netinet/if_ether.h>
#define MAXLINE 100
 int datalink;
 pcap_t *pd;
 char *device="eth0";
 int pn=0;;
 int snaplen=200;
 struct sockaddr* dest;
 int destlen;
 #define CMD "tcp"
void open_pcap(void)
{
	uint32_t localnet, netmask;
	 char cmd[MAXLINE], errbuf[PCAP_ERRBUF_SIZE]="error",
	 str1[INET_ADDRSTRLEN], str2[INET_ADDRSTRLEN];
	 struct bpf_program fcode;
	 if (device == NULL) {
	 
	 if ( (device = pcap_lookupdev(errbuf)) == NULL)
	 printf("pcap_lookup: %s", errbuf);
	 }
	 printf("device = %s\n", device);
	 /* hardcode: promisc=0, to_ms=500 */
	 if ( (pd = pcap_open_live(device, snaplen, 0, 500, errbuf) ) == NULL)
			printf("pcap_open_live: %s\n", errbuf);
	 if (pcap_lookupnet(device, &localnet, &netmask, errbuf) < 0)
	 	printf("pcap_lookupnet: %s\n", errbuf);
	 
	 printf("localnet = %s, netmask = %s\n",
 inet_ntop(AF_INET, &localnet, str1, sizeof(str1)),
 inet_ntop(AF_INET, &netmask, str2, sizeof(str2)));
 snprintf(cmd,sizeof(cmd),CMD);
	 if(pcap_compile(pd,&fcode,cmd,0,netmask)<0)
	 {
	 	printf("errr in pcap compile\n");
	 	
	 }
	 if(pcap_setfilter(pd,&fcode)<0)
	 {
	 	printf("errr in pcap setfilter\n");
	 }
	 if((datalink=pcap_datalink(pd))<0)
	 {
	 	printf("errr in pcap datalink\n");
	 }
	 printf("datalink %d\n",datalink);
	 
}

char* next_pcap(int *len)
 {
	 char *ptr;
	 struct pcap_pkthdr hdr;
	 /* keep looping until packet ready */
	 while ( (ptr = (char *) pcap_next(pd, &hdr)) == NULL) ;
	 *len = hdr.caplen; /* captured length */
	 return (ptr);
 }
 void printline()
{
	for(int i=0;i<32;i++)
	{
		printf("-");
	}
	printf("\n");
}
 void ipheader(struct ip* iph)
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
 void tcpheader(struct tcphdr* tcph)
{
	
	printline();
	printf("|%-14d|%-15d|\n",tcph->source,tcph->dest);
	printline();
	printf("|%-30u|\n",tcph->seq);	
	printline();
	printf("|%-30u|\n",(tcph->ack_seq));
	printline();
	printf("|%-3d| R |%d|%d|%d|%d|%d|%d|%-10d|\n",tcph->doff,tcph->urg,tcph->ack,tcph->psh,tcph->rst,tcph->syn,tcph->fin,tcph->window);
	printline();
	printf("|%-14d|%-15d|\n",tcph->check,tcph->urg_ptr);
		//printf("MESSAGE:\n");
	printline();
}
void check(char* ptr,int len)
 {
 
 	struct ip* iph=(struct ip*)ptr;
 	printf("Packet no: %d \n",pn);
 	pn++;
 	ipheader(iph);	
 	if(len>=40 && iph->ip_p==6)
 	{
 		struct tcphdr* tcph=(struct tcphdr*)(ptr+4*iph->ip_hl);
		printf("\nTCP HEADER:\n");
		tcpheader(tcph);
		/*if(len>40)
		{
			printf("Message\n");
			write(1,ptr+40,1000);
		}*/
	}
	else
	{
		printf("protocol: %d \n",iph->ip_p);
	}
 }
 void tcp_read(void)
 {
	 int len;
	 char *ptr;
	 struct ether_header *eptr;
	 for ( ; ; ) 
	 {
		 ptr = next_pcap(&len);
		 switch (datalink) 
		 {
		 
			 case DLT_NULL: /* loopback header = 4 bytes */
			 printf("case 1 packet\n");
			 return  ;
			//(check(ptr + 4, len - 4));
			 case DLT_EN10MB:
			 eptr = (struct ether_header *) ptr;
			 if (ntohs(eptr->ether_type) != ETHERTYPE_IP)
			 printf("Ethernet type %x not IP", ntohs(eptr->ether_type));
			 
			  printf("\n\ncase 2 packet IPHEADER\n");
			  check(ptr + 14, len - 14);
			 return ;
			 // (check(ptr + 14, len - 14));
			 case DLT_SLIP: /* SLIP header = 24 bytes */
			  printf("case 3 packet\n");
			 return ;
			 //(check(ptr + 24, len - 24));
			 case DLT_PPP: /* PPP header = 24 bytes */
			  printf("case 4 packet\n");
			 return ;
			 //(check(ptr + 24, len - 24));
			 default:
			 printf("unsupported datalink (%d)", datalink);
	 	}
 	}
 }
int main()
{
	open_pcap();
	while(1)
	tcp_read();	
	
}
