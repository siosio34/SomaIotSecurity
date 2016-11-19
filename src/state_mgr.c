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
void get_state(int wlan_i);
state_return_string_t* state_return_p=NULL;
dev_state_t dev[2][20];
int conn_dev_cnt[2]={0};
char command[2][35]={"sudo iw dev wlan0 station dump","sudo iw dev wlan1 station dump"};
int main()
{
	printf("상태관리자 실행 성공");
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
	while(1)
	{
		get_state(0);
		get_state(1);
		sleep(1);
	}
	return 0;
}
void get_state(int wlan_i){
	//printf("get state wlan %d~",wlan_i);
	int count = 0;
	FILE *fp = NULL;
	FILE *fp_dhcp = NULL;
	int i;
	int new_dev_flag=1;
	char dhcp_command[]="cat /var/lib/misc/dnsmasq.leases";
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
		dev_state_t temp_dev={0};
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
				for(i=0;i<conn_dev_cnt[wlan_i];i++)
						{

					if(strcmp(temp_dev.station,dev[wlan_i][i].station)==0)
								{
						if(dev[wlan_i][i].conn_state==0){(get_time(dev[wlan_i][i].connTime));}
						strcpy(temp_dev.connTime,dev[wlan_i][i].connTime);
						strcpy(temp_dev.flag,dev[wlan_i][i].flag);
						dev[wlan_i][i]=temp_dev;
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
					dev[wlan_i][conn_dev_cnt[wlan_i]]=temp_dev;
					//memcpy((dev[conn_dev_cnt]),temp_dev,sizeof(temp_dev));
					printf("%s",temp_dev.station);
					strcpy(dev[wlan_i][conn_dev_cnt[wlan_i]].station,temp_dev.station);					
					get_time(dev[wlan_i][conn_dev_cnt[wlan_i]].connTime);
					strcpy(dev[wlan_i][conn_dev_cnt[wlan_i]].flag,"1");
					dev_conn_check_flag[conn_dev_cnt[wlan_i]]=1;
					conn_dev_cnt[wlan_i]++;
				}
			}
		}
		/*접속 종료 기기 처리*/
		for(i=0;i<conn_dev_cnt[wlan_i];i++)
		{
			if(dev_conn_check_flag[i])
			{
			dev[wlan_i][i].conn_state=wlan_i==0?'1':'2';
			}
			else
			{
			dev[wlan_i][i].conn_state=0;
				//
			if(strlen(dev[wlan_i][i].disconnTime)==0)
				{
				get_time(dev[wlan_i][i].disconnTime);
				}
			}
			
			if(0){printf(
			"\nConneted:%c\nMAC:%s\nIP:%s\nHOST_NAME:%s\nrx:%stx:%sconnected:%s\ndisconnected:%s\n",
			dev[wlan_i][i].conn_state,dev[wlan_i][i].station,
			dev[wlan_i][i].IP,dev[wlan_i][i].host_name,
			dev[wlan_i][i].rxbytes,dev[wlan_i][i].txbytes,
			dev[wlan_i][i].connTime,dev[wlan_i][i].disconnTime);}
			
		}
	}

	pclose(fp);
	if(fp_dhcp){
		pclose(fp_dhcp);
		fp_dhcp=NULL;
	}
	sleep(1);
}
void sigusr_handler(int signo)
{
	
	struct json_object *array,*temp , *object[9];
       	array = json_object_new_array();
	printf("sigusr handler%d %d\n",conn_dev_cnt[0],conn_dev_cnt[1]);
	if(conn_dev_cnt>0){
	int i,j;
	for(j=0;j<2;j++)
	for(i=0; i<conn_dev_cnt[j];i++){
		char itoa_c[5];sprintf(itoa_c,"%d",i);
        	temp = json_object_new_object();
	      	object[0] = json_object_new_string(dev[j][i].conn_state);
	  	json_object_object_add(temp,itoa_c, object[0]);

		object[1] = json_object_new_string(dev[j][i].station);
		json_object_object_add(temp,"MAC", object[1]);
        	object[2] = json_object_new_string(dev[j][i].IP);
		json_object_object_add(temp,"IP", object[2]);

        	object[3] = json_object_new_string(dev[j][i].host_name);
		json_object_object_add(temp,"HOST_NAME", object[3]);

       		object[4] = json_object_new_string(dev[j][i].rxbytes);
		json_object_object_add(temp,"rx", object[4]);

        	object[5] = json_object_new_string(dev[j][i].txbytes);
 		json_object_object_add(temp,"tx", object[5]);

		object[6] = json_object_new_string(dev[j][i].connTime);
		json_object_object_add(temp,"connected", object[6]);

        	object[7] = json_object_new_string(dev[j][i].disconnTime);
		json_object_object_add(temp,"disconnected", object[7]);

        	object[8] = json_object_new_string(dev[j][i].flag);
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
