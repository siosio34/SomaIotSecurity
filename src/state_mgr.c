#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include "string_edit.h"
#include "state_mgr.h"
#include "unistd.h"
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <string.h>
#include <signal.h>
#define MAX_DEVICE_NUM 200
#define SHMEMKEY 1000
void sigusr_handler(int signo);
state_return_string_t* state_return_p;

dev_state_t dev[20];
int conn_dev_cnt=0;

	char command[2][35]={"sudo iw dev wlan0 station dump","sudo iw dev wlan1 station dump"};
int main()
{
	//sigusr_handler(1);
	char dhcp_command[]="cat /var/lib/misc/dnsmasq.leases";
	get_state(0);
	get_state(1);

	/*****공용 메모리*****/
	int shmem_id;
	shmem_id= shmget((key_t)SHMEMKEY,sizeof(state_return_string_t),0666);
	signal(SIGUSR1,sigusr_handler);
	if(shmem_id ==-1)
	{
		perror("shmget ERROR");
		exit(1);
	}
	 state_return_p =(state_return_string_t *)shmat(shmem_id,NULL,0);
	
	return 0;
}
void get_state(int wlan_i){
		int count = 0;
		FILE *fp;
		FILE *fp_dhcp;
		int i;
		int new_dev_flag=1;
while(1){
	fp = popen(command[wlan_i], "r");
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
						strcpy(temp_dev.flag,dev[i].flag);
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
					strcpy(dev[conn_dev_cnt].flag,"1");
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
			dev[i].conn_state=wlan_i==0?'1':'2';
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
			
			if(0){printf(
			"\nConneted:%c\nMAC:%s\nIP:%s\nHOST_NAME:%s\nrx:%stx:%sconnected:%s\ndisconnected:%s\n",
			dev[i].conn_state,dev[i].station,
			dev[i].IP,dev[i].host_name,
			dev[i].rxbytes,dev[i].txbytes,
			dev[i].connTime,dev[i].disconnTime);}
			
		}
	}

	pclose(fp);
	if(fp_dhcp)
	pclose(fp_dhcp);
	fp_dhcp=NULL;
	sleep(1);
	}
}
void sigusr_handler(int signo)
{
	
	struct json_object *array,*temp , *object[9];
       	array = json_object_new_array();
	printf("sigusr handler%d\n",conn_dev_cnt);
	if(conn_dev_cnt>0){
	int i;
	for(i=0; i<conn_dev_cnt;i++){
		char itoa_c[5];sprintf(itoa_c,"%d",i);
        	temp = json_object_new_object();
		printf("%s",dev[i].station);
	      	object[0] = json_object_new_string(dev[i].conn_state=='1'?"true":"false");
	  	json_object_object_add(temp,itoa_c, object[0]);

		object[1] = json_object_new_string(dev[i].station);
		json_object_object_add(temp,"MAC", object[1]);
        	object[2] = json_object_new_string(dev[i].IP);
		json_object_object_add(temp,"IP", object[2]);

        	object[3] = json_object_new_string(dev[i].host_name);
		json_object_object_add(temp,"HOST_NAME", object[3]);

       		object[4] = json_object_new_string(dev[i].rxbytes);
		json_object_object_add(temp,"rx", object[4]);

        	object[5] = json_object_new_string(dev[i].txbytes);
 		json_object_object_add(temp,"tx", object[5]);

		object[6] = json_object_new_string(dev[i].connTime);
		json_object_object_add(temp,"connected", object[6]);

        	object[7] = json_object_new_string(dev[i].disconnTime);
		json_object_object_add(temp,"disconnected", object[7]);

        	object[8] = json_object_new_string(dev[i].flag);
		json_object_object_add(temp,"flag", object[8]);
		json_object_array_add(array,temp);
	}
	 json_object * jobj = json_object_new_object();
	json_object *con_list = json_object_new_string("con_list");
	json_object_object_add(jobj,"page_name", con_list);
	json_object_object_add(jobj,"con_list",array);


	while(!state_return_p->check)
	{
		printf("waiting_state_mgr");
	}
	sprintf(state_return_p->dev_states,json_object_to_json_string(jobj));
	//
	printf("%s",state_return_p->dev_states);
	
	}//1개이상인경우
	else{//연결된 디바이스가 없는경우
		strcpy(state_return_p->dev_states,"{\"page_name\":\"con_list\",\"con_list\":[]}");
	}

	state_return_p->check=0;
}
