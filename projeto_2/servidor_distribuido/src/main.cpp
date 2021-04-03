#include <iostream>
#include <unistd.h>
#include <stdlib.h>

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

    return 0;
}
