#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <signal.h>
#include "gpio_control.h"
// #include "servidor.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#define PORTA_SERVIDOR 10123
#define BUFFER_SIZE 50
int servidorSocket;
int clienteSocket;
struct sockaddr_in servidorAddr;
struct sockaddr_in clienteAddr;
unsigned int clienteLength;
char buffer[BUFFER_SIZE];
int tamanhoRecebido;
const char* response = "hello from server";


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

void closeServer(int signal){
    cout << "\nEncerrando servidor...\n";

    close(clienteSocket);
    close(servidorSocket);
    shutdown_gpio();
    exit(EXIT_SUCCESS);
}

void trataClienteSocket(int clienteSocket){
    if((tamanhoRecebido = recv(clienteSocket, buffer, BUFFER_SIZE, 0)) < 0){
        perror("falha em recv()");
        exit(EXIT_FAILURE);
    }

    buffer[tamanhoRecebido] = '\0';

    cout << "Servidor recebeu: ";
    for(int i = 0; buffer[i] != '\0'; i++) cout << buffer[i];
    cout << endl;

    if(send(clienteSocket, response, strlen(response), 0) < 0){
        perror("falha em send");
        exit(EXIT_FAILURE);
    }

    if(strcmp(buffer, "exit") == 0) closeServer(SIGINT);
    else if(strcmp(buffer, "ligar lampada 01") == 0){
        cout << "ligando...\n";
        ligar_dispositivo(LAMPADA_1_COZINHA);
        sleep(3);
        desligar_dispositivo(LAMPADA_1_COZINHA);
    } 
}

void teste_socket(){
    // setup
    signal(SIGINT, closeServer);

    if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("falha no socket do servidor");
        exit(EXIT_FAILURE);
    }

    memset(&servidorAddr, 0, sizeof(servidorAddr));
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servidorAddr.sin_port = htons(PORTA_SERVIDOR);

    if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0){
        perror("falha no bind");
        exit(EXIT_FAILURE);
    }

    if(listen(servidorSocket, 10) < 0){
        perror("falha no listen");
        exit(EXIT_FAILURE);
    }

    cout << "Servidor aguardando requisições...\n";
    cout << "IP: " << inet_ntoa(servidorAddr.sin_addr) << endl;
    cout << "porta: " << (int) ntohs(servidorAddr.sin_port) << endl;

    // wait request
    while(1){
        clienteLength = sizeof(clienteAddr);
        if((clienteSocket = accept(servidorSocket,
                                    (struct sockaddr *) &clienteAddr,
                                    &clienteLength)) < 0){
            perror("falha no accept");
            exit(EXIT_FAILURE);
        }

        cout << "conexão do cliente " << inet_ntoa(clienteAddr.sin_addr) << endl;

        // trata conexão
        trataClienteSocket(clienteSocket);

        close(clienteSocket);
    }

    cout << "Servidor encerrado\n";
    close(servidorSocket);
}

int main(){
    gpio_control_setup();

    thread socket_listener(teste_socket);

    cout << "A thread foi ouvir o bagui, eu to tranquilão\n";

    socket_listener.join();

    return 0;
}
