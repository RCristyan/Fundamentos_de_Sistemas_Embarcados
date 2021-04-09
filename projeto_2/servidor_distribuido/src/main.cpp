#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <signal.h>
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

void socket_thread(){
    waitRequest();
}

int main(){
    gpio_control_setup();
    setupServer();

    thread socket_listener(socket_thread);

    cout << "Rodando na main\n";

    socket_listener.join();

    return 0;
}
