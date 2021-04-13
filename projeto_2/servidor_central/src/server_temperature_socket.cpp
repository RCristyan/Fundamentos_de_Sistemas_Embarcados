#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <csignal>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORTA_SERVIDOR 10023
#define BUFFER_SIZE 50

using namespace std;

int servidorSocket;
int clienteTemperatureSocket;
struct sockaddr_in servidorAddr;
struct sockaddr_in clienteAddr;
unsigned int clienteLength;
char buffer2[BUFFER_SIZE];
char server_response[50];

void closeServer(int signal){
    cout << "\nEncerrando servidor...\n";

    close(clienteTemperatureSocket);
    close(servidorSocket);
    exit(EXIT_SUCCESS);
}

void setupServer(){
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
}

void playAlarmSound(){
    cout << "deve reproduzir som de alarme\n";
    closeServer(SIGINT);
}

void tratalienteTemperatureSocket(int clienteTemperatureSocket){
    int tamanhoRecebido;

    if((tamanhoRecebido = recv(clienteTemperatureSocket, buffer2, BUFFER_SIZE, 0)) < 0){
        perror("falha em recv()");
        exit(EXIT_FAILURE);
    }

    buffer2[tamanhoRecebido] = '\0';

    cout << "Servidor recebeu: ";
    for(int i = 0; buffer2[i] != '\0'; i++) cout << buffer2[i];
    cout << endl;

    if(strcmp(buffer2, "brecha de segurança") == 0){
        strcpy(server_response, "aviso de brecha de segurança recebido");
    }

    strcpy(server_response, "dados recebidos");

    if(send(clienteTemperatureSocket, server_response, strlen(server_response), 0) < 0){
        perror("falha em send");
        exit(EXIT_FAILURE);
    }

    memset(&server_response, 0, sizeof(server_response));
    
    if(strcmp(buffer2, "brecha de segurança") == 0){
        cout << "brecha de segurança detectada. Disparando alarme\n";
        playAlarmSound();
    }
}

void waitRequest(){
    while(1){
        clienteLength = sizeof(clienteAddr);
        if((clienteTemperatureSocket = accept(servidorSocket,
                                    (struct sockaddr *) &clienteAddr,
                                    &clienteLength)) < 0){
            perror("falha no accept");
            exit(EXIT_FAILURE);
        }

        cout << "conexão do cliente " << inet_ntoa(clienteAddr.sin_addr) << endl;

        tratalienteTemperatureSocket(clienteTemperatureSocket);

        close(clienteTemperatureSocket);
    }
}
