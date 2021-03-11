#include <stdio.h>
#include <unistd.h>
#include "uart_modbus.h"

int main(){
    configura_uart();

    printf("Temperatura fornecida pelo sensor LM35 = %.2f °C\n", get_LM35_reading());
    printf("-----------\n");
    printf("Temperatura fornecida pelo potenciometro = %.2f °C\n", get_potenciometro_reading());

    close_uart();

    return 0;
}