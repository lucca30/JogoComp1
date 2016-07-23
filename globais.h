#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include"defs.h"
#ifndef GLOBAIS_H
#define GLOBAIS_H

extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Surface* gBackground;

extern BLOCK gBlock1;

extern BALL gBall;

extern int Mapa1[10][9];

extern PAD gPad;

extern GAMESTATS gPlayer1;

#endif
