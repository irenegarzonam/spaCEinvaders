#include <stdio.h>
#include <windows.h>
#include "Cliente.h"

#pragma comment(lib, "ws2_32.lib") // enlazar con la librería ws2_32.lib


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

// Función principal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Registro de la clase de ventana
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    // Creación de la ventana
    hWindow = CreateWindow("MyWindowClass", "My Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
                           NULL, NULL, hInstance, NULL);

    // Creación del botón
    hButton = CreateWindow("BUTTON", "Click me", WS_VISIBLE | WS_CHILD, 10, 10, 100, 30, hWindow, NULL, hInstance,
                           NULL);

    // Creación de la caja de texto
    hTextbox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 10, 50, 300, 20, hWindow,
                            NULL, hInstance, NULL);

    // Carga de la imagen


    // Mostrar ventana
    ShowWindow(hWindow, nCmdShow);

    // Bucle principal de mensajes
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Procedimiento de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            if (lParam == (LPARAM) hButton) {
                OnButtonClick();
            }
            return 0;

        case WM_PAINT:
            OnPaint(hwnd);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Función llamada cuando se presiona el botón
void OnButtonClick() {
    char text[256];
    GetWindowText(hTextbox, text, 256);
    MessageBox(hWindow, text, "Texto ingresado", MB_OK);
}

void OnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Dibujar imagen
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBitmapOld = (HBITMAP) SelectObject(hdcMem, hBitmap);

    BITMAP bitmap;
    GetObject(hBitmap, sizeof(bitmap), &bitmap);

    StretchBlt(hdc, 0, yPos, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

    SelectObject(hdcMem, hBitmapOld);
    DeleteDC(hdcMem);

    // Mover imagen
    yPos += 10;
    if (yPos > ps.rcPaint.bottom) {
        yPos = -bitmap.bmHeight;
    }

    EndPaint(hwnd, &ps);
}

HBITMAP LoadImageFromResource(const wchar_t* resourceName) {
    HBITMAP hBitmap = NULL;

    // Obtener identificador del módulo
    HMODULE hModule = GetModuleHandle(NULL);

    // Cargar imagen desde archivo en carpeta raíz del programa
    HBITMAP hLoadedBitmap = (HBITMAP) LoadImage(hModule, resourceName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hLoadedBitmap != NULL) {
        // Crear una copia compatible de la imagen cargada
        HDC hdcMem = CreateCompatibleDC(NULL);
        if (hdcMem != NULL) {
            BITMAP bitmap;
            GetObject(hLoadedBitmap, sizeof(bitmap), &bitmap);

            hBitmap = CreateCompatibleBitmap(hdcMem, bitmap.bmWidth, bitmap.bmHeight);

            if (hBitmap != NULL) {
                HBITMAP hBitmapOld = (HBITMAP) SelectObject(hdcMem, hBitmap);

                // Copiar la imagen cargada en la copia compatible
                BitBlt(hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

                SelectObject(hdcMem, hBitmapOld);
            }

            DeleteDC(hdcMem);
        }

        DeleteObject(hLoadedBitmap);
    }

    return hBitmap;
}


