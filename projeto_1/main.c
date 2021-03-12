#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "uart_modbus.h"
#include "i2c_read_bme280.h"
#include "i2c_lcd_write.h"

int main(){
    configura_uart();
    setupBME280();    
    configura_i2c_lcd();

    float TI, TE, TR;
    float p, u;

    while (1)   {
        TI = get_LM35_reading();
        TE = get_BME280_reading('t');
        p = get_BME280_reading('p');
        u = get_BME280_reading('u');
        TR = get_potenciometro_reading();

        char ti_buffer[20];
        char te_buffer[20];
        char tr_buffer[20];

        gcvt(TI, 4, ti_buffer);
        gcvt(TE, 4, te_buffer);
        gcvt(TR, 4, tr_buffer);

        printf("TI (buffer) = ");
        printf("%s, ", ti_buffer);
        printf("TE (buffer) = ");
        printf("%s, ", te_buffer);
        printf("TR (buffer) = ");
        printf("%s\n", tr_buffer);

        char line1[50];
        char line2[50];
        strcpy(line1, "TI:");
        strcat(line1, ti_buffer);
        strcat(line1, " TR:");
        strcat(line1, tr_buffer);

        strcpy(line2, "TE:");
        strcat(line2, te_buffer);

        clear_lcd();
        lcd_write(LINE1, line1, 0);
        lcd_write(LINE2, line2, 1000);
    }

    close_uart();

    return 0;
}