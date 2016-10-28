#define STATE_TYPE_NUM 14

struct conndiv_state {
	char station[20];
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
};
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
char* retText(char* arg) {
	int i = 0;
	char *ptr;
	for (i = 0; i < 
STATE_TYPE_NUM; i++)
	{
		if ((ptr = 
strstr(arg, text_t[i]))) {
			//printf("%s\n", ptr += strlen(text_t[i]));
			break;
		}
	}
	return ptr;
}
