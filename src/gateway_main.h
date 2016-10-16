//gateway_main.h

struct main_data{
    char local_SSID[20];
    char local_PW[20];
    char guest_SSID[20];
    char guest_PW[20];
};
extern volatile struct main_data inner_data;

struct update_flags{
    int hostapd;
    int otp_switch;
    int otp_web;
    int otp_no_user;
    int otp_conf;
    int lcd;

};
extern volatile struct update_flags update_flag;

