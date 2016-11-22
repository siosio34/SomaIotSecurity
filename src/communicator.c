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
#include "communicator.h"
#include "state_mgr.h"
#include "gateway_main.h"
#include "string_edit.h"
#define BUFF_SIZE 1024
#define SHMEMKEY 1000
#define MAX_DEV_NUM 200
//특정 문자 삭제 "지우기용
char* getJsonObject(json_object*,char *);

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
	state_return_string_t* state_return_p;
	int shmem_id;
	shmem_id= shmget((key_t)SHMEMKEY,sizeof(state_return_string_t),0666|IPC_CREAT);
	if(shmem_id==-1){printf("Shmeget ERROR\n"); exit(1);}
	else{printf("%d\n",shmem_id);}

	state_return_p=(state_return_string_t*)shmat(shmem_id, (void *)0, 0); 
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	state_return_p->check=1;
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
		buff_rcv[n] = NULL;
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
			{
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
			sprintf(inner_data.local_SSID,"%s",getJsonObject(jobj,"local_ssid"));
			sprintf(inner_data.local_PW,"%s",getJsonObject(jobj,"local_PW"));
			sprintf(inner_data.guest_PW,"%s",getJsonObject(jobj,"guest_ssid"));
			sprintf(inner_data.guest_SSID,"%s",getJsonObject(jobj,"guest_pw"));
			sprintf(inner_data.admin_PW,"%s",getJsonObject(jobj,"admin_pw"));
			update_flag.hostapd=1; //변경사항 설정
		}
		/***************************/
		//*****write******//
		//test data 실제 config 값으로 대체

		sprintf(buff_snd, "%s", ans);
		write(client_socket, buff_snd, strlen(buff_snd) + 1);
		// +1: NULL까지 포함해서 전송
		printf("\nsend: %s",buff_snd);
		printf("\n%s\n",buff_rcv);
		sleep(1);
		close(client_socket);
		printf("	클라이언트 접속 종료   		\n");
		//==================================//
		}
	return (void *)i;
}


void init_state_mgr(){

    state_mgr_PID = fork();
    if (state_mgr_PID > 0){
        printf("부모 프로세스 %d : %d\n", getpid(), state_mgr_PID); //for debug
    }
    else if (state_mgr_PID == 0){
        printf("자식 프로세스 %d\n", getpid());//for debug

        char *argv[]   = { "/home/pi/SomaIotSecurity/src/state_mgr",NULL};
        execv( "/home/pi/SomaIotSecurity/src/state_mgr", argv);
    }
}
char* getJsonObject(json_object *jobj, char *key) {
	struct json_object *jsontemp;
	json_object_object_get_ex(jobj, key, &jsontemp);
	char *str = json_object_to_json_string(jsontemp);
	Eliminate(str, '\"');
		return str;
}
