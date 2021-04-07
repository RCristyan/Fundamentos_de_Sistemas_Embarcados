#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <csignal>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "servidor.h"

#define PORTA_SERVIDOR 10123
#define BUFFER_SIZE 50

using namespace std;

int servidorSocket;
int clienteSocket;
struct sockaddr_in servidorAddr;
struct sockaddr_in clienteAddr;
unsigned int clienteLength;

void closeServer(int signal){
    cout << "\nEncerrando servidor...\n";

    close(clienteSocket);
    close(servidorSocket);
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

void trataClienteTCP(int clienteSocket){
    char buffer[BUFFER_SIZE];
    int tamanhoRecebido;

    if((tamanhoRecebido = recv(clienteSocket, buffer, BUFFER_SIZE, 0)) < 0){
        perror("falha em recv()");
        exit(EXIT_FAILURE);
    }

    buffer[tamanhoRecebido] = '\0';
    
    cout << "Servidor recebeu: ";
    for(int i = 0; buffer[i] != '\0'; i++) cout << buffer[i];
    cout << endl;

    const char* response = "hello from server";

    while(tamanhoRecebido > 0){
        if(send(clienteSocket, response, strlen(response), 0) < 0){
            perror("falha em send");
            exit(EXIT_FAILURE);
        }

        if((tamanhoRecebido = recv(clienteSocket, buffer, BUFFER_SIZE, 0)) < 0){
            perror("falha em recv");
            exit(EXIT_FAILURE);
        }

        buffer[tamanhoRecebido] = '\0';
    }
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

        trataClienteTCP(clienteSocket);
        close(clienteSocket);
    }
}
