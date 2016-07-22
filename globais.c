#include "globais.h"

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

BLOCK gBlock1;

BALL gBall;

int Mapa1[10][9]= {{0,0,0,0,0,0,0,0,0},
                   {1,0,1,0,1,1,1,1,1},
                   {1,0,1,0,1,1,1,1,1},
                   {1,0,1,0,1,1,1,1,1},
                   {1,0,1,0,1,1,1,1,1},
                   {1,0,1,0,1,1,1,1,1},
                   {1,0,1,0,1,1,1,1,1},
                   {1,0,1,0,1,1,1,1,1},
                   {1,0,1,0,1,1,1,1,1},
                   {1,0,1,0,1,1,1,1,1}};

PAD gPad;

GAMESTATS gPlayer1;
