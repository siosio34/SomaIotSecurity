#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> //pthread_create(), pthread_join(), pthread_self()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <json-c/json.h>    //제이슨 파서 라이브러리
#include <sys/shm.h> //공유메모리 라이브러리
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>
#include "communicator.h"
#include "state_mgr.h"
#include "gateway_main.h"
#include "string_edit.h"
#define BUFF_SIZE 1024
#define SHMEMKEY 1000
#define MAX_DEV_NUM 200
#define MAC_LEN 18
//특정 문자 삭제 "지우기용
char* getJsonObject(json_object*,char *);
void ban_user(char *);
void unban_user(char *);
void init_state_mgr();
void sigusr1_handler(int signo);
void sigusr2_handler(int signo);
struct ban_list_t
{
int ban_cnt;
char ban_list[50][MAC_LEN];
};
struct ban_list_t ban_list={0};
state_return_string_t* state_return_p=NULL;

//void Eliminate(char *str, char ch); //쓰레드에서 사용할 함수
int state_mgr_PID;
void *t_function(void *data) {
	int id;
	int i = 999;
	int json_ID;
	int new_json = 1;
	// id = *((int *)data);
	int server_socket;
	int client_socket;
	int client_addr_size;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char buff_rcv[BUFF_SIZE + 5];
	char buff_snd[BUFF_SIZE + 5];
	struct json_object *jobj;
	/*shared_memory val */
	void * shared_memory=NULL;
	state_return_string_t state_return_string;
	int shmem_id;
	shmem_id= shmget((key_t)SHMEMKEY,sizeof(state_return_string_t),0666|IPC_CREAT);
	if(shmem_id==-1){printf("Shmeget ERROR2 err=%d %s\n",errno, strerror(errno)); exit(1);}
	else{printf("%d\n",shmem_id);}

	state_return_p=(state_return_string_t*)shmat(shmem_id, (void *)0, 0);
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	state_return_p->check=1;
	signal(SIGUSR1,sigusr1_handler);
	signal(SIGUSR2,sigusr2_handler);
  	int sockopt = 1;
	   	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1) {
			  	perror("socket setting failed");
				exit(EXIT_FAILURE);
		}
	if (-1 == server_socket)
	{
		printf("server socket 생성 실패n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9090);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");// 127.0.0.1 localhost
	if (-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		printf("bind() 실행 에러n");
		exit(1);
	}
	printf("웹서버 통신 스래드 실행 \n");
	init_state_mgr();
	printf("상태관리자 실행 PID:%d\n",state_mgr_PID);
	while(1)
	{
			//===========json receiver=========//
			json_ID = 1; //update example
	   		// char* example_SSID = "Pi3-AP"; //test data
	   		// char* example_PW = "12345678";
		char page_name[20]="";
		char admin_pw[20]="";
		char request_id[20]="";
		char ans[2024] ="";
		if (-1 == listen(server_socket, 5))
		{
			printf("대기상태 모드 설정 실패n");
			exit(1);
  		}
		printf("ready to listen\n");
		client_addr_size = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr,&client_addr_size);
		if (-1 == client_socket)
		{
			printf("클라이언트 연결 수락 실패n");
			exit(1);
		}
		//*******read******//
		int n = 0;
		n = read(client_socket, buff_rcv, BUFF_SIZE);
		if (n < 0) {
			printf("CRITICAL ERROR\n");
		}

		buff_rcv[n] = '\0';
		jobj =json_tokener_parse(buff_rcv);
		printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj,JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
		printf("receive: %s\n", buff_rcv);
		sprintf(inner_data.admin_PW,"%s","homeiot");
		sprintf(page_name,"%s",getJsonObject(jobj,"page_name"));
		/*페이지이름에 따른  분기문*/
		if(strcmp(page_name,"request")==0)
		{
			sprintf(request_id,"%s",getJsonObject(jobj,"request_id"));
			printf("request := %s",request_id);
			if(strcmp(request_id,"ssid")==0)
			{
				printf("send ssid!");
				sprintf(ans,"{\"page_name\":\"ssid\", \"local_ssid\":\"%s\",\"guest_ssid\":\"%s\",\"otp_enable\":\"%s\"}",inner_data.local_SSID,inner_data.guest_SSID,update_flag.otp_enable?"true":"false");
			}
			else if(strcmp(request_id,"con_list")==0)
			{		update_flag.warning=0;
				printf("send conlist!");
				kill(state_mgr_PID, SIGUSR1);
				while( state_return_p->check) {
					sleep(0.3);
					printf("waiting...\n");
				}
				sprintf(ans,state_return_p->dev_states);
				//sprintf(ans,"{\"page_name\":\"con_list\",\"con_list\":[{\"1\":\"true\",\"MAC\":\"c8:14:79:e8:3e:15\",\"IP\":\"172.24.1.113\",\"HOST_NAME\":\"android-ebff699db65b334b\",\"rx\":\"96932\",\"tx\":\"225657\",\"connected\":\"1478777438\",\"disconnected\":\"0\"},{\"2\":\"true\",\"MAC\":\"00:04:79:e8:3e:15\",\"IP\":\"172.24.1.113\",\"HOST_NAME\":\"KIMDONGWOO\",\"rx\":\"96932\",\"tx\":\"225657\",\"connected\":\"1478777438\",\"disconnected\":\"0\"}]}");
				state_return_p->check=1;
			}
		}
		else if(strcmp(page_name,"login")==0)
		{
			printf("login\n");
			sprintf(admin_pw,"%s",getJsonObject(jobj,"admin_pw"));
			if(strcmp(admin_pw,inner_data.admin_PW)==0)
					{
							sprintf(ans,"%s","{\"page_name\":\"login\", \"verify\":\"true\"}");
			   		}
			   		else
			   	 	{
							sprintf(ans,"%s","{\"page_name\":\"login\", \"verify\":\"false\"}");
					}

		}
		else if(strcmp(page_name,"ssid")==0)
		{
			printf("NEWssid\n");
			char temp[20]={0};
			strcpy(temp,getJsonObject(jobj,"local_ssid"));
			if(strlen(temp)>2&&strcmp(temp,"null")!=0){
			sprintf(inner_data.local_SSID,"%s",getJsonObject(jobj,"local_ssid"));printf("%s",temp);}
			
                        strcpy(temp,getJsonObject(jobj,"local_pw"));
                        if(strlen(temp)>7&&strcmp(temp,"null")!=0){
			sprintf(inner_data.local_PW,"%s",getJsonObject(jobj,"local_PW"));printf("%s",temp);}
			
                        strcpy(temp,getJsonObject(jobj,"guest_ssid"));
                        if(strlen(temp)>2&&strcmp(temp,"null")!=0){
			sprintf(inner_data.guest_SSID,"%s",getJsonObject(jobj,"guest_ssid"));printf("%s",temp);}
			
                        strcpy(temp,getJsonObject(jobj,"edit_guest_pass"));
                        if(strlen(temp)>7&&strcmp(temp,"null")!=0)
			sprintf(inner_data.guest_PW,"%s",getJsonObject(jobj,"edit_guest_pass"));
			
                        strcpy(temp,getJsonObject(jobj,"admin_pw"));
                        if(strlen(temp)>7&&strcmp(temp,"null")!=0)
			sprintf(inner_data.admin_PW,"%s",getJsonObject(jobj,"admin_pw"));
			update_flag.hostapd=1; //변경사항 설정
			printf("hostapd flag:%d\n",update_flag.hostapd);
		}
		else if(strcmp(page_name,"ban")==0)
		{
			if(strcmp(getJsonObject(jobj,"ban_check"),"0")==0)
			ban_user(getJsonObject(jobj,"mac"));
			else
			unban_user(getJsonObject(jobj,"mac"));
		}
		/***************************/
		//*****write******//
		//test data 실제 config 값으로 대체

		sprintf(buff_snd, "%s", ans);
		write(client_socket, buff_snd, strlen(buff_snd) );
		// +1: NULL까지 포함해서 전송
		printf("\nsend: %s",buff_snd);
		printf("\n%s\n",buff_rcv);
		close(client_socket);
		printf("	클라이언트 접속 종료   		\n");
		//==================================//
		}
	return (void *)i;
}

