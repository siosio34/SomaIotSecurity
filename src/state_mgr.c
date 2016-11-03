#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_edit.h"
#include "state_mgr.h"
#include <string.h>
#include <time.h>
int main()
{
	char command[]="sudo iw dev wlan0 station dump";	
	int count = 0;
	FILE *fp;
    	int state;
	int i;
	int conn_div_cnt=1;
	int new_div_flag;
	div_state div[200]={};
	time_t   current_time;
	while(1){
	fp = popen(command, "r");
	if(!fp)
	{
		printf( "커멘드 실행에 실패했습니다." );
	}
	else
	{
		const size_t BUFFER_SIZE = 128;
		char buffer[BUFFER_SIZE];
		div_state temp_div={};
		while (fgets(buffer,BUFFER_SIZE,fp)!=NULL)
		{
			count++;
			Eliminate(buffer, ' ');
			if(retText(buffer,&temp_div)){ //마지막 줄을 찾았을때.
	                	for(i=0;i<conn_div_cnt;i++)
        	        	{
                	        	if(strcmp(temp_div.station,div[i].station)==0)
                        		{	char timestp[20]={};
						strcpy(timestp,div[i].connTime);
                                		div[i]=temp_div;
						strcpy(div[i].connTime,timestp);

                                		//printf("%s",div[i].station);
						new_div_flag=0;
                        			break;
                        		}
					new_div_flag=1;
					printf("MAC:%s, TX:%s, RX:%s",div[i].station,div[i].rxbytes,div[i].txbytes);
                		}
				if(new_div_flag)
				{

					time( &current_time);
					printf( "%ldn", current_time);
					printf( ctime( &current_time));
					printf("new divice connected\n");
					conn_div_cnt++;
					div[conn_div_cnt]=temp_div;
					strcpy(div[conn_div_cnt].connTime,ctime(&current_time));
				}
			}
		}
	}
	state= pclose(fp);
	sleep(1);
	}
	return 0;
}

