#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "lcd.h"
#include "gateway_main.h"


void *lcd_update(void *data){

    while(1){
        if(update_flag.lcd == 1){
            //write lcd
            printf("same memory shared check\n");
            update_flag.lcd = 0;
        }
        sleep(1); //check for each second

    } //while end


}