void init_arp_mgr(){

    state_mgr_PID = fork();
    if (state_mgr_PID > 0){
    }
    else if (state_mgr_PID == 0){

        char *argv[]   = { "/home/pi/SomaIotSecurity/src/raw_socket/raw_socket",NULL};
        execv( "/home/pi/SomaIotSecurity/src/raw_socket/raw_socket", argv);
    }
}

void init_state_mgr(){

    state_mgr_PID = fork();
    if (state_mgr_PID > 0){
    }
    else if (state_mgr_PID == 0){

        char *argv[]   = { "/home/pi/SomaIotSecurity/src/state_mgr",NULL};
        execv( "/home/pi/SomaIotSecurity/src/state_mgr", argv);
    }
}
char* getJsonObject(json_object *jobj, char *key) {
	struct json_object *jsontemp;
//	char str[100];
	json_object_object_get_ex(jobj, key, &jsontemp);
//	snprintf(str, 100, "%s", json_object_to_json_string(jsontemp));
	char *str = json_object_to_json_string(jsontemp);
	Eliminate(str, '\"');
	return str;
}
void init()//저장되어있는 벤목록을 로드
{
FILE *fr=NULL;
  fr=fopen("backup_banlist.txt","r");
   int i=0;
        if(fr!=NULL){
        while(fscanf(fr, "%s",ban_list.ban_list[ban_list.ban_cnt])!=EOF)
        ban_list.ban_cnt++;
	}

}
void ban_user(char* MAC)
{
	char command[70];
	printf("ban_user %s\n",MAC);
	int i;
	int found=0;
	for(i=0;i<ban_list.ban_cnt;i++)
	{
		if(memcmp(ban_list.ban_list[i],MAC,6)==0)// 이미 리스트에 들어잇음.
		{
			found=1;
			break;
		}
	}
	if(!found){
	sprintf(command,"iptables -I FORWARD -m mac --mac-source %s -j DROP",MAC);
	system(command);
	memcpy(ban_list.ban_list[ban_list.ban_cnt++],MAC,6);
	
	kill(state_mgr_PID, SIGUSR2);
	strcpy(state_return_p->MAC,MAC);
	printf("banuser %s!!!!!!!! \n",state_return_p->MAC);
	while( state_return_p->check) {
                      sleep(0.3);
                    }
        state_return_p->check=1;
	}
}


void unban_user(char* MAC)
{
        char command[70];
        printf("ban_user %s\n",MAC);
        int i;
        int found=0;
        for(i=0;i<ban_list.ban_cnt;i++)
        {
                if(strcmp(ban_list.ban_list[i],MAC)==0)// 이미 리스트에 들어잇음.
                {
                        found=1;
                        break;
                }
        }
        if(!found){
        sprintf(command,"iptables -D FORWARD -m mac --mac-source %s -j DROP",MAC);
        system(command);
        strcpy(ban_list.ban_list[ban_list.ban_cnt--],MAC); 
	kill(state_mgr_PID, SIGUSR2);
	        strcpy(state_return_p->MAC,MAC);
		printf("unbanuser %s!!!!!!!! \n",state_return_p->MAC);
        while( state_return_p->check) {
                      sleep(0.3);
                    }
        state_return_p->check=1;
	}
}

void sigusr1_handler(int signo)
{
                    update_flag.warning=2;
                    state_return_p->conn_dev_cont=0;
}
void sigusr2_handler(int signo)
{
		printf("get sig2");
		update_flag.warning=1;
}
