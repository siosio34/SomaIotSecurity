//lcd.h
struct lcd_struct{
    char row1[20];
    char row2[20];
    char row3[20];
    char row4[20];
};
extern volatile struct lcd_struct lcd_data;

void *lcd_update(void *data);
