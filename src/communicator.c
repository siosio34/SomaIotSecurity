#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> //pthread_create(), pthread_join(), pthread_self()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <json.h>
#include "communicator.h"
#include "gateway_main.h"
#define BUFF_SIZE 1024
//특정 문자 삭제 "지우기용
char* getJsonObject(json_object*,char *);
void Eliminate(char *str, char ch); //쓰레드에서 사용할 함수
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
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == server_socket)
	{
		printf("server socket 생성 실패n");
		exit(1);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9990);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");// 127.0.0.1 localhost
	if (-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		printf("bind() 실행 에러n");
		exit(1);
	}
	while(1)
	{
        	//===========json receiver=========//
        	json_ID = 1; //update example
       		// char* example_SSID = "Pi3-AP"; //test data
       		// char* example_PW = "12345678";
		char page_name[20]="";
		char admin_pw[20]="";
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
		sprintf(page_name,"%s",getJsonObject(jobj,"page_name"));
		/*페이지이름에 따른  분기문*/
		if(strcmp(page_name,"login")==0)
		{
			printf("login\n");
			sprintf(admin_pw,"%s",getJsonObject(jobj,"admin_pw"));
		}
		/***************************/
		printf("received page_name= %s, PW= %s \n",page_name,admin_pw);
		//*****write******//
		char ans[6] ="";
		if(strcmp(admin_pw,"homeiot")==0)
		{
			sprintf(ans,"%s","{\"page_name\":\"login\", \"verify\":\"true\"}");		
		}
		else
		{
			sprintf(ans,"%s","{\"page_name\":\"login\", \"verify\":\"false\"}");		
		}
		//test data 실제 config 값으로 대체
		
		sprintf(buff_snd, "%s", buff_rcv);
		write(client_socket, buff_snd, strlen(buff_snd) + 1);
		// +1: NULL까지 포함해서 전송
		printf("\n%s\n",buff_rcv);
		close(client_socket);
		printf("	클라이언트 접속 종료   		\n");
		//==================================//
        	if(new_json == 1 && json_ID == 1)
		{ //received json data is for update
            	//write global struct
            	sprintf(inner_data.admin_PW,"%s",admin_pw);
            	//raise update flag
            	flag_update = 1;
        	}
      		// if(global_test==42)
       		//sleep(3); //delay for test
    	}
    return (void *)i;
}

void Eliminate(char *str, char ch) {
    for (; *str != '\0'; str++)//종료 문자를 만날 때까지 반복
    {
        if (*str == ch)//ch와 같은 문자일 때
        {
            strcpy(str, str + 1);
            str--;
        }
    }
}
char* getJsonObject(json_object *jobj, char *key) {
	struct json_object *jsontemp;
	json_object_object_get_ex(jobj, key, &jsontemp);
	char *str = json_object_to_json_string(jsontemp);
	Eliminate(str, '\"');
		return str;
}
