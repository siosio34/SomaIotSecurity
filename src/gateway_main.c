#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <wiringPi.h>
#include <signal.h>
#include "gateway_main.h"
#include "communicator.h"
#include "write_conf.h"
#include "lcd.h"
#include "otp.h"
char command1[100]={0};

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
   
    // update_flag.otp_conf = 1;
    signal(SIGCHLD, SIG_IGN);
    update_flag.hostapd = 1;
    init_hostapd_conf();
    init_service();
    sleep(1);
    restart_lcd(); //cut power to lcd and erase lcd register
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
    printf("hi");
    otp_init();
    update_flag.otp_enable = 1; //for debug
    update_flag.otp_easyword = 1;
    update_flag.hostapd = 0;
    //main loop for management demon
    while(1){
        write_conf();
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

void init_hostapd_conf(){
    //for debug
    printf("SSID= %s\n", inner_data.local_SSID);

    //update local hostapd .conf (unactivated)
    sprintf(command1, "./script/update_hostapd_conf.sh %s %s", inner_data.local_SSID, inner_data.local_PW);
    system(command1); //update conf by script

    //update guest hostapd .conf (unactivated)
    sprintf(command1, "./script/update_hostapd_conf_1.sh %s %s", inner_data.guest_SSID, inner_data.guest_PW);
    system(command1); //update conf by script

    //restart hostapd (unactivated)
    // sprintf(command, "./restart_hostapd.sh")
    // system(command); //update conf by script
    sprintf(lcd_data.row[0], "SSID: %s", inner_data.guest_SSID);
    sprintf(lcd_data.row[1], "PW: %s", inner_data.guest_PW);
    update_flag.lcd = 1;
}

