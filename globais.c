#include "globais.h"

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gBackground = NULL;


int Mapa1[10][9]= {{0,0,0,0,0,0,0,0,0},
                   {2,1,2,1,1,1,1,1,1},
                   {2,1,2,1,2,2,2,2,2},
                   {2,1,2,1,1,1,1,1,1},
                   {2,1,2,1,2,2,2,2,2},
                   {2,1,2,0,0,0,0,0,1},
                   {2,1,2,0,2,2,2,0,2},
                   {2,1,2,0,1,1,1,0,1},
                   {2,1,2,0,2,2,2,0,2},
                   {2,1,2,0,1,1,1,0,1}};

LIXO trash;
