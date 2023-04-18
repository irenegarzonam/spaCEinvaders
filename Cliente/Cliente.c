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
int sendToServer(char* message) {
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
                  100, 100, 1400, 700, 0, 0, hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

//Aquí sucede toda la lógica
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
        {
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

            // loop over each element in the matrix and add the x and y coordinates to the Coord array
            int index = 0;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 12; j++) {
                    coords[index].x = matrixAliens[i][j][0]; // add x coordinate to Coord array
                    coords[index].y = matrixAliens[i][j][1]; // add y coordinate to Coord array
                    index++;
                }
            }

            //Carga imagenes en pantalla utilizando las coordenadas en la estructura
            for (int i = 0; i < 60; i++) {
                HWND hsti = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP,
                                          coords[i].x, coords[i].y, 50, 50, hwnd, (HMENU) (i + 1), NULL, NULL);
                int column = i % 5;
                if (column < 1) {
                    coords[i].imagen = hBitmapSquid;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) coords[i].imagen);
                } else if (column < 3) {
                    coords[i].imagen = hBitmapCrab;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) coords[i].imagen);
                } else {
                    coords[i].imagen = hBitmapOctopus;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) coords[i].imagen);
                }
            }
            nave = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, x_jugador, y_jugador, 50, 50, hwnd,
                                 (HMENU) 1, NULL, NULL);
            SendMessage(nave, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) hBitmap);


            // Crear temporizador que actualiza las posiciones de las imágenes cada 500ms
            SetTimer(hwnd, 1, 0.0000001, NULL);
            break;
        }

            //Cuando se cierra la ventana
        case WM_DESTROY:
        {
            DeleteObject(hBitmap);
            DeleteObject(hBitmapOctopus);
            DeleteObject(hBitmapCrab);
            DeleteObject(hBitmapSquid);
            PostQuitMessage(0);
            break;

        }
            //Movimiento de la nave

        case WM_KEYDOWN:
        {
            switch (wParam) {
                case VK_LEFT:
                {
                    x_jugador -= 10;
                    break;
                }
                case VK_RIGHT:
                {
                    x_jugador += 10;
                    break;
                }
                case VK_SPACE:
                {
                    // crear un nuevo disparo en la posición actual del jugador
                    if (num_disparos < 100) {
                        disparo d = {x_jugador + 20, y_jugador - 10, hBitmapDisparoJugador};
                        disparos[num_disparos++] = d;
                    }
                    break;
                }
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

        case WM_TIMER:
        {
            // Actualizar posiciones de las imágenes con las nuevas posiciones de los aliens
            receiveFromServer();
            int index = 0;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 12; j++) {
                    coords[index].x = matrixAliens[i][j][0]; // add x coordinate to Coord array
                    coords[index].y = matrixAliens[i][j][1]; // add y coordinate to Coord array
                    index++;
                }
            }
            for (int i = 0; i < 60; i++) {
                HWND hsti = GetDlgItem(hwnd, i+1);
                //Mover la imagen a la nueva posición utilizando la función MoveWindow
                MoveWindow(hsti, coords[i].x, coords[i].y, 50, 50, TRUE);
            }

            // mover cada disparo hacia arriba
            for (int i = 0; i < num_disparos; i++) {
                disparos[i].y -= 10;
                HWND hstd = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP,
                                          disparos[i].x, disparos[i].y, 10, 30, hwnd, (HMENU) (i + 100), NULL, NULL);
                SendMessage(hstd, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) disparos[i].imagen);
                if (disparos[i].y < -10) {
                    // si el disparo sale de la pantalla, destruir la imagen y reutilizar la estructura disparo
                    DestroyWindow(hstd);
                    disparos[i].y = 0;
                    disparos[i].x = 0;
                    disparos[i].imagen = NULL;
                } else {
                    // detectar colisiones entre el disparo y los aliens
                    for (int j = 0; j < 60; j++) {
                        if (disparos[i].x >= coords[j].x && disparos[i].x <= coords[j].x + 50
                            && disparos[i].y >= coords[j].y && disparos[i].y <= coords[j].y + 50) {
                            // si hay colision, destruir la imagen del alien y reutilizar la estructura disparo

                            if(coords[j].imagen == hBitmapSquid){
                                sendToServer("calamar");
                                puntaje += 10; // Actualizar variable de puntaje
                                char puntaje_texto[10];
                                sprintf(puntaje_texto, "Puntaje: %d", puntaje);
                                SetWindowText(hpuntaje, puntaje_texto); // Actualizar texto de ventana estática de puntaje
                            }

                            if(coords[j].imagen == hBitmapOctopus){
                                sendToServer("pulpo");
                                puntaje += 40; // Actualizar variable de puntaje
                                char puntaje_texto[10];
                                sprintf(puntaje_texto, "Puntaje: %d", puntaje);
                                SetWindowText(hpuntaje, puntaje_texto); // Actualizar texto de ventana estática de puntaje
                            }

                            if(coords[j].imagen == hBitmapCrab){
                                sendToServer("cangrejo");
                                puntaje += 20; // Actualizar variable de puntaje
                                char puntaje_texto[10];
                                sprintf(puntaje_texto, "Puntaje: %d", puntaje);
                                SetWindowText(hpuntaje, puntaje_texto); // Actualizar texto de ventana estática de puntaje
                            }

                            DestroyWindow(GetDlgItem(hwnd, j+1));
                            matrixAliens[j/12][j%12][0] = -100;
                            matrixAliens[j/12][j%12][1] = -100;
                            DestroyWindow(hstd);
                            disparos[i].y = 0;
                            disparos[i].x = 0;
                            disparos[i].imagen = NULL;

                            break;
                        }
                    }
                }
            }
            break;
        }
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void convertStringToVariables(char str[]) {
    char *token = strtok(str, "_");  // Separamos la cadena en la primera subcadena

    char var1[1], var2[1], var3[60], var4[800], var5[100];

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

    lives = atoi(var1);
    score = atoi(var2);

    printf("var1: %d\n", lives);
    printf("var2: %d\n", score);
    //printf("var3: %s\n", var3);


    //MATRIZ 1: --------------Matriz de Escudos------------------------------------------------------------------------------------------------
    //char matrizStr[] = "[[237,500,100],[549,500,100],[861,500,100],[1173,500,100]]"; //Matriz de prueba

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

    // Dividir la cadena en tokens
    char *token2 = strtok(str3, ",");
    int i2 = 0, j3 = 0;

    while (token2 != NULL) {
        // Convertir el token a entero y almacenarlo en la matriz
        matrixBunkers[i2][j3] = atoi(token2);
        // Mover al siguiente elemento de la matriz
        j3++;
        if (j3 == 3) {
            j3 = 0;
            i2++;
        }
        // Obtener el siguiente token
        token2 = strtok(NULL, ",");
    }
    // Imprimir la matriz
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", matrixBunkers[i][j]);
        }
        printf("\n");
    }

    free(str3);

    //---------------------------------------------------------------------------------------------------------------------------------
    //MATRIZ 2: --------------Matriz de Aliens------------------------------------------------------------------------------------------------
    //printf("var4: %s\n", var4);
    //char matrizStr5[] = "[[[60, 75], [140, 75], [220, 75], [300, 75], [380, 75]], [[60, 155], [140, 155], [220, 155], [300, 155], [380, 155]], [[60, 235], [140, 235], [220, 235], [300, 235], [380, 235]], [[60, 315], [140, 315], [220, 315], [300, 315], [380, 315]], [[60, 395], [140, 395], [220, 395], [300, 395], [380, 395]]]";

    // Eliminar los corchetes externos
    char* matrizStr6 = var4 + 2;
    matrizStr6[strlen(matrizStr6) - 2] = '\0';

    // Eliminar los corchetes internos y los espacios en blanco
    char* matrizStr7 = strdup(matrizStr6);
    char* p6 = matrizStr7;
    while (*p6) {
        if (*p6 == '[' || *p6 == ']' || *p6 == ' ') {
            memmove(p6, p6 + 1, strlen(p6));
        } else {
            ++p6;
        }
    }

    // Eliminar los corchetes internos y los espacios en blanco (segunda capa)
    char* matrizStr8 = strdup(matrizStr7);
    p6 = matrizStr8;
    while (*p6) {
        if (*p6 == '[' || *p6 == ']' || *p6 == ' ') {
            memmove(p6, p6 + 1, strlen(p6));
        } else {
            ++p6;
        }
    }
    // Dividir la cadena en tokens
    char* token3 = strtok(matrizStr8, ",");
    int i1 = 0, j2 = 0, k = 0;

    while (token3 != NULL) {
        // Convertir el token a entero y almacenarlo en la matriz
        matrixAliens[i1][j2][k] = atoi(token3);

        // Mover al siguiente elemento de la matriz
        k++;
        if (k == 2) {
            k = 0;
            j2++;
            if (j2 == 12) {
                j2 = 0;
                i1++;
            }
        }

        // Obtener el siguiente token
        token3 = strtok(NULL, ",");
    }

    // Imprimir la matriz
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 12; j++) {
            printf("(%d, %d) ", matrixAliens[i][j][0], matrixAliens[i][j][1]);
        }
        printf("\n");
    }

    free(matrizStr7);
    free(matrizStr8);


    printf("var5: %s\n", var5);

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

    hBitmapDisparoJugador = LoadImageW(NULL,
                                       L"imagenes\\disparo_jugador.bmp",
                                       IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapDisparoAlien = LoadImageW(NULL,
                                       L"imagenes\\disparo_alien.bmp",
                                       IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

