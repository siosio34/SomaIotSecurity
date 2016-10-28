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
				case 0: sprintf((*div).station,"%s",ptr); break;
				case 1:	sprintf((*div).inactivetime,"%s",ptr); break;
                                case 2:
                                case 3:
                                case 4:
                                case 5:
                                case 6:
                                case 7:
				default : break;
			}
			break;
		}
	}
	return ptr;
}
