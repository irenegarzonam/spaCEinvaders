#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib") // enlazar con la librería ws2_32.lib

#include "Cliente.h"

//TODO_ LO DE PARTE CLIENTE
// Global variables
WSADATA wsa;
SOCKET sockfd;
struct sockaddr_in servidor;
char mensaje[1024], respuesta[1024];
int bytes_enviados, bytes_recibidos;

// Connect to server method
int connectToServer() {
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
    return 0;
}

// Receive from server method
int receiveFromServer() {
    // Recibir datos del servidor
    bytes_recibidos = recv(sockfd, respuesta, 1024, 0);
    if (bytes_recibidos < 0) {
        printf("Error al recibir datos del servidor: %d", WSAGetLastError());
        return 1;
    }
    respuesta[bytes_recibidos] = '\0';
    printf("Respuesta recibida del servidor: %s", respuesta);
    convertStringToVariables(respuesta);
    return 0;
}

// Send to server method
int sendToServer(char *message) {
    bytes_enviados = send(sockfd, message, strlen(message), 0);
    if (bytes_enviados < 0) {
        printf("Error al enviar datos al servidor: %d", WSAGetLastError());
        return 1;
    }
    return 0;
}

// Close connection method
int closeConnection() {
    // Cerrar el socket y liberar recursos
    closesocket(sockfd);
    WSACleanup();
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
    static HWND hvida;
    static HWND hpuntaje;
    static int x_jugador = 546;
    static int y_jugador = 605;
    static int x[60], y[60];

    switch (msg) {
        case WM_CREATE:

            hvida = CreateWindowW(L"Static", L"Vidas: 2", WS_CHILD | WS_VISIBLE,
                                  10, 630, 100, 30, hwnd, NULL, NULL, NULL);
            hpuntaje = CreateWindowW(L"Static", L"Puntaje: 0", WS_CHILD | WS_VISIBLE,
                                     120, 630, 100, 30, hwnd, NULL, NULL, NULL);


            LoadMyImage();
            /* La idea aquí es llamar Cliente(1) para así tener la interfaz, y después tener un temporizador que cada X tiempo
             * actualice la matriz, o bueno, la sobreescriba xd
             */
            connectToServer();
            receiveFromServer();


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
                    break;
                case VK_RIGHT:
                    x_jugador += 10;
                    break;
                    /*
                case VK_UP:
                    y_jugador -= 10;
                    break;
                case VK_DOWN:
                    y_jugador += 10;
                    break;
                     */
                case VK_SPACE:
                    vidas--; // Actualizar variable de vidas
                    char vidas_texto[10];
                    sprintf(vidas_texto, "Vidas: %d", vidas);
                    SetWindowText(hvida, vidas_texto); // Actualizar texto de ventana estática de vidas

                    puntaje++; // Actualizar variable de puntaje
                    char puntaje_texto[10];
                    sprintf(puntaje_texto, "Puntaje: %d", puntaje);
                    SetWindowText(hpuntaje, puntaje_texto); // Actualizar texto de ventana estática de puntaje
                default:
                    break;
            }
            //Límite para que no se salga
            if (x_jugador > 26 && x_jugador < 1166) {
                SetWindowPos(nave, NULL, x_jugador, y_jugador, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
                printf("Posicion de la nave x: %d , y: %d \n", x_jugador, y_jugador);
            }
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void convertStringToVariables(char str[]) {
    char *token = strtok(str, "_");  // Separamos la cadena en la primera subcadena

    char var1[1], var2[1], var3[60], var4[302], var5[100];

    if (token != NULL) {
        strncpy(var1, token, sizeof(var1));  // Copiamos la primera subcadena a la variable var1
        token = strtok(NULL, "_");  // Separamos la cadena en la siguiente subcadena

        if (token != NULL) {
            strncpy(var2, token, sizeof(var2));  // Copiamos la segunda subcadena a la variable var2
            token = strtok(NULL, "_");

            if (token != NULL) {
                strncpy(var3, token, sizeof(var3));
                token = strtok(NULL, "_");

                if (token != NULL) {
                    strncpy(var4, token, sizeof(var4));
                    token = strtok(NULL, "_");

                    if (token != NULL) {
                        strncpy(var5, token, sizeof(var5));
                    }
                }
            }
        }
    }

    printf("var1: %s\n", var1);
    printf("var2: %s\n", var2);
    printf("var3: %s\n", var3);


    //MATRIZ 1: -----------------------------------------------------------------------------------------------------------------------
    char matrizStr[] = "[[237,500,100],[549,500,100],[861,500,100],[1173,500,100]]"; //Matriz de prueba

    // Eliminar los corchetes externos
    char *str2 = var3 + 1;
    str2[strlen(str2) - 1] = '\0';

    // Eliminar los corchetes internos y los espacios en blanco
    char *str3 = strdup(str2);
    char *p = str3;
    while (*p) {
        if (*p == '[' || *p == ']' || *p == ' ') {
            memmove(p, p + 1, strlen(p));
        } else {
            ++p;
        }
    }

    // Inicializar la matriz
    int matriz[4][3];

    // Dividir la cadena en tokens
    char *token2 = strtok(str3, ",");
    int i = 0, j = 0;

    while (token2 != NULL) {
        // Convertir el token a entero y almacenarlo en la matriz
        matriz[i][j] = atoi(token2);
        // Mover al siguiente elemento de la matriz
        j++;
        if (j == 3) {
            j = 0;
            i++;
        }
        // Obtener el siguiente token
        token2 = strtok(NULL, ",");
    }
    // Imprimir la matriz
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }

    free(str3);

    //---------------------------------------------------------------------------------------------------------------------------------

    printf("var4: %s\n", var4);
    printf("var5: %s\n", var5);
}

int convertMatrix(char matrizString[]) {
    int matriz[60][2];

    char *ptr = matrizString + 1; // saltamos el primer corchete

    for (int i = 0; i < 60; i++) {
        sscanf(ptr, "%d, %d", &matriz[i][0], &matriz[i][1]);
        ptr += 7; // avanzamos 7 caracteres para saltar la coma y el espacio
    }

    return 0;
}

//AUX que tiene dirección de imagenes, tiene que cambiarlo en su PC porque son direcciones desde el disco, no desde root, es un error que hay que escribir en docu externa
void LoadMyImage(void) {

    hBitmapOctopus = LoadImageW(NULL,
                                L"imagenes\\pulpo.bmp",
                                IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapCrab = LoadImageW(NULL,
                             L"imagenes\\cangrejo.bmp",
                             IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapSquid = LoadImageW(NULL,
                              L"imagenes\\calamar.bmp",
                              IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmap = LoadImageW(NULL,
                         L"imagenes\\nave.bmp",
                         IMAGE_BITMAP,
                         0, 0, LR_LOADFROMFILE);

}

