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
    pinMode(LAMPADA_4_QUARTO_2, OUTPUT);
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

void ligar_dispositivo(int pin){
    digitalWrite(pin, HIGH);
}

void desligar_dispositivo(int pin){
    digitalWrite(pin, LOW);
}

void shutdown_gpio(){
    desligar_dispositivo(LAMPADA_1_COZINHA);
    desligar_dispositivo(LAMPADA_2_SALA);
    desligar_dispositivo(LAMPADA_3_QUARTO_1);
    desligar_dispositivo(LAMPADA_4_QUARTO_2);
    desligar_dispositivo(AR_CONDICIONADO_1_QUARTO_1);
    desligar_dispositivo(AR_CONDICIONADO_2_QUARTO_2);
}

void display_gpio_status(){
    cout << "----------------\n";
    cout << "Valores GPIO\n";
    cout << "Dispositivos:\n";
    cout << "LAMPADA_1_COZINHA: " << digitalRead(LAMPADA_1_COZINHA) << endl;
    cout << "LAMPADA_2_SALA: " << digitalRead(LAMPADA_2_SALA) << endl;
    cout << "LAMPADA_3_QUARTO_1: " << digitalRead(LAMPADA_3_QUARTO_1) << endl;
    cout << "LAMPADA_4_QUARTO_2: " << digitalRead(LAMPADA_4_QUARTO_2) << endl;
    cout << "AR_CONDICIONADO_1_QUARTO_1: " << digitalRead(AR_CONDICIONADO_1_QUARTO_1) << endl;
    cout << "AR_CONDICIONADO_2_QUARTO_2: " << digitalRead(AR_CONDICIONADO_2_QUARTO_2) << endl;

    cout << "\nSensores:\n";
    cout << "SENSOR_PRESENCA_1_SALA: " << digitalRead(SENSOR_PRESENCA_1_SALA) << endl;
    cout << "SENSOR_PRESENCA_2_COZINHA: " << digitalRead(SENSOR_PRESENCA_2_COZINHA) << endl;
    cout << "SENSOR_ABERTURA_1_PORTA_COZINHA: " << digitalRead(SENSOR_ABERTURA_1_PORTA_COZINHA) << endl;
    cout << "SENSOR_ABERTURA_2_JANELA_COZINHA: " << digitalRead(SENSOR_ABERTURA_2_JANELA_COZINHA) << endl;
    cout << "SENSOR_ABERTURA_3_PORTA_SALA: " << digitalRead(SENSOR_ABERTURA_3_PORTA_SALA) << endl;
    cout << "SENSOR_ABERTURA_4_JANELA_SALA: " << digitalRead(SENSOR_ABERTURA_4_JANELA_SALA) << endl;
    cout << "SENSOR_ABERTURA_5_JANELA_QUARTO_1: " << digitalRead(SENSOR_ABERTURA_5_JANELA_QUARTO_1) << endl;
    cout << "SENSOR_ABERTURA_6_JANELA_QUARTO_2: " << digitalRead(SENSOR_ABERTURA_6_JANELA_QUARTO_2) << endl;
    cout << "----------------\n";
}
