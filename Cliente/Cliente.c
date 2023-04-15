#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib") // enlazar con la librería ws2_32.lib

#include "Cliente.h"


//TODO_ LO DE PARTE CLIENTE
int main() {

    /*
    // Inicializar la librería de sockets
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error al inicializar la libreria de sockets: %d", WSAGetLastError());
        return 1;
    }

    // Crear el socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error al crear el socket: %d", WSAGetLastError());
        return 1;
    }

    // Configurar el servidor al que nos conectaremos
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); // dirección IP del servidor
    servidor.sin_port = htons(12345); // puerto del servidor

    // Conectar al servidor
    if (connect(sockfd, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
        printf("Error al conectar con el servidor: %d", WSAGetLastError());
        return 1;
    }

    // Enviar datos al servidor
    printf("Mensaje para enviar al servidor: ");
    fgets(mensaje, 1024, stdin);
    bytes_enviados = send(sockfd, mensaje, strlen(mensaje), 0);
    if (bytes_enviados < 0) {
        printf("Error al enviar datos al servidor: %d", WSAGetLastError());
        return 1;
    }

    // Recibir datos del servidor
    bytes_recibidos = recv(sockfd, respuesta, 1024, 0);
    if (bytes_recibidos < 0) {
        printf("Error al recibir datos del servidor: %d", WSAGetLastError());
        return 1;
    }
    respuesta[bytes_recibidos] = '\0';
    printf("Respuesta recibida del servidor: %s", respuesta);

    // Cerrar el socket y liberar recursos
    closesocket(sockfd);
    WSACleanup();
    return 0;

     */

    HINSTANCE hInstance = GetModuleHandle(NULL);
    HINSTANCE hPrevInstance = NULL;
    LPSTR lpCmdLine = GetCommandLine();
    int nCmdShow = SW_SHOWDEFAULT;

    WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;
}


//TODO_ LO DE LA INTERFAZ

// Declaración de variables globales
HWND g_hwnd;
HBITMAP g_hBitmap;
int g_x = 0;
int g_y = 0;

// Declaración de funciones
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG  msg;
    WNDCLASSW wc = {0};
    wc.lpszClassName = L"Static image";
    wc.hInstance     = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = WndProc;
    wc.hCursor       = LoadCursor(0,IDC_ARROW);


    RegisterClassW(&wc);
    CreateWindowW(wc.lpszClassName, L"Static image",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  100, 100, 330, 270, 0, 0, hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HWND hsti;

    switch(msg) {

        case WM_CREATE:

            LoadMyImage();
            hsti = CreateWindowW(L"Static", L"",
                                 WS_CHILD | WS_VISIBLE | SS_BITMAP,
                                 5, 5, 300, 300, hwnd, (HMENU) 1, NULL, NULL);

            SendMessage(hsti, STM_SETIMAGE,
                        (WPARAM) IMAGE_BITMAP, (LPARAM) hBitmap);
            break;

        case WM_DESTROY:

            DeleteObject(hBitmap);
            PostQuitMessage(0);
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void LoadMyImage(void) {

    hBitmap = LoadImageW(NULL, L"C:\\Users\\DylanG\\Documents\\1.UNIVERSIDAD\\1. Semestres\\5to. Semestre\\1. Compi\\Tarea Paradigma Imperativo OOP\\spaCEinvaders\\Cliente\\imagenes\\nave.bmp", IMAGE_BITMAP,
                         0, 0, LR_LOADFROMFILE);
}