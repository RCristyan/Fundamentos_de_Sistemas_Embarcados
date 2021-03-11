#include <stdio.h>
#include <unistd.h>
#include "uart_modbus.h"
#include "i2c_read_bme280.h"

int main(){
    configura_uart();

    printf("Temperatura fornecida pelo sensor LM35 = %.2f °C\n", get_LM35_reading());
    printf("-----------\n");
    printf("Temperatura fornecida pelo potenciometro = %.2f °C\n", get_potenciometro_reading());

    setupBME280();    
    printf("Valores fornecidos pelo BME:\n");
    printf("T = %2f °C, P = %6.2f Pa, H = %2.2f%%\n", get_BME280_reading('t'), get_BME280_reading('p'), get_BME280_reading('u'));

    close_uart();

    return 0;
}