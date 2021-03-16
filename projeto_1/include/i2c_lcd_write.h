#ifndef __I2C_LCD_WRITE__
#define __I2C_LCD_WRITE__

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line

void configura_i2c_lcd();
void lcd_write(int line, char string[20], int delay_ms);
void clear_lcd();
void write_values_on_lcd(float TI, float TE, float TR);

#endif