#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

// bilbioteca disponível em https://github.com/bitbank2/bme280
#include "bme.h"

#define DEVICE_ADDRESS 0x76
#define I2C_PORT 0X01

void setupBME280(){
    int i = bme280Init(I2C_PORT, DEVICE_ADDRESS);
    if(i != 0) exit(1);
    printf("bme280 aberto\n");
}

float get_BME280_reading(char option){
    int temperatura, pressao, umidade;
    bme280ReadValues(&temperatura, &pressao, &umidade);

    if(option == 't' || option == 'T') return (float) temperatura/100.0;
    else if(option == 'p' || option == 'P') return (float) pressao/256.0;
    else if(option == 'u' || option == 'U') return (float) umidade/1024.0;

}
