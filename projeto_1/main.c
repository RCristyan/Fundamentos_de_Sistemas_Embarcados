#include <stdio.h>
#include <stdlib.h>
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

        write_values(TI, TE, TR);
    }

    close_uart();

    return 0;
}