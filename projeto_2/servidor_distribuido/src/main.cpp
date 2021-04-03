#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "gpio_control.h"

extern "C"{
    #include "i2c_read_bme280.h"
}

using namespace std;

int main(){

    cout << "Hello from C++!\n";
    
    setupBME280();
    float t, p, u;

    for(int x = 0; x < 30; x++){
        t = get_BME280_reading('t');
        p = get_BME280_reading('p');
        u = get_BME280_reading('u');

        cout << "T: " << t << ", P: " << p << ", U: " << u << endl;

        usleep(200000);
    }

    gpio_control_setup();

    test_gpio(SENSOR_PRESENCA_1_SALA, INPUT);
    test_gpio(SENSOR_PRESENCA_2_COZINHA, INPUT);
    test_gpio(SENSOR_ABERTURA_1_PORTA_COZINHA, INPUT);
    test_gpio(SENSOR_ABERTURA_2_JANELA_COZINHA, INPUT);
    test_gpio(SENSOR_ABERTURA_3_PORTA_SALA, INPUT);
    test_gpio(SENSOR_ABERTURA_4_JANELA_SALA, INPUT);
    test_gpio(SENSOR_ABERTURA_5_JANELA_QUARTO_1, INPUT);
    test_gpio(SENSOR_ABERTURA_6_JANELA_QUARTO_2, INPUT);

    return 0;
}
