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
const int OTP_LENGTH = 8;
const int OTP_WORD_LENGTH = 16;
const int EASYWORD_LINE_NUM = 230;
char new_otp[16] = {0};
char old_otp[16] = {0};

void otp_init(){
    //otp HW switch init
    pinMode(OTP_SW_PIN, INPUT) ;
    pullUpDnControl(OTP_SW_PIN, PUD_UP); //pull-up switch pin
    wiringPiISR(OTP_SW_PIN, INT_EDGE_FALLING, &otp_switch_handler);
    
    printf("otp init \n");
    srand(time(NULL));
}

void otp_switch_handler(){
    update_flag.otp_switch = 1;
    printf("otp clicked");
}

void otp(){
    if(update_flag.otp_enable == 1){

        sleep(1); //otp_switch debouncing

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

    if(update_flag.otp_easyword == 1){ // easyword PW
        gen_easy_str();
        while(strcmp(new_otp, old_otp) == 0){ //if new_otp is same as old_otp
            gen_easy_str(); //make again
            // printf("%s\n", new_otp);
        }
        strncpy(old_otp, new_otp, OTP_WORD_LENGTH); //update old_otp

    }
    else{ //random PW
        gen_rand_str(OTP_LENGTH); //generate random string to new_otp
        while(strcmp(new_otp, old_otp) == 0){ //if new_otp is same as old_otp
            gen_rand_str(OTP_LENGTH); //make again
            // printf("%s\n", new_otp);
        }
        strncpy(old_otp, new_otp, OTP_LENGTH); //update old_otp
    }



    // printf("%s\n", new_otp); //for debug
}

void otp_update(){

    //update struct && config file
    strncpy(inner_data.guest_PW, new_otp, OTP_WORD_LENGTH); //need to fix
    update_flag.otp_conf = 1;

    //assign and change lcd data
    sprintf(lcd_data.row[0], "SSID: %s", inner_data.guest_SSID);
    sprintf(lcd_data.row[1], "PW: %s", inner_data.guest_PW);

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

void gen_easy_str(){

    int lineNumber = rand() % EASYWORD_LINE_NUM - 1; //pick random line from easyword DB(.txt)

    static const char filename[] = "easyword.txt";
    FILE *file = fopen(filename, "r");
    int count = 0;
    if ( file != NULL ){
        char line[OTP_WORD_LENGTH]; /* or other suitable maximum line size */
        while (fgets(line, OTP_WORD_LENGTH, file) != NULL){ /* read a line */
            if (count == lineNumber){
                memset(new_otp, '\0', OTP_WORD_LENGTH);
                strncpy(new_otp, line, OTP_WORD_LENGTH); //update new_otp
                new_otp[strlen(line)-1] = ' '; //prevent printing strange char end of otp
                printf("%s\n", line); //for debug
                fclose(file);
                return;
            }
            else{
                count++;
            }
        }
        fclose(file);
    }
    else{
        //file doesn't exist
        printf("easyword.txt file not exist\n"); //for debug
    }
}

