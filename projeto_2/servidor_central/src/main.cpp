#include <iostream>
#include "central_socket.h"

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
    
    default:
        cout << "comando inválido! tente novamente\n";
        break;
    }
}

void menu(){
    cout << "---------------------------\n";

    cout << "Bem vindo. Escolha uma das opções abaixo:\n";
    cout << "1. Ligar lampada 1\n";
    cout << "2. Ligar lampada 2\n";
    cout << "3. Ligar lampada 3\n";
    cout << "4. Ligar lampada 4\n";
    cout << "5. Ligar ar condicionado 1\n";
    cout << "6. Ligar ar condicionado 2\n";
    cout << "0. Sair\n";
}

int getUserInput(){
    cout << "> ";

    int option;
    cin >> option;
    return option;
}

int main(){

    menu();

    while(1){
        int comando = getUserInput();
        requisitar_ligar_equipamento(comando);
    }

    return 0;
}