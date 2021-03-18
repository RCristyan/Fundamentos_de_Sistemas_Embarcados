#ifndef __UART_MODBUS__
#define __UART_MODBUS__

void configura_uart();
float get_uart_sensor_value(int option);
void close_uart();

#endif