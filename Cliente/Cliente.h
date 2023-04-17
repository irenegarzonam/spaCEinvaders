#ifndef CLIENTE_CLIENTE_H
#define CLIENTE_CLIENTE_H

//Parte Cliente
WSADATA wsa;
SOCKET sockfd;
struct sockaddr_in servidor;
char mensaje[1024], respuesta[1024];
int bytes_enviados, bytes_recibidos;
int connectToServer();
int receiveFromServer();
int sendToServer(char* message);
int closeConnection();
int score;
int lives;
int matrixBunkers[4][3];
int matrixAliens[5][5][2];

//Parte Interfaz
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LoadMyImage(void);
void convertStringToVariables(char* str, int* var1, int* var2, int matrix1[][3], int matrix2[][5][2]);

//La idea es que esta matriz esté vacía
int matrix[60][2] =
        {
        {75,55},{75,135},{75,215},{75,295},{75,375},
        {155,55},{155,135},{155,215},{155,295},{155,375},
        {235,55},{235,135},{235,215},{235,295},{235,375},
        {315,55},{315,135},{315,215},{315,295},{315,375},
        {395,55},{395,135},{395,215},{395,295},{395,375},
        {475,55},{475,135},{475,215},{475,295},{475,375},
        {555,55},{555,135},{555,215},{555,295},{555,375},
        {635,55},{635,135},{635,215},{635,295},{635,375},
        {715,55},{715,135},{715,215},{715,295},{715,375},
        {795,55},{795,135},{795,215},{795,295},{795,375},
        {875,55},{875,135},{875,215},{875,295},{875,375},
        {955,55},{955,135},{955,215},{955,295},{955,375},
};
HBITMAP hBitmapOctopus, hBitmapCrab, hBitmapSquid, hBitmap;

int vidas = 2;
int puntaje = 0;

#endif //CLIENTE_CLIENTE_H