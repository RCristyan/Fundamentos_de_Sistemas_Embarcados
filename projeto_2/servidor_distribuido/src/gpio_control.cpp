extern "C"{
    #include "wiringPi.h"
}

#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>

// OUTPUT
#define LAMPADA_1_COZINHA           0    // wiringPi  0, GPIO 17
#define LAMPADA_2_SALA              1    // wiringPi  1, GPIO 18
#define LAMPADA_3_QUARTO_1          2    // wiringPi  2, GPIO 27
#define LAMPADA_4_QUARTO_2          3    // wiringPi  3, GPIO 22
#define AR_CONDICIONADO_1_QUARTO_1 23    // wiringPi 23, GPIO 13
#define AR_CONDICIONADO_2_QUARTO_2 24    // wiringPi 24, GPIO 19

// INPUT
#define SENSOR_PRESENCA_1_SALA               6  // wiringPi  6, GPIO 25
#define SENSOR_PRESENCA_2_COZINHA           25  // wiringPi 25, GPIO 26
#define SENSOR_ABERTURA_1_PORTA_COZINHA     21  // wiringPi 21, GPIO  5
#define SENSOR_ABERTURA_2_JANELA_COZINHA    22  // wiringPi 22, GPIO  6
#define SENSOR_ABERTURA_3_PORTA_SALA        26  // wiringPi 26, GPIO 12
#define SENSOR_ABERTURA_4_JANELA_SALA       27  // wiringPi 27, GPIO 16
#define SENSOR_ABERTURA_5_JANELA_QUARTO_1   28  // wiringPi 28, GPIO 20
#define SENSOR_ABERTURA_6_JANELA_QUARTO_2   29  // wiringPi 29, GPIO 21

using namespace std;

void test_gpio(int pin, int direction){
    pinMode(pin, direction);
    cout << "valor do pino " << pin << ": " << digitalRead(pin) << endl;

    if(direction == OUTPUT){
        cout << "ligando...\n";
        digitalWrite(pin, HIGH);

        cout << "valor do pino " << pin << ": " << digitalRead(pin) << endl;
        sleep(2);
        digitalWrite(pin, LOW);
    }
}

int main(){

    if(wiringPiSetup() < 0){
        perror("erro ao configurar wiringpi");
        exit(EXIT_FAILURE);
    }

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