#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <wiringPi.h>           //WiringPi headers
#include <lcd.h>                //LCD headers from WiringPi
#include "lcd.h"
#include "gateway_main.h"

//Pin numbers below are the WiringPi pin numbers
const int LCD_RS = 3;
const int LCD_E = 0;
const int LCD_D4 = 6;
const int LCD_D5 = 1;
const int LCD_D6 = 5;
const int LCD_D7 = 4;
const int LCD_ROW = 4;
const int LCD_COL = 20;

const int LED_WARNING = 13; //wiringPi pin 13
int warning_updated = 0;

void *lcd_update(void *data){
    int lcd; //Handle for LCD

    //init LCD
    if (lcd = lcdInit (LCD_ROW, LCD_COL, 4, LCD_RS, LCD_E ,LCD_D4 , LCD_D5, LCD_D6,LCD_D7,0,0,0,0)){
            printf ("lcdInit failed! \n");
    }

    //init warning LED
    init_warning_LED();
    int i;int y=0;

    while(1){
        //check is there update for lcd
        if(update_flag.lcd == 1){
            //write lcd
            lcdClear(lcd);
            lcdPosition(lcd,0,0);
            lcdPuts(lcd, lcd_data.row[0]);
            lcdPosition(lcd,0,1);
            lcdPuts(lcd, lcd_data.row[1]);
            lcdPosition(lcd,0,2);
            lcdPuts(lcd, lcd_data.row[2]);
            lcdPosition(lcd,0,3);
            lcdPuts(lcd, lcd_data.row[3]);

            update_flag.lcd = 0;
        }

        //check is there update for led
        update_warning_LED();
        update_warning_sign(); //pring warning sign on LCD
        sleep(1); //check for each second

    } //while end


}

void init_warning_LED(){
    pullUpDnControl(LED_WARNING, PUD_DOWN); //pull-down
    pinMode(LED_WARNING, OUTPUT);
}

void update_warning_LED(){
    if(update_flag.warning == 1 || update_flag.warning == 2){
        digitalWrite(LED_WARNING, HIGH);
    }
    else{
        digitalWrite(LED_WARNING, LOW);
    }

}

void update_warning_sign(){
    if(warning_updated != update_flag.warning){
        update_flag.lcd = 1;
        warning_updated = update_flag.warning;
    }

    if(update_flag.warning == 1){ //alart hackking attempt
        sprintf(lcd_data.row[2], "!!!Hacking Attempt!!");
        sprintf(lcd_data.row[3], "!!!!! Detected !!!!!");
    }
    else if(update_flag.warning == 2){ //alart new device connected
        sprintf(lcd_data.row[2], "New Device connected");
        sprintf(lcd_data.row[3], "  check admin page  ");
    }
    else{
        sprintf(lcd_data.row[2], "                    ");
        sprintf(lcd_data.row[3], "                    ");
    }
}
