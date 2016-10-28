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
//    wiringPiSetup();        //init WiringPi
//    restart_lcd(); //cut power to lcd and erase lcd register
//    init_struct();
    // update_flag.hostapd = 1;
    // update_flag.otp_conf = 1;
//    write_conf();
//    init_service();

    //thread1 generation
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)&a);
    if(thr_id < 0){
            perror("thread create error : ");
             exit(0);
     }

    //// lcd thread generation
    // thr_id = pthread_create(&p_thread[1], NULL, lcd_update, (void *)&a);
    // if(thr_id < 0){
    //         perror("thread create error : ");
    //         exit(0);
    // }

    //otp_init();
    //lcd thread generation
    thr_id = pthread_create(&p_thread[1], NULL, lcd_update, (void *)&a);
    if(thr_id < 0){
            perror("thread create error : ");
            exit(0);
    }
    printf("hi");
    //otp_init();
    update_flag.otp_enable = 1; //for debug

    //main loop for management demon
    while(0){
     //write_conf();
   // otp();
   // sleep(1);
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
void init_struct(){

    //read struct from backup file(backup.txt)
    FILE *fr;
    fr=fopen("backup.txt","r");
    fscanf(fr,"%s %s %s %s",&inner_data.local_SSID, &inner_data.local_PW, &inner_data.guest_SSID, &inner_data.guest_PW);
    fclose(fr);

}

void init_service(){
    //run hostapd
    init_hostapd();

}

void init_hostapd(){
    system("sudo /usr/sbin/hostapd /etc/hostapd/hostapd.conf -f /var/log/hostapd_local.log &");
    // system("disown");
    system("sudo /usr/sbin/hostapd /etc/hostapd/hostapd_1.conf -f /var/log/hostapd_guest.log &");
    // system("disown");
}

void backup_struct(){
    //save current struct
    FILE *fw;
    fw=fopen("backup.txt","w");
    fprintf(fw,"%s %s %s %s\n",inner_data.local_SSID, inner_data.local_PW, inner_data.guest_SSID, inner_data.guest_PW);
    fclose(fw);
}

void restart_lcd(){
    const int LCD_POW_PIN = 2; //wiringPi pin 7
    pullUpDnControl(LCD_POW_PIN, PUD_DOWN); //pull-down switch pin
    pinMode(LCD_POW_PIN, OUTPUT);
    digitalWrite(LCD_POW_PIN, HIGH);
    sleep(1);
    digitalWrite(LCD_POW_PIN, LOW);

}
