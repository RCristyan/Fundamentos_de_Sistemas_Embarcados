#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "central_socket.h"

#define PORTA_SERVIDOR 10123
#define IP_SERVIDOR "192.168.0.4"
#define BUFFER_SIZE 50

using namespace std;

int clienteSocket;
char buffer[BUFFER_SIZE];

void exitProgram(){
    cout << "Encerrando programa...\n";
    close(clienteSocket);
    exit(EXIT_SUCCESS);
}

void setupClienteSocket(){
    if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("erro no socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servidorAddr;
    memset(&servidorAddr, 0, sizeof(servidorAddr));
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = inet_addr(IP_SERVIDOR);
    servidorAddr.sin_port = htons(PORTA_SERVIDOR);

    if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0){
        perror("erro em connect");
        exit(EXIT_FAILURE);
    }
}

void sendRequest(const char* request){
    unsigned int tamanhoMensagem;
    tamanhoMensagem = strlen(request);

    cout << "enviando comando: " << request << endl;
    if(send(clienteSocket, request, tamanhoMensagem, 0) != tamanhoMensagem){
        perror("erro ao enviar mensagem");
        exit(EXIT_FAILURE);
    }
    cout << "comando enviado\n";

    if(strcmp(request, "exit") == 0) exitProgram();
}

char* getServerResponse(){
    int bytesRecebidos;
    int totalBytesRecebidos;

    memset(&buffer, 0, sizeof(buffer));

    totalBytesRecebidos = 0;
    if((bytesRecebidos = recv(clienteSocket, buffer, BUFFER_SIZE, 0)) < 0){
        perror("erro ao receber");
        exit(EXIT_FAILURE);
    }

    totalBytesRecebidos += bytesRecebidos;
    buffer[bytesRecebidos] = '\0';

    char* response = buffer;
    return response;
}

void requestToServer(const char* msg){
    setupClienteSocket();
    
    sendRequest(msg);
    char* serverResponse = getServerResponse();
    cout << "resposta do servidor: " << serverResponse << endl;

    close(clienteSocket);
}
