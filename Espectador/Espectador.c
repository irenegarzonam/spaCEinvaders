#include <stdio.h>
#include <windows.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib") // enlazar con la librería ws2_32.lib

#include "Espectador.h"

//TODO_ LO DE PARTE CLIENTE

/**
 * @brief Ejecución del servidor
 *
 * @return valores de salida en caso de error
 */
int Cliente() {
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
    servidor.sin_port = htons(8888); // puerto del servidor

    // Conectar al servidor
    if (connect(sockfd, (struct sockaddr *) &servidor, sizeof(servidor)) < 0) {
        printf("Error al conectar con el servidor: %d", WSAGetLastError());
        return 1;
    }

    // Enviar datos al servidor

    sprintf(mensaje, "A:%d:%d:%d:%d:%d:%d:%d", 0, 0, 0, 0, x_jugador, 0,
            0); //ESTRUCTURA= A: Alien_x : Alien_y : TipoAlien : NumBunker : X_jugador  : X_disparo : Y_disparo
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
    convertStringToVariables(respuesta);
    printf("Respuesta recibida del servidor: %s \n", respuesta);

    // Cerrar el socket y liberar recursos
    closesocket(sockfd);
    WSACleanup();

    return 0;
}

//TODO_ LO DE LA INTERFAZ


// Declaración de funciones
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**
 * @brief Función principal que llama la ventana
 * @param hInstance
 * @param hPrevInstance
 * @param lpCmdLine
 * @param nCmdShow
 * @return callback de la función
 */
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

/**
 * @brief Lógica de la ventana
 * @param hwnd
 * @param msg
 * @param wParam
 * @param lParam
 * @return ventana
 */
//Aquí sucede toda la lógica
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            hvida = CreateWindowW(L"Static", L"Vidas: 2", WS_CHILD | WS_VISIBLE,
                                  10, 630, 100, 30, hwnd, NULL, NULL, NULL);
            hpuntaje = CreateWindowW(L"Static", L"Puntaje: 0", WS_CHILD | WS_VISIBLE,
                                     120, 630, 100, 30, hwnd, NULL, NULL, NULL);
            LoadMyImage();
            sprintf(mensaje, "A:%d:%d:%d:%d:%d:%d:%d", 0, 0, 0, 0, x_jugador, 0,
                    0); //ESTRUCTURA= A: Alien_x : Alien_y : TipoAlien : NumBunker : X_jugador  : X_disparo : Y_disparo
            Cliente();

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

            for (int i = 0; i < 4; i++) {
                Bcoords[i].x = matrixBunkers[i][0];
                Bcoords[i].y = matrixBunkers[i][1];
                Bcoords[i].health = matrixBunkers[i][2];
                HWND hsti = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP,
                                          Bcoords[i].x, Bcoords[i].y, 50, 50, hwnd, (HMENU) (i + 1), NULL, NULL);
                if (Bcoords[i].health == 100) {
                    Bcoords[i].imagen = hBitmapBunker100;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) Bcoords[i].imagen);
                } else if (Bcoords[i].health < 100 && Bcoords->health >= 50) {
                    Bcoords[i].imagen = hBitmapBunker75;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) Bcoords[i].imagen);
                } else if (Bcoords[i].health < 50 && Bcoords->health > 0) {
                    Bcoords[i].imagen = hBitmapBunker50;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) Bcoords[i].imagen);
                } else {
                }
            }

            nave = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, x_jugador, y_jugador, 50, 50, hwnd,
                                 (HMENU) 1, NULL, NULL);
            SendMessage(nave, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) hBitmap);


            // Crear temporizador que actualiza las posiciones de las imágenes cada 500ms
            SetTimer(hwnd, 1, 1000, NULL);
            break;
        }

            //Cuando se cierra la ventana
        case WM_DESTROY: {
            DeleteObject(hBitmap);
            DeleteObject(hBitmapOctopus);
            DeleteObject(hBitmapCrab);
            DeleteObject(hBitmapSquid);
            DeleteObject(hBitmapBunker100);
            DeleteObject(hBitmapBunker75);
            DeleteObject(hBitmapBunker50);
            PostQuitMessage(0);
            break;

        }
            //Movimiento de la nave


        case WM_TIMER: {
            // Actualizar posiciones de las imágenes con las nuevas posiciones de los aliens
            sprintf(mensaje, "A:%d:%d:%d:%d:%d:%d:%d", 0, 0, 0, 0, x_jugador, 0,
                    0); //ESTRUCTURA= A: Alien_x : Alien_y : TipoAlien : NumBunker : X_jugador  : X_disparo : Y_disparo
            Cliente();
            int index = 0;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 12; j++) {
                    coords[index].x = matrixAliens[i][j][0]; // add x coordinate to Coord array
                    coords[index].y = matrixAliens[i][j][1]; // add y coordinate to Coord array
                    index++;
                }
            }
            for (int i = 0; i < 60; i++) {
                HWND hsti = GetDlgItem(hwnd, i + 1);
                //Mover la imagen a la nueva posición utilizando la función MoveWindow
                MoveWindow(hsti, coords[i].x, coords[i].y, 50, 50, TRUE);
            }

            for (int i = 0; i < 4; i++) {
                Bcoords[i].x = matrixBunkers[i][0];
                Bcoords[i].y = matrixBunkers[i][1];
                Bcoords[i].health = matrixBunkers[i][2];
                HWND hsti = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP,
                                          Bcoords[i].x, Bcoords[i].y, 50, 50, hwnd, (HMENU) (i + 1), NULL, NULL);
                if (Bcoords[i].health == 100) {
                    Bcoords[i].imagen = hBitmapBunker100;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) Bcoords[i].imagen);
                } else if (Bcoords[i].health < 100 && Bcoords->health >= 50) {
                    Bcoords[i].imagen = hBitmapBunker75;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) Bcoords[i].imagen);
                } else if (Bcoords[i].health < 50) {
                    Bcoords[i].imagen = hBitmapBunker50;
                    SendMessage(hsti, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) Bcoords[i].imagen);
                }
            }

            // mover cada disparo hacia arriba
            for (int i = 0; i < num_disparos; i++) {
                disparos[i].y -= 10;
                HWND hstd = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP,
                                          disparos[i].x, disparos[i].y, 10, 30, hwnd, (HMENU) (i + 100), NULL, NULL);
                sprintf(mensaje, "A:%d:%d:%d:%d:%d:%d:%d", 0, 0, 0, 0, x_jugador, disparos[i].x,
                        disparos[i].y); //ESTRUCTURA= A: Alien_x : Alien_y : TipoAlien : NumBunker : X_jugador  : X_disparo : Y_disparo
                Cliente();
                SendMessage(hstd, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) disparos[i].imagen);
            }
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

