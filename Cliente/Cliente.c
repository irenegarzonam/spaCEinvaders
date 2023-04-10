#include <stdio.h>
#include <stdlib.h>
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

// Definir las constantes para la ventana y la imagen
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define IMAGE_WIDTH 64
#define IMAGE_HEIGHT 64

// Definir la estructura para la imagen
typedef struct {
    int x;
    int y;
} Image;

// Función de controlador de eventos para las teclas de flecha
void handle_arrow_key(Image *image, int keycode) {
    switch (keycode) {
        case VK_LEFT:
            image->x -= 10;
            break;
        case VK_RIGHT:
            image->x += 10;
            break;
        case VK_UP:
            image->y -= 10;
            break;
        case VK_DOWN:
            image->y += 10;
            break;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    HWND hwnd = CreateWindow("STATIC", "Image Moving Program",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
                             NULL, NULL, NULL, NULL);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "ImageMovingProgram";
    RegisterClass(&wc);

    // Crear la imagen
    Image image = { WINDOW_WIDTH/2 - IMAGE_WIDTH/2, WINDOW_HEIGHT/2 - IMAGE_HEIGHT/2 };
    HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, "nave.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBitmap == NULL) {
        MessageBox(NULL, "No se pudo cargar la imagen", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Mostrar la imagen en la ventana
    HDC hdc = GetDC(hwnd);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBitmapOld = SelectObject(hdcMem, hBitmap);
    BitBlt(hdc, image.x, image.y, IMAGE_WIDTH, IMAGE_HEIGHT, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hBitmapOld);
    DeleteDC(hdcMem);

    // Iniciar el bucle principal
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Mover la imagen si se ha presionado una tecla de flecha
        if (msg.message == WM_KEYDOWN) {
            handle_arrow_key(&image, msg.wParam);

            // Actualizar la posición de la imagen en la consola
            printf("Image position: (%d, %d)\n", image.x, image.y);

            // Mostrar la imagen en la nueva posición
            hdc = GetDC(hwnd);
            hdcMem = CreateCompatibleDC(hdc);
            hBitmapOld = SelectObject(hdcMem, hBitmap);
            BitBlt(hdc, image.x, image.y, IMAGE_WIDTH, IMAGE_HEIGHT, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, hBitmapOld);
            DeleteDC(hdcMem);
        }
    }

    // Liberar los recursos
    DeleteObject(hBitmap);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);

    return 0;
}
