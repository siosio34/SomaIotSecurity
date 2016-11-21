//gateway_main.h

struct main_data{
    char local_SSID[20];
    char local_PW[20];
    char guest_SSID[20];
    char guest_PW[20];
    char admin_PW[20];
};
extern struct main_data inner_data;

struct update_flags{
    int hostapd; //need to update hostapd
    int otp_enable; //otp function is active
    int otp_switch; //hardware otp switch is clicked
    int otp_web; //admin user clicked otp change button on web
    int otp_no_user; //in guest lan, no user remain
    //int otp_no_user_changed; //prevent repeated change of otp while no user using
    int otp_conf; //guest hostapd need to be change (guest_PW)
    int otp_easyword; //0 for random PW, 1 for easyword PW
    int lcd; //lcd need to be update
    int warning; //alart hacking attempt (turn on led & LCD to warn) 00(BIN):NOMAL x1(BIN):new device 1x(BIN):arp spoofing

};
extern struct update_flags update_flag;

int hostapd_local_PID;
int hostapd_guest_PID;
