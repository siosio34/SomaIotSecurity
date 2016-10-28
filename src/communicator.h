//communicator.h
extern int global_test;
//extern int flag_page[5];
extern int flag_update;
struct pharsed_data{
    char local_SSID[20];
    char local_PW[20];
};
extern struct pharsed_data internal_data;

void *t_function(void *data);
