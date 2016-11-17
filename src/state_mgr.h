#include <time.h>
#define STATE_TYPE_NUM 14

typedef struct{
	char conn_state;
	char station[40];
	char IP[16];
	char host_name[50];
	char inactivetime[20];
	char rxbytes[20];
	char rxpackets[20];
	char txbytes[20];
	char txpackets[20];
	char txfailed[20];
	char signal[20];
	char txbitrate[20];
	char rxbitrate[20];
	char authorized[20];
	char authenticated[20];
	char WMMWME[20];
	char TDLSpeer[20];
	char connTime[15];
	char disconnTime[15];
}dev_state_t;
typedef struct{
	int check;
	int conn_dev_cont;
	char dev_states[1024];
}state_return_string_t;
typedef struct{
	char connTime[15];
	char MAC[40];
	char IP[16];
	char host_name[50];
}MAC_IP;
char *text_t[] = {
	"Station",
	"inactivetime:",
	"rxbytes:",
	"rxpackets:",
	"txbytes:",
	"txpackets:",
	"txfailed:",
	"signal:",
	"txbitrate:",
	"rxbitrate:",
	"authorized:",
	"authenticated:",
	"WMM/WME:",
	"TDLSpeer:"
};
int get_time(char *dev);
int retText(char *arg,dev_state_t *dev) {
	int i = 0;	
	char *ptr;
	for (i = 0; i <STATE_TYPE_NUM; i++)
	{
		if ((ptr =strstr(arg, text_t[i]))) {
//			printf("%s", ptr += strlen(text_t[i]));
			ptr += strlen(text_t[i]);

			switch(i)
			{
			case 0: snprintf((*dev).station,18, "%s", ptr); break;
			case 1: sprintf((*dev).inactivetime, "%s", ptr); break;			
			case 2: sprintf((*dev).rxbytes, "%s", ptr); break;
			case 3: sprintf((*dev).rxpackets, "%s", ptr); break;
			case 4: sprintf((*dev).txbytes, "%s", ptr); break;
			case 5: sprintf((*dev).txpackets, "%s", ptr); break;
			case 6: sprintf((*dev).txfailed, "%s", ptr); break;
			case 7: sprintf((*dev).signal, "%s", ptr); break;
			case 8: sprintf((*dev).txbitrate, "%s", ptr); break;
			case 9: sprintf((*dev).rxbitrate, "%s", ptr); break;
			case 10: sprintf((*dev).authorized, "%s", ptr); break;
			case 11: sprintf((*dev).authenticated, "%s", ptr); break;
			case 12: sprintf((*dev).WMMWME, "%s", ptr); break;
			case 13: sprintf((*dev).TDLSpeer, "%s", ptr); return 1; break;
			default: break;
			}
			break;
		}
	}
return 0;
}
int get_time(char *dev_time){
time_t current_time;
time( &current_time);
sprintf(dev_time,"%d",(int)current_time);
return 0;
}
