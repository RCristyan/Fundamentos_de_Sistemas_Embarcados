#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "uart_modbus.h"
#include "i2c_read_bme280.h"
#include "i2c_lcd_write.h"
#include "logger.h"

void encerrar_processo(){
    printf("encerrando processo");

    close_uart();
    close_log_file();

    exit(0);
}

int main(){
    configura_uart();
    setupBME280();    
    configura_i2c_lcd();
    create_log_file();
    
    signal(SIGINT, encerrar_processo);

    float TI, TE, TR;
    float p, u;

    while(1){
        TI = get_LM35_reading();
        TE = get_BME280_reading('t');
        p = get_BME280_reading('p');
        u = get_BME280_reading('u');
        TR = get_potenciometro_reading();

        write_values_on_lcd(TI, TE, TR);
        write_on_log_file(TI, TE, TR);
    }

    return 0;
}