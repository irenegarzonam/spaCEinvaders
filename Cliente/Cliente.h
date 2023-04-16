#ifndef CLIENTE_CLIENTE_H
#define CLIENTE_CLIENTE_H

//Parte Cliente
WSADATA wsa;
SOCKET sockfd;
struct sockaddr_in servidor;
char mensaje[1024], respuesta[1024];
int bytes_enviados, bytes_recibidos;

//Parte Interfaz
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoadMyImage(void);
HBITMAP hBitmap;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LoadMyImage(void);

// Variables globales
HBITMAP hBitmap;
int matrix[15][2] = {
        {0,0},{0,100},{0,200},
        {80,0},{80,100},{80,200},
        {160,0},{160,100},{160,200},
        {240,0},{240,100},{240,200},
        {320,0},{320,100},{320,200},
};
HBITMAP hBitmap1, hBitmap2, hBitmap3;

#endif //CLIENTE_CLIENTE_H
