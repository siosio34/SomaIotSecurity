#define MAC_LEN 6
#define IP_LEN 4

typedef struct{
	char L2_DMAC[MAC_LEN];
	char L2_SMAC[MAC_LEN];
	char DMAC[MAC_LEN];
	char SMAC[MAC_LEN];
	char arp_dip[IP_LEN];
	char arp_sip[IP_LEN];
	//time timeout;
} arp_db_t;
	
