#ifndef __UART_MODBUS__
#define __UART_MODBUS__

#define LM35            0
#define POTENCIOMETRO   1

void configura_uart();
float get_uart_sensor_value(int option);
void close_uart();

#endif
