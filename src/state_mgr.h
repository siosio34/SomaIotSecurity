#define STATE_TYPE_NUM 14

typedef struct{
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
}div_state;
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

char* retText(char *arg,div_state *div) {
	int i = 0;
	char *ptr;
	for (i = 0; i <STATE_TYPE_NUM; i++)
	{
		if ((ptr =strstr(arg, text_t[i]))) {
			printf("%s", ptr += strlen(text_t[i]));
			switch(i)
			{
			case 0: sprintf((*div).station, "%s", ptr); break;
			case 1: sprintf((*div).inactivetime, "%s", ptr); break;
			case 2: sprintf((*div).rxbytes, "%s", ptr); break;
			case 3: sprintf((*div).rxpackets, "%s", ptr); break;
			case 4: sprintf((*div).txbytes, "%s", ptr); break;
			case 5: sprintf((*div).txpackets, "%s", ptr); break;
			case 6: sprintf((*div).txfailed, "%s", ptr); break;
			case 7: sprintf((*div).signal, "%s", ptr); break;
			case 8: sprintf((*div).txbitrate, "%s", ptr); break;
			case 9: sprintf((*div).rxbitrate, "%s", ptr); break;
			case 10: sprintf((*div).authorized, "%s", ptr); break;
			case 11: sprintf((*div).authenticated, "%s", ptr); break;
			case 12: sprintf((*div).WMMWME, "%s", ptr); break;
			case 13: sprintf((*div).TDLSpeer, "%s", ptr); break;
			default: break;
			}
			break;
		}
	}
	return ptr;
}
