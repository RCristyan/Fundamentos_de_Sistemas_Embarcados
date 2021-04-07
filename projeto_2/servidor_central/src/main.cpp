#include <iostream>

using namespace std;

void requisitar_ligar_equipamento(int comando){
    switch (comando){
    case 1:
        cout << "Você escolheu 1\n";
        break;

    case 2:
        cout << "Você escolheu 2\n";
        break;

    case 3:
        cout << "Você escolheu 3\n";
        break;

    case 4:
        cout << "Você escolheu 4\n";
        break;
    
    default:
        break;
    }
}

int menu(){
    cout << "---------------------------\n";

    cout << "Bem vindo. Escolha uma das opções abaixo:\n";
    cout << "1. Ligar lampada 1\n";
    cout << "2. Ligar lampada 2\n";
    cout << "3. Ligar lampada 3\n";
    cout << "4. Ligar lampada 4\n";

    cout << ">";

    int option;
    cin >> option;
    return option;
}

int main(){

    int comando = menu();
    requisitar_ligar_equipamento(comando);

    return 0;
}