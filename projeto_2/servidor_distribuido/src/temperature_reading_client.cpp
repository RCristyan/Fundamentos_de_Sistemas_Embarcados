#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern "C"{
    #include "i2c_read_bme280.h"
}

#define PORTA_SERVIDOR 10023
#define IP_SERVIDOR "192.168.0.53"
#define BUFFER_SIZE 50

using namespace std;

int clienteTemperatureSocket;
char buffer2[BUFFER_SIZE];

void exitProgram(){
    cout << "Encerrando programa...\n";
    close(clienteTemperatureSocket);
    exit(EXIT_SUCCESS);
}

void setupClienteSocket(){
    if((clienteTemperatureSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("erro no socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servidorAddr;
    memset(&servidorAddr, 0, sizeof(servidorAddr));
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = inet_addr(IP_SERVIDOR);
    servidorAddr.sin_port = htons(PORTA_SERVIDOR);

    if(connect(clienteTemperatureSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0){
        perror("erro em connect");
        exit(EXIT_FAILURE);
    }
}

void sendRequest(const char* request){
    unsigned int tamanhoMensagem;
    tamanhoMensagem = strlen(request);

    cout << "enviando comando: " << request << endl;
    if(send(clienteTemperatureSocket, request, tamanhoMensagem, 0) != tamanhoMensagem){
        perror("erro ao enviar mensagem");
        exit(EXIT_FAILURE);
    }

    if(strcmp(request, "exit") == 0) exitProgram();
}

char* getServerResponse(){
    int bytesRecebidos;
    int totalBytesRecebidos;

    memset(&buffer2, 0, sizeof(buffer2));

    totalBytesRecebidos = 0;
    if((bytesRecebidos = recv(clienteTemperatureSocket, buffer2, BUFFER_SIZE, 0)) < 0){
        perror("erro ao receber");
        exit(EXIT_FAILURE);
    }

    totalBytesRecebidos += bytesRecebidos;
    buffer2[bytesRecebidos] = '\0';

    char* response = buffer2;
    return response;
}

void sendTemperatureReadingsToServer(){
    setupBME280();

    while(1){
    setupClienteSocket();
    float t, p, u;

        t = get_BME280_reading('t');
        p = get_BME280_reading('p');
        u = get_BME280_reading('u');
        char t_buffer[6];
        char p_buffer[6];
        char u_buffer[6];
        gcvt(t, 4, t_buffer);
        gcvt(p, 4, p_buffer);
        gcvt(u, 4, u_buffer);

        // cout << "T: " << t << ", P: " << p << ", U: " << u << endl;
        char reading[50];
        memset(&reading, 0, sizeof(reading));
        strcat(reading, "Temperatura: ");
        strcat(reading, t_buffer);
        strcat(reading, ", Pressão: ");
        strcat(reading, p_buffer);
        strcat(reading, ", Umidade: ");
        strcat(reading, u_buffer);
        strcat(reading, "\0");

        // enviar a string reading (sem '\0')
        cout << "reading: \n\t" << reading << endl;

        unsigned int tamanhoMensagem;
        tamanhoMensagem = strlen(reading);

        cout << "enviando comando: " << reading << endl;
        if(send(clienteTemperatureSocket, reading, tamanhoMensagem, 0) != tamanhoMensagem){
             perror("erro ao enviar mensagem");
             exit(EXIT_FAILURE);
        }

        usleep(1000000);

        char* serverResponse = getServerResponse();
        cout << "resposta do servidor: " << serverResponse << endl;

        close(clienteTemperatureSocket);
    }
}
