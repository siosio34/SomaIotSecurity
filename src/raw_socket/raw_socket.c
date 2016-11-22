#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <asm/types.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

#include "arp_db.h"

#define BUF_SIZE 42
#define DEVICE "wlan1"
#define ETH_P_NULL 0x0
#define ETH_MAC_LEN ETH_ALEN
#define ETH_ARP 0x0806

int s = 0; /*Socketdescriptor*/
void* buffer = NULL;
long total_packets = 0;
long answered_packets = 0;

arp_db_t arp_db[3000]={0,};

arp_db_t arp_temp={0,};
int db_save_cnt=0;

void sigint(int signum);
void db_print(int signum);

struct __attribute__((packed)) arp_header
{
    unsigned short arp_hd;
    unsigned short arp_pr;
    unsigned char arp_hdl;
    unsigned char arp_prl;
    unsigned short arp_op;
    unsigned char arp_sha[6];
    unsigned char arp_spa[4];
    unsigned char arp_dha[6];
    unsigned char arp_dpa[4];
};

int main(void) {
    buffer = (void*)malloc(BUF_SIZE); /*Buffer for Ethernet Frame*/
    unsigned char* etherhead = buffer;  /*Pointer to Ethenet Header*/
    struct ethhdr *eh = (struct ethhdr *)etherhead; /*Another pointer to
                                                    ethernet header*/
    unsigned char* arphead = buffer + 14;
    struct arp_header *ah;
    unsigned char src_mac[6];    /*our MAC address*/

    struct ifreq ifr;
    struct sockaddr_ll socket_address;
    int ifindex = 0;     /*Ethernet Interface index*/
    int i;
    int length;  /*length of received packet*/
    int sent;
    printf("Server started, entering initialiation phase...\n");

    /*open socket*/
    s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (s == -1) {
        perror("socket():");
        exit(1);
    }
    printf("Successfully opened socket: %i\n", s);

    /*retrieve ethernet interface index*/
    strncpy(ifr.ifr_name, DEVICE, IFNAMSIZ);
    if (ioctl(s, SIOCGIFINDEX, &ifr) == -1) {
        perror("SIOCGIFINDEX");
        exit(1);
    }
    ifindex = ifr.ifr_ifindex;
    printf("Successfully got interface index: %i\n", ifindex);

    /*retrieve corresponding MAC*/
    if (ioctl(s, SIOCGIFHWADDR, &ifr) == -1) { //내맥주소가 맞는
        perror("SIOCGIFINDEX");
        exit(1);
    }
    for (i = 0; i < 6; i++) {
        src_mac[i] = ifr.ifr_hwaddr.sa_data[i];
    }
    //printf("Successfully got our MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //    src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]);

    /*prepare sockaddr_ll*/
    socket_address.sll_family = PF_PACKET; //소켓옵
    socket_address.sll_protocol = htons(ETH_P_IP);
    socket_address.sll_ifindex = ifindex;
    socket_address.sll_hatype = ARPHRD_ETHER;
    socket_address.sll_pkttype = PACKET_OTHERHOST;
    socket_address.sll_halen = 0;
    socket_address.sll_addr[6] = 0x00;
    socket_address.sll_addr[7] = 0x00;
    /*establish signal handler*/
    signal(SIGINT, sigint);
    
    signal(SIGUSR1, db_print);
    printf("Successfully established signal handler for SIGINT\n");
    printf("We are in production state, waiting for incoming packets....\n");
	memset(arp_db,0,sizeof(arp_db));
    while (1) {
        /*Wait for incoming packet...*/
        length = recvfrom(s, buffer, BUF_SIZE, 0, NULL, NULL);
        if (length == -1)
        {
            perror("recvfrom():");
            exit(1);
        }
        if(htons(eh->h_proto) == 0x806)
        {
			int in_table=0;
            unsigned char buf_arp_dha[6];
            unsigned char buf_arp_dpa[4];
            ah = (struct arp_header *)arphead;
            if(htons(ah->arp_op) != 0x0001)
                continue;
            //printf("buffer is---------------- %s \n",(char*)ah);
            //printf("H/D TYPE : %x PROTO TYPE : %x \n",ah->arp_hd,ah->arp_pr);
            //printf("H/D leng : %x PROTO leng : %x \n",ah->arp_hdl,ah->arp_prl);
            //printf("OPERATION : %x \n", ah->arp_op);
            printf("L2 DMAC: %02X:%02X:%02X:%02X:%02X:%02X",
                eh->h_dest[0],
                eh->h_dest[1],
                eh->h_dest[2],
                eh->h_dest[3],
                eh->h_dest[4],
                eh->h_dest[5]
            );
           memcpy(arp_temp.L2_DMAC,eh->h_dest,6);
            
            printf(" SMAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                eh->h_source[0],
                eh->h_source[1],
                eh->h_source[2],
                eh->h_source[3],
                eh->h_source[4],
                eh->h_source[5]
            );
           
            memcpy(arp_temp.L2_SMAC,eh->h_source,6);
             (arp_temp.L2_SMAC[0]=eh->h_source[0]);
             (arp_temp.L2_SMAC[1]=eh->h_source[1]);
             (arp_temp.L2_SMAC[2]=eh->h_source[2]);
             (arp_temp.L2_SMAC[3]=eh->h_source[3]);
             (arp_temp.L2_SMAC[4]=eh->h_source[4]);
             (arp_temp.L2_SMAC[5]=eh->h_source[5]);
            printf(" SMAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
               arp_temp.L2_SMAC[0],
               arp_temp.L2_SMAC[1],
               arp_temp.L2_SMAC[2],
               arp_temp.L2_SMAC[3],
               arp_temp.L2_SMAC[4],
               arp_temp.L2_SMAC[5]
            );
             printf("ARP: %02d:%02d:%02d:%02d",
                ah->arp_spa[0],
                ah->arp_spa[1],
                ah->arp_spa[2],
                ah->arp_spa[3]
            );
            memcpy(&arp_temp.arp_sip,ah->arp_spa,4);
            
            printf(" SMAC: (%02X:%02X:%02X:%02X:%02X:%02X)",
                ah->arp_sha[0],
                ah->arp_sha[1],
                ah->arp_sha[2],
                ah->arp_sha[3],
                ah->arp_sha[4],
                ah->arp_sha[5]
            );
            memcpy(&arp_temp.SMAC,ah->arp_sha,6);
            
            printf(": %02d:%02d:%02d:%02d",
                ah->arp_dpa[0],
                ah->arp_dpa[1],
                ah->arp_dpa[2],
                ah->arp_dpa[3]
            );
            memcpy(&arp_temp.arp_dip,ah->arp_dpa,4);
            
			printf(" DMAC: (%02X:%02X:%02X:%02X:%02X:%02X)\n",
                (0xFF)&ah->arp_dha[0],
                (0xFF)&ah->arp_dha[1],
                (0xFF)&ah->arp_dha[2],
                (0xFF)&ah->arp_dha[3],
                (0xFF)&ah->arp_dha[4],
                (0xFF)&ah->arp_dha[5]
            );
            memcpy(&arp_temp.DMAC,ah->arp_dha,6);
            
           printf("--------------------------------------- \n");
     
            if(ah->arp_spa[0]==10&&ah->arp_spa[1]==00&&ah->arp_spa[2]==00&&ah->arp_spa[3]==01)
            {
                printf("Sender ip is .............bam bam..........................................\n");
                system("sudo arp -s 10.0.0.1  00:1e:73:91:04:0d");
            }
           
            memcpy( (void*)etherhead, (const void*)(etherhead+ETH_MAC_LEN),
                ETH_MAC_LEN);
            memcpy( (void*)(etherhead+ETH_MAC_LEN), (const void*)src_mac,
                ETH_MAC_LEN);
            eh->h_proto = ETH_ARP;
           
				db_print(1);
			int i;
			for(i=0;i<db_save_cnt;i++)
			{
				if(memcmp(&arp_db[i],&arp_temp,sizeof(arp_temp))==0&&memcmp(&arp_db[i],&arp_temp,sizeof(arp_temp))==0)
				{
					printf("발견했습니다");
					in_table=1;
					break;
				}
			}
			if(!in_table)
			{
					printf("새로운 아이를 발견!!!");
					if(serch_db(arp_temp)) 
					{
					}
				else{
				arp_db[db_save_cnt]=arp_temp;
				db_save_cnt++;
				}
			}
			else
			{
				db_print(1);
				in_table=0;
			}
        }
    }
}

