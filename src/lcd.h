//lcd.h
struct lcd_struct{
    char row[4][20];
};
extern volatile struct lcd_struct lcd_data;

void *lcd_update(void *data);