/**
 * @brief convierte en caracteres el string recibido por el servidor
 * @param str
 */
void convertStringToVariables(char str[]) {
    char *token = strtok(str, "_");  // Separamos la cadena en la primera subcadena

    char var1[1], var2[1], var3[60], var4[800], var5[100], var6[1], var7[1], var8[1];

    if (token != NULL) {
        strncpy(var1, token, sizeof(var1));
        token = strtok(NULL, "_");

        if (token != NULL) {
            strncpy(var2, token, sizeof(var2));
            token = strtok(NULL, "_");

            if (token != NULL) {
                strncpy(var3, token, sizeof(var3));
                token = strtok(NULL, "_");

                if (token != NULL) {
                    strncpy(var4, token, sizeof(var4));
                    token = strtok(NULL, "_");

                    if (token != NULL) {
                        strncpy(var5, token, sizeof(var5));
                        token = strtok(NULL, "_");

                        if (token != NULL) {
                            strncpy(var6, token, sizeof(var6));
                            token = strtok(NULL, "_");

                            if (token != NULL) {
                                strncpy(var7, token, sizeof(var7));
                                token = strtok(NULL, "_");

                                if (token != NULL) {
                                    strncpy(var8, token, sizeof(var8));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    lives = atoi(var1);
    char vida_texto[10];
    sprintf(vida_texto, "Puntaje: %d", lives);
    SetWindowText(hpuntaje,
                  vida_texto);

    score = atoi(var2);
    char puntaje_texto[10];
    sprintf(puntaje_texto, "Puntaje: %d", score);
    SetWindowText(hpuntaje,
                  puntaje_texto);

    x_jugador = atoi(var6);
    printf("POSICION JUGADOR %d\n", x_jugador);
    SetWindowPos(nave, NULL, x_jugador, y_jugador, 0, 0, SWP_NOZORDER | SWP_NOSIZE);


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
    char *matrizStr6 = var4 + 2;
    matrizStr6[strlen(matrizStr6) - 2] = '\0';

    // Eliminar los corchetes internos y los espacios en blanco
    char *matrizStr7 = strdup(matrizStr6);
    char *p6 = matrizStr7;
    while (*p6) {
        if (*p6 == '[' || *p6 == ']' || *p6 == ' ') {
            memmove(p6, p6 + 1, strlen(p6));
        } else {
            ++p6;
        }
    }

    // Eliminar los corchetes internos y los espacios en blanco (segunda capa)
    char *matrizStr8 = strdup(matrizStr7);
    p6 = matrizStr8;
    while (*p6) {
        if (*p6 == '[' || *p6 == ']' || *p6 == ' ') {
            memmove(p6, p6 + 1, strlen(p6));
        } else {
            ++p6;
        }
    }
    // Dividir la cadena en tokens
    char *token3 = strtok(matrizStr8, ",");
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

/**
 * @brief carga previamente las imagenes necesarias por la interfaz
 */
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
    hBitmapBunker100 = LoadImageW(NULL,
                                  L"imagenes\\Bunker100.bmp",
                                  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapBunker75 = LoadImageW(NULL,
                                 L"imagenes\\Bunker75.bmp",
                                 IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBitmapBunker50 = LoadImageW(NULL,
                                 L"imagenes\\Bunker50.bmp",
                                 IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