void sigint(int signum) {
    /*Clean up.......*/

    struct ifreq ifr;

    if (s == -1)
        return;

    strncpy(ifr.ifr_name, DEVICE, IFNAMSIZ);
    ioctl(s, SIOCGIFFLAGS, &ifr);
    ifr.ifr_flags &= ~IFF_PROMISC;
    ioctl(s, SIOCSIFFLAGS, &ifr);
    close(s);

    free(buffer);

    printf("Server terminating....\n");

    printf("Totally received: %ld packets\n", total_packets);
    printf("Answered %ld packets\n", answered_packets);
    exit(0);
}
int serch_db(arp_db_t temparp)
{	int i;
	for(i=0;i<db_save_cnt;i++)
			{
				if(strcmp(arp_db[i].SMAC,temparp.SMAC)==0&&!strcmp(arp_db[i].arp_sip,temparp.arp_sip)==0)
				{//잡았다 요놈
					printf("잡았따 요놈 %02d:%02d:%02d:%02d ",	temparp.arp_sip[0],
					temparp.arp_sip[1],
					temparp.arp_sip[2],
					temparp.arp_sip[3]);
					return 1;
				}
			}
			return 0;
}
void db_print(int signum) {
   	int i;
	for(i=0;i<db_save_cnt;i++)
			{
				printf("\n\n%d SMAC: (%02X:%02X:%02X:%02X:%02X:%02X)",
					i,
					(0xFF)&arp_db[i].SMAC[0],
					(0xFF)&arp_db[i].SMAC[1],
					(0xFF)&arp_db[i].SMAC[2],
					(0xFF)&arp_db[i].SMAC[3],
					(0xFF)&arp_db[i].SMAC[4],
					(0xFF)&arp_db[i].SMAC[5]
				);
				printf(" DMAC: (%02X:%02X:%02X:%02X:%02X:%02X)",
					(0xFF)&arp_db[i].DMAC[0],
					(0xFF)&arp_db[i].DMAC[1],
					(0xFF)&arp_db[i].DMAC[2],
					(0xFF)&arp_db[i].DMAC[3],
					(0xFF)&arp_db[i].DMAC[4],
					(0xFF)&arp_db[i].DMAC[5]
				);
				printf("\nsip: %02d:%02d:%02d:%02d ",
					(0xFF)&arp_db[i].arp_sip[0],
					(0xFF)&arp_db[i].arp_sip[1],
					(0xFF)&arp_db[i].arp_sip[2],
					(0xFF)&arp_db[i].arp_sip[3]
				);
				printf("dip: %02d:%02d:%02d:%02d \n\n",
					(0xFF)&arp_db[i].arp_dip[0],
					(0xFF)&arp_db[i].arp_dip[1],
					(0xFF)&arp_db[i].arp_dip[2],
					(0xFF)&arp_db[i].arp_dip[3]
				);
			}
}
