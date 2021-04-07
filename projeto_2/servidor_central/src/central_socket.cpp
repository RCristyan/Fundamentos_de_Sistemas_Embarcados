#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORTA_SERVIDOR 10123
#define IP_SERVIDOR "192.168.0.52"
#define BUFFER_SIZE 50

using namespace std;

int clienteSocket;
struct sockaddr_in servidorAddr;
const char *msg = "ronaldinho";
char buffer[BUFFER_SIZE];
unsigned int tamanhoMensagem;

void setupClienteSocket(){
    if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("erro no socket");
        exit(EXIT_FAILURE);
    }

    memset(&servidorAddr, 0, sizeof(servidorAddr));
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = inet_addr(IP_SERVIDOR);
    servidorAddr.sin_port = htons(PORTA_SERVIDOR);

    if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0){
        perror("erro em connect");
        exit(EXIT_FAILURE);
    }
}

int main(){

    setupClienteSocket();

    tamanhoMensagem = strlen(msg);

    if(send(clienteSocket, msg, tamanhoMensagem, 0) != tamanhoMensagem){
        perror("erro ao enviar mensagem");
        exit(EXIT_FAILURE);
    }

    int bytesRecebidos;
    int totalBytesRecebidos;

    memset(&buffer, 0, sizeof(buffer));

    totalBytesRecebidos = 0;
    while(totalBytesRecebidos < tamanhoMensagem){
        if((bytesRecebidos = recv(clienteSocket, buffer, BUFFER_SIZE, 0)) <= 0){
            perror("erro ao receber");
            exit(EXIT_FAILURE);
        }

        totalBytesRecebidos += bytesRecebidos;
        buffer[bytesRecebidos] = '\0';
        cout << buffer << endl;
    }

    close(clienteSocket);

    return 0;
}
