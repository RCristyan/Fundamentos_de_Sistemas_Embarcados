#include <stdio.h>
#include <stdlib.h>
#include "wiringPi.h"
#include "softPwm.h"
#include "gpio.h"

void setup_PWM(int pino){
    if(wiringPiSetup() < 0){
        printf("Erro ao inicializar wiringPi para acessar pinos GPIO.\n");
        exit(1);
    }

    pinMode(pino, OUTPUT);
    softPwmCreate(pino, ON, 100);
}

void write_PWM(int pino, int valor){
    softPwmWrite(pino, valor);
}

void finish_PWM(int pino){
    softPwmWrite(pino, 0);
    delay(50);
}
