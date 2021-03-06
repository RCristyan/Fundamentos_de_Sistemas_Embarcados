#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include "servidor.h"
#include "temperature_reading_client.h"
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

void socket_thread(){
    waitRequest();
}

void socket_temperature_update(){
    checkForPerimeterBreach();
    sendTemperatureReadingsToServer();
}

int main(){
    setupServer();
    gpio_control_setup();

    thread socket_listener(socket_thread);
    thread temperature_update(socket_temperature_update);
    temperature_update.join();
    socket_listener.join();

    return 0;
}
