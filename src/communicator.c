#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> //pthread_create(), pthread_join(), pthread_self()
#include "communicator.h"


//쓰레드에서 사용할 함수
void *t_function(void *data)
{
    int id;
    int i = 999;
    int json_ID;
    int new_json = 1;

    // id = *((int *)data);

    while(1)
    {
        
        //===========json receiver=========//
        //place json receive function here
        //=================================//
        json_ID = 1; //update example
        char example_SSID[20] = "Pi3-AP"; //test data
        char example_PW[20] = "12345678";

        if(new_json == 1 && json_ID == 1){ //received json data is for update
            //write global struct
            strncpy(internal_data.local_SSID, example_SSID, 20);

            //raise update flag
            flag_update = 1;
        }


        // if(global_test==42)

        sleep(15); //delay for test
        
    }
    return (void *)i;
}
