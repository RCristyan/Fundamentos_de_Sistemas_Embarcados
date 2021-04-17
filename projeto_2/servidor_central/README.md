# Projeto 2 - 2020/2
## Servidor Central

Neste repositório estão os arquivos necessários para executar o código do servidor central.
Ele fornece uma interface onde o usuário irá receber leituras de temperatura, pressão e umidade
do servidor distribuido a cada 1 segundo.

Caso um dos sensores de presença for ativado, um alarme será disparado pelo próprio servidor central.

Além disso, é possível controlar remotamente as lampadas e os ar-condicionados.
Pressione 'ctrl + \\' a qualquer momento para exibir o menu de interação, e depois escolha uma
das opções.

As opções disponíveis são:
```
    1.  Ligar lampada 1
    2.  Ligar lampada 2
    3.  Ligar lampada 3
    4.  Ligar lampada 4
    5.  Ligar ar condicionado 1
    6.  Ligar ar condicionado 2
    
    7.  Desligar lampada 1
    8.  Desligar lampada 2
    9.  Desligar lampada 3
    10. Desligar lampada 4
    11. Desligar ar condicionado 1
    12. Desligar ar condicionado 2

    0. Sair
```

Lembrete: use
```
    make run
```
para compilar e executar o código.