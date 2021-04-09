#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <csignal>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "servidor.h"
#include "gpio_control.h"

#define PORTA_SERVIDOR 10123
#define BUFFER_SIZE 50

using namespace std;

int servidorSocket;
int clienteSocket;
struct sockaddr_in servidorAddr;
struct sockaddr_in clienteAddr;
unsigned int clienteLength;
char buffer[BUFFER_SIZE];
char server_response[50];

void closeServer(int signal){
    cout << "\nEncerrando servidor...\n";

    close(clienteSocket);
    close(servidorSocket);
    shutdown_gpio();
    exit(EXIT_SUCCESS);
}

void handleRequisiton(){
    if(strcmp(buffer, "exit") == 0) closeServer(SIGINT);
    else if(strcmp(buffer, "ligar lampada 01") == 0){
        cout << "ligando...\n";
        ligar_dispositivo(LAMPADA_1_COZINHA);
        sleep(3);
        desligar_dispositivo(LAMPADA_1_COZINHA);
        strcpy(server_response, "lampada 01 ligada");
        cout << "lampada 01 ligada\n";
    }
    else if(strcmp(buffer, "ligar lampada 02") == 0){
        cout << "ligando...\n";
        ligar_dispositivo(LAMPADA_2_SALA);
        sleep(3);
        desligar_dispositivo(LAMPADA_2_SALA);
        strcpy(server_response, "lampada 02 ligada");
        cout << "lampada 02 ligada\n";
    }
    else if(strcmp(buffer, "ligar lampada 03") == 0){
        cout << "ligando...\n";
        ligar_dispositivo(LAMPADA_3_QUARTO_1);
        sleep(3);
        desligar_dispositivo(LAMPADA_3_QUARTO_1);
        strcpy(server_response, "lampada 03 ligada");
        cout << "lampada 03 ligada\n";
    }
    else if(strcmp(buffer, "ligar lampada 04") == 0){
        cout << "ligando...\n";
        ligar_dispositivo(LAMPADA_4_QUARTO_2);
        sleep(3);
        desligar_dispositivo(LAMPADA_4_QUARTO_2);
        strcpy(server_response, "lampada 04 ligada");
        cout << "lampada 04 ligada\n";
    }
    else{
        strcpy(server_response, "comando não reconhecido");
        cout << "comando não reconhecido\n";
    }
}

void setupServer(){
    // setup
    signal(SIGINT, closeServer);
    gpio_control_setup();

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
}

void trataClienteSocket(int clienteSocket){
    int tamanhoRecebido;

    if((tamanhoRecebido = recv(clienteSocket, buffer, BUFFER_SIZE, 0)) < 0){
        perror("falha em recv()");
        exit(EXIT_FAILURE);
    }

    buffer[tamanhoRecebido] = '\0';

    cout << "Servidor recebeu: ";
    for(int i = 0; buffer[i] != '\0'; i++) cout << buffer[i];
    cout << endl;

    handleRequisiton();

    if(send(clienteSocket, server_response, strlen(server_response), 0) < 0){
        perror("falha em send");
        exit(EXIT_FAILURE);
    }

    memset(&server_response, 0, sizeof(server_response));
}

void waitRequest(){
    while(1){
        clienteLength = sizeof(clienteAddr);
        if((clienteSocket = accept(servidorSocket,
                                    (struct sockaddr *) &clienteAddr,
                                    &clienteLength)) < 0){
            perror("falha no accept");
            exit(EXIT_FAILURE);
        }

        cout << "conexão do cliente " << inet_ntoa(clienteAddr.sin_addr) << endl;

        trataClienteSocket(clienteSocket);

        close(clienteSocket);
    }

    cout << "Servidor encerrado\n";
    close(servidorSocket);
}
