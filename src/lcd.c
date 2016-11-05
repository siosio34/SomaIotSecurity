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


void *lcd_update(void *data){
    int lcd; //Handle for LCD

    // wiringPiSetup();        //Initialise WiringPi

    //init LCD
    if (lcd = lcdInit (LCD_ROW, LCD_COL, 4, LCD_RS, LCD_E ,LCD_D4 , LCD_D5, LCD_D6,LCD_D7,0,0,0,0)){
            printf ("lcdInit failed! \n");
    }

    //test example
    // strncpy(lcd_data.row[0],"Hello World!" , 20);
    // strncpy(lcd_data.row[1],"2nd row" , 20);
    // strncpy(lcd_data.row[2],"3nd row" , 20);
    int i;int y=0;

    while(1){
        if(update_flag.lcd == 1){
            //write lcd
            // printf("same memory shared check\n");
            lcdClear(lcd);
            lcdPosition(lcd,0,0);
            lcdPuts(lcd, lcd_data.row[0]);
            lcdPosition(lcd,0,1);
            lcdPuts(lcd, lcd_data.row[1]);
            lcdPosition(lcd,0,2);
            lcdPuts(lcd, lcd_data.row[2]);
            lcdPosition(lcd,0,3);
            lcdPuts(lcd, lcd_data.row[3]);

            //for debug
            //sprintf(lcd_data.row[3],"%d", y); y++;

            update_flag.lcd = 0;
        }
        // sleep(1); //check for each second

    } //while end


}


