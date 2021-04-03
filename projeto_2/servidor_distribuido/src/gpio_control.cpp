extern "C"{
    #include "wiringPi.h"
}

#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio_control.h"

using namespace std;

void gpio_control_setup(){
    if(wiringPiSetup() < 0){
        perror("erro ao configurar wiringpi");
        exit(EXIT_FAILURE);
    }

    pinMode(LAMPADA_1_COZINHA, OUTPUT);
    pinMode(LAMPADA_2_SALA, OUTPUT);
    pinMode(LAMPADA_3_QUARTO_1, OUTPUT);
    pinMode(AR_CONDICIONADO_1_QUARTO_1, OUTPUT);
    pinMode(AR_CONDICIONADO_2_QUARTO_2, OUTPUT);

    pinMode(SENSOR_PRESENCA_1_SALA, INPUT);
    pinMode(SENSOR_PRESENCA_2_COZINHA, INPUT);
    pinMode(SENSOR_ABERTURA_1_PORTA_COZINHA, INPUT);
    pinMode(SENSOR_ABERTURA_2_JANELA_COZINHA, INPUT);
    pinMode(SENSOR_ABERTURA_3_PORTA_SALA, INPUT);
    pinMode(SENSOR_ABERTURA_4_JANELA_SALA, INPUT);
    pinMode(SENSOR_ABERTURA_5_JANELA_QUARTO_1, INPUT);
    pinMode(SENSOR_ABERTURA_6_JANELA_QUARTO_2, INPUT);
}

void test_gpio(int pin, int direction){
    cout << "valor do pino " << pin << ": " << digitalRead(pin) << endl;

    if(direction == OUTPUT){
        cout << "ligando...\n";
        digitalWrite(pin, HIGH);

        cout << "valor do pino " << pin << ": " << digitalRead(pin) << endl;
        sleep(2);
        digitalWrite(pin, LOW);
    }
}
