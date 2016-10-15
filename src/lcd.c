#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "lcd.h"

void *lcd_update(void *data){

    while(1){
        if(update_flag.lcd == 1){
            //write lcd
        }
        sleep(1); //check for each second

    } //while end


}
