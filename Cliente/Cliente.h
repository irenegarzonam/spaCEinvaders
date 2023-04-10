#ifndef CLIENTE_CLIENTE_H
#define CLIENTE_CLIENTE_H

//Parte Cliente
WSADATA wsa;
SOCKET sockfd;
struct sockaddr_in servidor;
char mensaje[1024], respuesta[1024];
int bytes_enviados, bytes_recibidos;

//adasd

#endif //CLIENTE_CLIENTE_H
