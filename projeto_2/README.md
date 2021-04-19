# Projeto 2 - 2020/2

Projeto 2 da disciplina de Fundamentos de Sistemas Embarcados (2020/2)

O enunciado do projeto 2 pode ser acessado em https://gitlab.com/fse_fga/projetos_2020_2/projeto-2-2020.2

## Objetivo

O projeto se trata da implementação de um sistema de automação residencial para monitoramento de sensores. O sistema é composto por duas partes, o servidor central que faz a interface com o usuário e o servidor distribuido que tem acesso aos diversos sensores.

A comunicação entre as partes é realizada utilizando sockets cliente-servidor TCP/IP. O servidor distribuido está conectado a 4 lâmpadas, 2 ar-condicionados e 8 sensores de presença através dos pinos GPIO. Além disso, ele tem acesso a um sensor de temperatura BME280 utilizando o barramento I2C.

O principal objetivo deste trabalho é fornecer ao usuário do servidor central meios para controlar remotamente os dispositivos conectados ao servidor distribuido, além de receber informações sobre temperatura e uma mensagem de alarme caso um dos sensores seja ativado.

## Como executar o programa
Para executar o programa, compile e rode primeiramente o código do servidor central com
```
    cd servidor_central
    make run
```

Logo após, compile e rode o código do servidor distribuido
```
    cd servidor_distribuido
    make run
```
