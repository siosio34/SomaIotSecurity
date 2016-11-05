#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <wiringPi.h>
#include "gateway_main.h"
#include "communicator.h"
#include "write_conf.h"
#include "lcd.h"
#include "otp.h"


int flag_update = 0;
struct pharsed_data internal_data;
struct main_data inner_data;
struct main_data backup_data;
struct update_flags update_flag;
struct lcd_struct lcd_data;

int main(){

    // char command[100] = {0};
    // char SSID[50] = {0};
    // char passphrase[50] = {0};

    pthread_t p_thread[2]; //thread ID 저장할 변수
    int thr_id; //thread generation error check
    int status; //thread 종료시 반환하는 값 저장 변수
    int a = 1; //쓰레드 함수 인자

    //================init=====================//
    wiringPiSetup();        //init WiringPi

    init_struct();
    // update_flag.hostapd = 1;
    // update_flag.otp_conf = 1;
//    write_conf();
    signal(SIGCHLD, SIG_IGN);
    init_service();
    sleep(1);
    restart_lcd(); //cut power to lcd and erase lcd register
    //thread1 generation
    // thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)&a);
    // if(thr_id < 0){
    //         perror("thread create error : ");
    //          exit(0);
    //  }


    //lcd thread generation
    thr_id = pthread_create(&p_thread[1], NULL, lcd_update, (void *)&a);
    if(thr_id < 0){
            perror("thread create error : ");
            exit(0);
    }
    printf("hi");
    otp_init();
    update_flag.otp_enable = 1; //for debug

    //main loop for management demon
    while(1){
        write_conf();
        update_flag.otp_easyword = 1;
        otp();
        sleep(1);
  //  printf("%s\n", inner_data.guest_PW);
  //  printf("===================\n");

    }

    //========================================//
    //thread1 종료처리
    pthread_join(p_thread[0], (void **)&status);
    //printf("return thread 0 %d\n", status);

    //lcd thread 종료처리
    pthread_join(p_thread[1], (void **)&status);
    //printf("return thread 0 %d\n", status);

    return 0;

}

