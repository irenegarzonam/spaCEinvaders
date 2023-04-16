#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <unistd.h>

#pragma comment(lib, "ws2_32.lib") // enlazar con la librería ws2_32.lib

#include "Cliente.h"

//TODO_ LO DE PARTE CLIENTE
int Cliente(int comando) {

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
    memset(&servidor, '0', sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); // dirección IP del servidor
    servidor.sin_port = htons(12345); // puerto del servidor

    // Conectar al servidor
    if (connect(sockfd, (struct sockaddr *) &servidor, sizeof(servidor)) < 0) {
        printf("Error al conectar con el servidor: %d", WSAGetLastError());
        return 1;
    }

    /* Lista de comandos
     * La idea aquí, es que di, por cada comando pida:
     * 1: matriz para actualizarla
     * 2: el resto que son vidas, temporizador, etc. xd
     * De igual manera, aquí es donde está sucediendo eso que le digo, Commando == 1 funciona porque manda algo de consola, pero commando 2 no funciona
     */

    if (comando == 1) {
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
        printf("Respuesta recibida del servidor desde COmando 1: %s", respuesta);
    }

    if (comando == 2) {
        char message[50];
        strcpy(message, "Hola desde el cliente en C");
        bytes_enviados = send(sockfd, message, strlen(message), 0);
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
        printf("Respuesta recibida del servidor desde COMando 2: %s", respuesta);
    }

    // Cerrar el socket y liberar recursos
    closesocket(sockfd);
    WSACleanup();


    HINSTANCE hInstance = GetModuleHandle(NULL);
    HINSTANCE hPrevInstance = NULL;
    LPSTR lpCmdLine = GetCommandLine();
    int nCmdShow = SW_SHOWDEFAULT;

    return 0;

}


//TODO_ LO DE LA INTERFAZ

// Declaración de funciones
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Código para definir y crear la ventana, y solo la ventana, tipo base
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASSW wc = {0};
    wc.lpszClassName = L"Static image";
    wc.hInstance = hInstance;
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);


    RegisterClassW(&wc);
    CreateWindowW(wc.lpszClassName, L"Static image",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  100, 100, 1250, 700, 0, 0, hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

//Aquí sucede toda la lógica
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hsti[60];
    static HWND nave;
    static int x_jugador = 546;
    static int y_jugador = 605;
    static int x[60], y[60];
    switch (msg) {

        case WM_CREATE:

            LoadMyImage();
            /* La idea aquí es llamar Cliente(1) para así tener la interfaz, y después tener un temporizador que cada X tiempo
             * actualice la matriz, o bueno, la sobreescriba xd
             */
            Cliente(1);

            //Carga imagenes en pantalla recorriendo la matriz
            for (int i = 0; i < 60; i++) {
                x[i] = matrix[i][0];
                y[i] = matrix[i][1];
                hsti[i] = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP,
                                        x[i], y[i], 50, 50, hwnd, (HMENU) (i + 1), NULL, NULL);
                int column = i % 5;
                if (column < 1) {
                    SendMessage(hsti[i], STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) hBitmapSquid);
                } else if (column < 3) {
                    SendMessage(hsti[i], STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) hBitmapCrab);
                } else {
                    SendMessage(hsti[i], STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) hBitmapOctopus);
                }
            }


            nave = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, x_jugador, y_jugador, 50, 50, hwnd,
                                 (HMENU) 1, NULL, NULL);
            SendMessage(nave, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) hBitmap);
            break;

            //Cuando se cierra la ventana
        case WM_DESTROY:

            DeleteObject(hBitmap);
            DeleteObject(hBitmapOctopus);
            DeleteObject(hBitmapCrab);
            DeleteObject(hBitmapSquid);
            PostQuitMessage(0);
            break;

            //Movimiento de la nave
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_LEFT:
                    x_jugador -= 10;
                    Cliente(2);
                    break;
                case VK_RIGHT:
                    x_jugador += 10;
                    break;
                case VK_UP:
                    y_jugador -= 10;
                    break;
                case VK_DOWN:
                    y_jugador += 10;
                    break;
                default:
                    break;
            }
            //Límite para que no se salga
            if (x_jugador > 26 && x_jugador < 1166) {
                SetWindowPos(nave, NULL, x_jugador, y_jugador, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
                printf("Posicion del alien x: %d , y: %d \n", x_jugador, y_jugador);
            }
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

//AUX que tiene dirección de imagenes, tiene que cambiarlo en su PC porque son direcciones desde el disco, no desde root, es un error que hay que escribir en docu externa
void LoadMyImage(void) {

    hBitmapOctopus = LoadImageW(NULL,
                                L"C:\\Users\\DylanG\\Documents\\1.UNIVERSIDAD\\1. Semestres\\5to. Semestre\\1. Compi\\Tarea Paradigma Imperativo OOP\\spaCEinvaders\\Cliente\\imagenes\\pulpo.bmp",
                                IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapCrab = LoadImageW(NULL,
                             L"C:\\Users\\DylanG\\Documents\\1.UNIVERSIDAD\\1. Semestres\\5to. Semestre\\1. Compi\\Tarea Paradigma Imperativo OOP\\spaCEinvaders\\Cliente\\imagenes\\cangrejo.bmp",
                             IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapSquid = LoadImageW(NULL,
                              L"C:\\Users\\DylanG\\Documents\\1.UNIVERSIDAD\\1. Semestres\\5to. Semestre\\1. Compi\\Tarea Paradigma Imperativo OOP\\spaCEinvaders\\Cliente\\imagenes\\calamar.bmp",
                              IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmap = LoadImageW(NULL,
                         L"C:\\Users\\DylanG\\Documents\\1.UNIVERSIDAD\\1. Semestres\\5to. Semestre\\1. Compi\\Tarea Paradigma Imperativo OOP\\spaCEinvaders\\Cliente\\imagenes\\nave.bmp",
                         IMAGE_BITMAP,
                         0, 0, LR_LOADFROMFILE);
}

