#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "write_conf.h"
#include "gateway_main.h"
#include "init.h"
#include "lcd.h"
char command[100] = {0};

void write_conf(){

    if(update_flag.hostapd == 1){ //웹에서 업데이트 할 내용이 생긴 경우
        update_hostapd();
        backup_struct();
        update_flag.hostapd = 0; //flag 초기
    }
    else if(update_flag.otp_conf == 1){
        update_otp();
        backup_struct();
        update_flag.otp_conf = 0;
    }
}

void update_hostapd(){

    //update local hostapd .conf (unactivated)
    sprintf(command, "./script/update_hostapd_conf.sh %s %s", inner_data.local_SSID, inner_data.local_PW);
    system(command); //update conf by script

    //update guest hostapd .conf (unactivated)
    sprintf(command, "./script/update_hostapd_conf_1.sh %s %s", inner_data.guest_SSID, inner_data.guest_PW);
    system(command); //update conf by script

    //restart hostapd
    restart_hostapd_local();
    restart_hostapd_guest();
    sleep(1);
    sprintf(lcd_data.row[0], "SSID: %s", inner_data.guest_SSID);
    sprintf(lcd_data.row[1], "PW: %s", inner_data.guest_PW);
    update_flag.lcd = 1;
}

void update_otp(){

    //update guest hostapd .conf (unactivated)
    sprintf(command, "./script/update_hostapd_conf_1.sh %s %s", inner_data.guest_SSID, inner_data.guest_PW);
    system(command); //update conf by script


    restart_hostapd_guest();

}
