#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "gpio_control.h"
#include "servidor.h"

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

void gpio_tests(){
    gpio_control_setup();

    ligar_dispositivo(LAMPADA_1_COZINHA);
    ligar_dispositivo(LAMPADA_2_SALA);
    ligar_dispositivo(LAMPADA_3_QUARTO_1);
    ligar_dispositivo(LAMPADA_4_QUARTO_2);
    ligar_dispositivo(AR_CONDICIONADO_1_QUARTO_1);
    ligar_dispositivo(AR_CONDICIONADO_2_QUARTO_2);

    sleep(1);

    if (perimeter_breach()) cout << "brecha de segurança!!\n";
    else cout << "tudo certo\n";

    shutdown_gpio();
}

// escutar as requisições do socket através de uma thread
void socket_wait_request(){
    waitRequest();
}

int main(){
    setupServer();

    socket_wait_request();

    return 0;
}
