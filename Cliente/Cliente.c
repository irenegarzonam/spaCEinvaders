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

// Declaración de variables globales
HWND g_hwnd;
HBITMAP g_hBitmap;
int g_x = 0;
int g_y = 0;

// Declaración de funciones
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Definición de variables locales
    WNDCLASS wc = {0};
    MSG msg = {0};
    HWND hwnd;
    HDC hdc;
    BITMAP bm;
    HINSTANCE hinst;
    HBITMAP hbmp;
    BITMAPFILEHEADER hdr;
    BITMAPINFOHEADER bmi;
    RGBQUAD rgb[256];
    DWORD dwBytesRead;
    HANDLE hFile;

    // Registro de la clase de ventana
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = TEXT("MyClass");
    RegisterClass(&wc);

    // Creación de la ventana
    hwnd = CreateWindow(TEXT("MyClass"), TEXT("Moving Image"), WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    // Carga de la imagen desde un archivo BMP
    hFile = CreateFile(TEXT("nave.bmp"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_hBitmap == NULL) {
        MessageBox(hwnd, "Failed to load image", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    ReadFile(hFile, &hdr, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
    ReadFile(hFile, &bmi, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
    ReadFile(hFile, rgb, sizeof(RGBQUAD) * bmi.biClrUsed, &dwBytesRead, NULL);
    hbmp = CreateDIBSection(NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, (void**)&hdc, NULL, 0);
    ReadFile(hFile, hdc, bmi.biSizeImage, &dwBytesRead, NULL);
    CloseHandle(hFile);

    // Configuración de variables globales
    g_hwnd = hwnd;
    g_hBitmap = hbmp;

    // Bucle principal de mensajes
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Liberación de recursos
    DeleteObject(hbmp);

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Definición de variables locales
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    int key;

    switch (msg) {
        case WM_TIMER:
        {
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                g_x -= 10;
            }
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                g_x += 10;
            }
            if (GetAsyncKeyState(VK_UP) & 0x8000) {
                g_y -= 10;
            }
            if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                g_y += 10;
            }

            // Redibujado de la imagen
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

            // Redibujado de la imagen
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case WM_PAINT:
            // Dibujo de la imagen en la ventana
            hdc = BeginPaint(hwnd, &ps);
            SelectObject(hdc, g_hBitmap);
            GetClientRect(hwnd, &rect);
            StretchBlt(hdc, g_x, g_y, rect.right, rect.bottom, hdc, 0, 0, rect.right, rect.bottom, SRCCOPY);
            EndPaint(hwnd, &ps);

            // Impresión de la posición de la imagen en la consola
            printf("Posición de la imagen: (%d, %d)\n", g_x, g_y);
            break;
        case WM_DESTROY:
            // Detención del temporizador y cierre de la ventana
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
