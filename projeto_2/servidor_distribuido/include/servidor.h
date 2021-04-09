#ifndef __SERVIDOR__
#define __SERVIDOR__

void setupServer();
void closeServer(int signal);
void handleRequisiton();
void trataClienteSocket(int clienteSocket);
void waitRequest();

#endif