//lcd.h
struct lcd_struct{
    char row[4][20];
};
extern struct lcd_struct lcd_data;

void *lcd_update(void *data);
void init_warning_LED();
void update_warning_LED();
void update_warning_sign();
