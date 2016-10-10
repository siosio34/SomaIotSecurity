//communicator.h
extern volatile int global_test;
//extern int flag_page[5];
extern volatile int flag_update;
struct pharsed_data{
    char local_SSID[20];
    char local_PW[20];
};
extern volatile struct pharsed_data internal_data;

void *t_function(void *data);
