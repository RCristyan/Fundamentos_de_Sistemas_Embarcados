#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "uart_modbus.h"
#include "i2c_read_bme280.h"
#include "i2c_lcd_write.h"
#include "logger.h"
#include "gpio.h"
#include "pid.h"

int intensidade_ventoinha = 0;
int intensidade_resistor = 0;
double retorno_pid = 0;

void encerrar_processo(){
    printf("\n");
    printf("Encerrando processo...\n");

    close_uart();
    close_log_file();
    clear_lcd();
    finish_PWM(VENTOINHA);
    finish_PWM(RESISTOR);

    exit(0);
}

void read_inputs(float* TI, float* TE, float* TR, float* p, float* u){
    *TI = get_uart_sensor_value(LM35);
    if(*TI == -1){
        printf("Erro na leitura do sensor LM35\n");
        encerrar_processo();
    }

    *TE = get_BME280_reading('t');
    *p = get_BME280_reading('p');
    *u = get_BME280_reading('u');

    *TR = get_uart_sensor_value(POTENCIOMETRO);
    if(*TR == -1){
        printf("Erro na leitura do potenciometro\n");
        encerrar_processo();
    }
}

void temperature_control(float *TI, float *TR){
    pid_atualiza_referencia(*TR);
    retorno_pid = pid_controle(*TI);

    if(retorno_pid < 0){
        intensidade_ventoinha = -1 * retorno_pid;
        if(intensidade_ventoinha < 40) intensidade_ventoinha = 0;
        intensidade_resistor = 0;
    } else if(retorno_pid >= 0){
        intensidade_ventoinha = 0;
        intensidade_resistor = retorno_pid;
    }

    write_PWM(VENTOINHA, intensidade_ventoinha);
    write_PWM(RESISTOR, intensidade_resistor);
}

void print_info(float TI, float TE, float TR, double retorno_pid, int intensidade_ventoinha, int intensidade_resistor){
    printf("TI\tTE\tTR\tPID\tVentoinha(%%)\tResistor(%%)\n");
    printf("%.2f\t%.2f\t%.2f\t", TI, TE, TR);
    printf("%.2lf\t%d\t\t%d\n", retorno_pid, intensidade_ventoinha, intensidade_resistor);
    printf("----------\n");
}

void trata_sigalrm(){
    alarm(1);
}

void setup(){
    configura_uart();
    setupBME280();    
    configura_i2c_lcd();
    create_log_file();
    setup_PWM(VENTOINHA);
    setup_PWM(RESISTOR);
    signal(SIGINT, encerrar_processo);
    signal(SIGALRM, trata_sigalrm);
    alarm(1);
    pid_configura_constantes(5.0, 1.0, 5.0);
}

int menu(){
    printf("===============================\n");
    printf("Bem vindo ao menu. Escolha como definir a temperatura de referência:\n\n");
   
    int option = 0;

    while(1){
        printf("\t1. Pelo potenciometro.\n");
        printf("\t2. Pelo terminal.\n");
        scanf("%d", &option);

        if(option == 1 || option == 2) break;

        printf("Escolha uma das opções!\n");
    }

    printf("===============================\n");

    return option;
}

int main(){
    setup();

    float TI, TE, TR;
    float p, u;

    int option = menu();

    while(1){
        if(option == 1) read_inputs(&TI, &TE, &TR, &p, &u);
        write_values_on_lcd(TI, TE, TR);

        print_info(TI, TE, TR, retorno_pid, intensidade_ventoinha, intensidade_resistor);
        
        temperature_control(&TI, &TR);
        write_on_log_file(TI, TE, TR, intensidade_ventoinha, intensidade_resistor);

        pause();
    }

    return 0;
}
