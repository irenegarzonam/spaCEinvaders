#ifndef CLIENTE_CLIENTE_H
#define CLIENTE_CLIENTE_H

//Parte Cliente
WSADATA wsa;
SOCKET sockfd;
struct sockaddr_in servidor;
char mensaje[1024], respuesta[1024];
int score = 0;
int lives = 2;
int matrixBunkers[4][3];
int matrixAliens[5][12][2];
void convertStringToVariables(char str[]);


// Global variables
WSADATA wsa;
SOCKET s;
struct sockaddr_in server;
char message[1000], server_reply[2000];
int recv_size;

static HWND nave;
static HWND hvida;
static HWND hpuntaje;
static int x_jugador = 546;
static int y_jugador = 605;

//Parte Interfaz
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void LoadMyImage(void);

HBITMAP hBitmapOctopus, hBitmapCrab, hBitmapSquid, hBitmap, hBitmapDisparoJugador, hBitmapDisparoAlien, hBitmapBunker100, hBitmapBunker75, hBitmapBunker50;

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
    int health;
    HBITMAP imagen;
} BunkerCoord;
BunkerCoord Bcoords[4];

typedef struct {
    int x;
    int y;
    HBITMAP imagen;
} disparo;

disparo disparos[100];
int num_disparos = 0;


#endif /* CLIENTE_CLIENTE_H */