#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "gpio_control.h"

extern "C"{
    #include "i2c_read_bme280.h"
}

using namespace std;

void i2c_sensor_reading(){
    setupBME280();
    float t, p, u;

    for(int x = 0; x < 30; x++){
        t = get_BME280_reading('t');
        p = get_BME280_reading('p');
        u = get_BME280_reading('u');

        cout << "T: " << t << ", P: " << p << ", U: " << u << endl;

        usleep(200000);
    }
}

int main(){

    gpio_control_setup();

    ligar_dispositivo(LAMPADA_1_COZINHA);
    ligar_dispositivo(LAMPADA_2_SALA);
    ligar_dispositivo(LAMPADA_3_QUARTO_1);
    ligar_dispositivo(LAMPADA_4_QUARTO_2);
    ligar_dispositivo(AR_CONDICIONADO_1_QUARTO_1);
    ligar_dispositivo(AR_CONDICIONADO_2_QUARTO_2);

    sleep(2);

    shutdown_gpio();

    return 0;
}
