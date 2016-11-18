#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include "string_edit.h"
#include "state_mgr.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>
#define MAX_DEVICE_NUM 200
#define SHMEMKEY 1000
void sigusr_handler(int signo);
state_return_string_t* state_return_p;

int main()
{
	printf("afdsf");
	printf("Shmat ERROR");
	char command[]="sudo iw dev wlan0 station dump";	
	char dhcp_command[]="cat /var/lib/misc/dnsmasq.leases";
	int count = 0;
	FILE *fp;
	FILE *fp_dhcp;
	int i;
	int conn_dev_cnt=0;
	int new_dev_flag=1;
	dev_state_t dev[200];
	/*****공용 메모리*****/
	int shmem_id;
	shmem_id= shmget((key_t)SHMEMKEY,sizeof(state_return_string_t),0666);
	signal(SIGUSR1,sigusr_handler);
	if(shmem_id ==-1)
	{
		perror("shmget ERROR");
		exit(1);
	}
	if(dev==(void *)-1)
	{
		perror("shmat ERROR");
		exit(1);
	}
	  state_return_p =(state_return_string_t *)shmat(shmem_id,NULL,0);
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
		char dev_conn_check_flag[MAX_DEVICE_NUM]={0};
		/*접속 기기 처리*/
		dev_state_t temp_dev={};
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
						if(dev[i].conn_state=='\0'){(get_time(dev[i].connTime));}
						strcpy(temp_dev.connTime,dev[i].connTime);
						dev[i]=temp_dev;
						//memcpy((dev->dev_states[i]),temp_dev,sizeof(temp_dev));
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
					//memcpy((dev[conn_dev_cnt]),temp_dev,sizeof(temp_dev));
					printf("%s",temp_dev.station);
					strcpy(dev[conn_dev_cnt].station,temp_dev.station);
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
				//
			if(strlen(dev[i].disconnTime)==0)
				{
				get_time(dev[i].disconnTime);
				}
			}
			printf(
			"\nConneted:%c\nMAC:%s\nIP:%s\nHOST_NAME:%s\nrx:%stx:%sconnected:%s\ndisconnected:%s\n",
			dev[i].conn_state,dev[i].station,
			dev[i].IP,dev[i].host_name,
			dev[i].rxbytes,dev[i].txbytes,
			dev[i].connTime,dev[i].disconnTime);
			
		}
	}
	if(shmdt(state_return_p) == -1)//detach from shared memory
	{
		perror("shmdt failed");
		exit(1);
	}

	pclose(fp);
	pclose(fp_dhcp);
	sleep(1);
	}
	return 0;
}

void sigusr_handler(int signo)
{
	while(!state_return_p->check)
	{
		printf("waiting");
	}
	
	strcpy(state_return_p->dev_states,"{\"page_name\":\"con_list\",\"con_list\":[{\"1\":\"true\",\"MAC\":\"c8:14:79:e8:3e:15\",\"IP\":\"172.24.1.113\",\"HOST_NAME\":\"android-ebff699db65b334b\",\"rx\":\"96932\",\"tx\":\"225657\",\"connected\":\"1478777438\",\"disconnected\":\"0\"},{\"2\":\"true\",\"MAC\":\"00:04:79:e8:3e:15\",\"IP\":\"172.24.1.113\",\"HOST_NAME\":\"KIMDONGWOO\",\"rx\":\"96932\",\"tx\":\"225657\",\"connected\":\"1478777438\",\"disconnected\":\"0\"}]}");
	state_return_p->check=0;
}
