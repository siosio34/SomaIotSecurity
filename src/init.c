#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include "gateway_main.h"
#include "write_conf.h"
#include "lcd.h"
#include "otp.h"
#include "init.h"

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
