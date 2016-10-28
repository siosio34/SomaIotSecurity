#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_edit.h"
#include "state_mgr.h"
#include <string.h>
int main()
{
	char command[]="sudo iw dev wlan0 station dump";	
	int count = 0;
	FILE *fp;
    	int state;
	fp = popen(command, "r");
	if(!fp)
	{
		printf( "커멘드 실행에 실패했습니다." );
	}
	else
	{
		const size_t BUFFER_SIZE = 128;
		char buffer[BUFFER_SIZE];
		div_state div={};
		while (fgets(buffer,BUFFER_SIZE,fp)!=NULL)
		{
			count++;
			Eliminate(buffer, ' ');
			retText(buffer,&div);
		}
	}
	state= pclose(fp);
	return 0;
}

