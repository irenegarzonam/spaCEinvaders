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
int matrixAliens[5][12][2];

static HWND nave;
static HWND hvida;
static HWND hpuntaje;
static int x_jugador = 546;
static int y_jugador = 605;

//Parte Interfaz
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LoadMyImage(void);
void convertStringToVariables(char str[]);

HBITMAP hBitmapOctopus, hBitmapCrab, hBitmapSquid, hBitmap, hBitmapDisparoJugador, hBitmapDisparoAlien;

int vidas = 2;
int puntaje = 0;
// Declarar variables globales para el handle e hilo

typedef struct {
    int x;
    int y;
    HBITMAP imagen;
} Coord;
Coord coords[60];

typedef struct {
    int x;
    int y;
    HBITMAP imagen;
} disparo;

disparo disparos[100];
int num_disparos = 0;


#endif /* CLIENTE_CLIENTE_H */