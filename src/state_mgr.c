#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_edit.h"
#include "state_mgr.h"
#include <string.h>
#define MAX_DEVICE_NUM 200
int main()
{
	char command[]="sudo iw dev wlan0 station dump";	
	char dhcp_command[]="cat /var/lib/misc/dnsmasq.leases";
	int count = 0;
	FILE *fp;
	FILE *fp_dhcp;
    	int state;
	int i;
	int conn_dev_cnt=0;
	int new_dev_flag=1;
	dev_state dev[MAX_DEVICE_NUM]={};
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
		char buffer_dhcp[BUFFER_SIZE];
		dev_state temp_dev={};
		char dev_conn_check_flag[MAX_DEVICE_NUM]={0};
		/*접속 기기 처리*/
		while (fgets(buffer,BUFFER_SIZE,fp)!=NULL)
		{
			count++;
			Eliminate(buffer,' ');
			Eliminate(buffer,'\t');
			if(retText(buffer,&temp_dev)){ //마지막 줄을 찾았을때.
				MAC_IP mac_ip={};
				fp_dhcp=popen(dhcp_command,"r");
                                while(fgets(buffer_dhcp,BUFFER_SIZE,fp_dhcp)!=NULL)
                                {
					sscanf(buffer_dhcp,"%s %s %s %s",mac_ip.connTime,mac_ip.MAC,mac_ip.IP,mac_ip.host_name);
					 if(strcmp(temp_dev.station,mac_ip.MAC)==0)
                                        {
                                        	strcpy(temp_dev.IP,mac_ip.IP);
                                        	strcpy(temp_dev.host_name,mac_ip.host_name);
                                	}
                                }
				for(i=0;i<conn_dev_cnt;i++)
        	        	{
	              	        	if(strcmp(temp_dev.station,dev[i].station)==0)
                        		{
						char timestp[30]={};
						if(dev[i].conn_state=='0')get_time(dev[i].connTime);
						strcpy(temp_dev.connTime,dev[i].connTime);
                                		dev[i]=temp_dev;
						new_dev_flag=0;
						dev_conn_check_flag[i]=1;
                        			break;
                        		}
					//couldn't found div MAC
					new_dev_flag=1;
                		}
				if(new_dev_flag)
				{

					printf("\n!!!!!!!!!!!!!!!!!!!!!new divice connected!!!!!!!!!!!\n\n");
					dev[conn_dev_cnt]=temp_dev;
					printf("\n\n%s\n\n\n",dev[conn_dev_cnt].station);
					get_time(dev[conn_dev_cnt].connTime);
					dev_conn_check_flag[conn_dev_cnt]=1;
					conn_dev_cnt++;
				}
			}
		}
		/*접속 종료 기기 처리*/
		for(i=0;i<conn_dev_cnt;i++)
		{
			if(dev_conn_check_flag[i])
			{
			dev[i].conn_state='1';
			}
			else
			{
			dev[i].conn_state='0';
			if(strlen(dev[i].disconnTime)==0)get_time(dev[i].disconnTime);
			}
			printf("\nConneted:%c\nMAC:%s\nIP:%s\nHOST_NAME:%s\nrx:%stx:%sconnected:%s\ndisconnected:%s\n",dev[i].conn_state,dev[i].station,dev[i].IP,dev[i].host_name,dev[i].rxbytes,dev[i].txbytes,dev[i].connTime,dev[i].disconnTime);

		}
	}
	state= pclose(fp);
	sleep(1);
	}
	return 0;
}

