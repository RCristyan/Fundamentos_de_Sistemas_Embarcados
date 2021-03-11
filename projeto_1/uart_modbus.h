#ifndef __UART_MODBUS__
#define __UART_MODBUS__

void configura_uart();
float get_LM35_reading();
float get_potenciometro_reading();
void close_uart();

#endif