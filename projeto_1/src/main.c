#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "uart_modbus.h"
#include "i2c_read_bme280.h"
#include "i2c_lcd_write.h"
#include "logger.h"
#include "gpio.h"

void encerrar_processo(){
    printf("\n");
    printf("Encerrando processo...\n");

    close_uart();
    close_log_file();
    finish_PWM(VENTOINHA);
    finish_PWM(RESISTOR);

    exit(0);
}

int main(){
    configura_uart();
    setupBME280();    
    configura_i2c_lcd();
    create_log_file();
    setup_PWM(VENTOINHA);
    setup_PWM(RESISTOR);
    
    signal(SIGINT, encerrar_processo);

    float TI, TE, TR;
    float p, u;
    int intensidade_ventoinha = 0;
    int intensidade_resistor = 0;
    int direction = 1;

    while(1){
        TI = get_LM35_reading();
        TE = get_BME280_reading('t');
        p = get_BME280_reading('p');
        u = get_BME280_reading('u');
        TR = get_potenciometro_reading();

        write_values_on_lcd(TI, TE, TR);
        write_on_log_file(TI, TE, TR);

        printf("TI\tTE\tTR\tVentoinha(%%)\tResistor(%%)\n");
        printf("%.2f\t%.2f\t%.2f\t", TI, TE, TR);
        printf("%d\t\t%d\n", intensidade_ventoinha, intensidade_resistor);
        printf("---------------------------------------\n");

        // calcular pid aqui
        // passar valor calculado para gpio

        write_PWM(VENTOINHA, intensidade_ventoinha);
        write_PWM(RESISTOR, intensidade_resistor);
        intensidade_ventoinha += 20 * direction;

        if(intensidade_ventoinha > 100){
            intensidade_ventoinha = 100;
            direction = -1;
        } else if(intensidade_ventoinha < 0){
            intensidade_ventoinha = 0;
            direction = 1;
        }

        intensidade_resistor = intensidade_ventoinha;
    }

    return 0;
}
