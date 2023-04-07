#ifndef CLIENTE_CLIENTE_H
#define CLIENTE_CLIENTE_H

//Parte Cliente
WSADATA wsa;
SOCKET sockfd;
struct sockaddr_in servidor;
char mensaje[1024], respuesta[1024];
int bytes_enviados, bytes_recibidos;

//Parte Interfaz

// Declaración de variables globales
HWND hWindow, hButton, hTextbox; // Handles de ventana, botón y caja de texto
HBITMAP hBitmap; // Handle de imagen
int yPos = 0; // Posición inicial de la imagen

// Declaración de funciones
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnButtonClick();
void OnPaint(HWND hwnd);
HBITMAP LoadImageFromFile(const wchar_t* filePath);
HBITMAP LoadImageFromResource(const wchar_t* resourceName);

#endif //CLIENTE_CLIENTE_H
