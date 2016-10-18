#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>
#include "otp.h"
#include "lcd.h"
#include "gateway_main.h"

const int OTP_SW_PIN = 7; //wiringPi pin 7
char new_otp[16] = {0};
char old_otp[16] = {0};

void otp_init(){
    //otp HW switch init
    pinMode (OTP_SW_PIN, INPUT) ;
    pullUpDnControl(OTP_SW_PIN, PUD_UP); //pull-up switch pin
    wiringPiISR (OTP_SW_PIN, INT_EDGE_FALLING, &otp_switch_handler);

    srand(time(NULL));
}

void otp_switch_handler(){
    update_flag.otp_switch = 1;
}

void otp(){
    if(update_flag.otp_enable == 1){
        //otp need to be changed
        if(update_flag.otp_switch == 1 || update_flag.otp_web == 1 || update_flag.otp_no_user == 1){

            otp_generate();
            otp_update();

            //reset flags
            update_flag.otp_switch = 0;
            update_flag.otp_web = 0;
            update_flag.otp_no_user = 0;
        }
    }
}


void otp_generate(){

    do{
        gen_rand_str(16);
        // printf("%s\n", new_otp);
        strncpy(old_otp, new_otp, 16); //update old_otp
    }while(!strcmp(new_otp, old_otp));
    // printf("%s\n", new_otp); //for debug
}

void otp_update(){

    //update struct && config file
    strncpy(inner_data.guest_PW, new_otp, 16);
    update_flag.otp_conf = 1;

    //assign and change lcd data
    // sprintf(lcd_data.row[0], "SSID: test");
    // sprintf(lcd_data.row[1], "PW: %s", new_otp);

    update_flag.lcd = 1; //

}

void gen_rand_str(int size){

    static char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890123456789";

    for (int n = 0;n < size;n++) {
        int key = rand() % (int)(sizeof(charset) -1);
        new_otp[n] = charset[key];
    }

    new_otp[size] = '\0';


}

