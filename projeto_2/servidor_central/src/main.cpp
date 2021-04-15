#include <iostream>
#include <csignal>
#include <thread>
#include "central_socket.h"
#include "server_temperature_socket.h"
#include "logger.h"

using namespace std;

void requisitar_ligar_equipamento(int comando){
    switch (comando){
    case 0:
        requestToServer("exit");
        break;
    case 1:
        requestToServer("ligar lampada 01");
        break;

    case 2:
        requestToServer("ligar lampada 02");
        break;

    case 3:
        requestToServer("ligar lampada 03");
        break;

    case 4:
        requestToServer("ligar lampada 04");
        break;
    
    case 5:
        requestToServer("ligar ar condicionado 01");
        break;
    
    case 6:
        requestToServer("ligar ar condicionado 02");
        break;
    
    case 7:
        requestToServer("desligar lampada 01");
        break;
    
    case 8:
        requestToServer("desligar lampada 02");
        break;
    
    case 9:
        requestToServer("desligar lampada 03");
        break;
    
    case 10:
        requestToServer("desligar lampada 04");
        break;
    
    case 11:
        requestToServer("desligar ar condicionado 01");
        break;
    
    case 12:
        requestToServer("desligar ar condicionado 02");
        break;

    default:
        cout << "comando inválido! tente novamente\n";
        break;
    }
}

void menu(){
    cout << "\n---------------------------\n";

    cout << "Bem vindo. Escolha uma das opções abaixo:\n";
    cout << "1.  Ligar lampada 1\n";
    cout << "2.  Ligar lampada 2\n";
    cout << "3.  Ligar lampada 3\n";
    cout << "4.  Ligar lampada 4\n";
    cout << "5.  Ligar ar condicionado 1\n";
    cout << "6.  Ligar ar condicionado 2\n";
    
    cout << endl;
    
    cout << "7.  Desligar lampada 1\n";
    cout << "8.  Desligar lampada 2\n";
    cout << "9.  Desligar lampada 3\n";
    cout << "10. Desligar lampada 4\n";
    cout << "11. Desligar ar condicionado 1\n";
    cout << "12. Desligar ar condicionado 2\n";

    cout << endl;
    
    cout << "0. Sair\n";
}

int getUserInput(){
    cout << "> ";

    int option;
    cin >> option;
    return option;
}

void requests(){
    waitRequest();
}

void handleUserRequisition(int signal){
    setStopPrinting(true);

    menu();

    int comando = getUserInput();
    requisitar_ligar_equipamento(comando);

    setStopPrinting(false);
}

int main(){

    signal(SIGQUIT, handleUserRequisition);

    setupServer();
    thread request_listener(requests);
    request_listener.join();

    return 0;
}
