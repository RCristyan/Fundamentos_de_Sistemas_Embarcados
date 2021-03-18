#ifndef __LOGGER__
#define __LOGGER__

#define DATETIME_STRING_SIZE 23

void create_log_file();
void write_on_log_file(float TI, float TE, float TR, int intensidade_ventoinha, int intensidade_resistor);
void close_log_file();

#endif