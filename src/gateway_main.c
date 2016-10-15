#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "gateway_main.h"
#include "communicator.h"
#include "write_conf.h"
#include "lcd.h"

volatile int global_test = 42;
volatile int flag_update = 0;
volatile struct pharsed_data internal_data;
volatile struct main_data inner_data;
volatile struct update_flags update_flag;
volatile struct lcd_struct lcd_data;

int main(){

    char command[100] = {0};
    char SSID[50] = {0};
    char passphrase[50] = {0};

    pthread_t p_thread[2]; //thread ID 저장할 변수
    int thr_id; //thread generation error check
    int status; //thread 종료시 반환하는 값 저장 변수
    int a = 1; //쓰레드 함수 인자


    //thread1 generation
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)&a);
    if(thr_id < 0){
            perror("thread create error : ");
            exit(0);
    }

    //lcd thread generation
    thr_id = pthread_create(&p_thread[1], NULL, lcd_update, (void *)&a);
    if(thr_id < 0){
            perror("thread create error : ");
            exit(0);
    }

    //main loop for management demon
    while(1){

    write_conf();


    }

    //===========================//
    //thread1 종료처리
    pthread_join(p_thread[0], (void **)&status);
    //printf("return thread 0 %d\n", status);

    //lcd thread 종료처리
    pthread_join(p_thread[1], (void **)&status);
    //printf("return thread 0 %d\n", status);

    return 0;

}




