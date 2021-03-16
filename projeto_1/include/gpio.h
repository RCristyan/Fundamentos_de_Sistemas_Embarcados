#ifndef __GPIO__
#define __GPIO__

#define ON  0
#define OFF 1
#define RESISTOR  4     // GPIO 23, WiringPi 4
#define VENTOINHA 5     // GPIO 24, WiringPi 5

void setup_PWM(int pino);
void write_PWM(int pino, int valor);
void finish_PWM(int pino);

#endif
