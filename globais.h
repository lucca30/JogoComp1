#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include"defs.h"
#ifndef GLOBAIS_H
#define GLOBAIS_H

extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Surface* gBackground;
extern Mix_Chunk* gLevel;
extern Mix_Chunk* gHit;
extern Mix_Chunk* gHit2;
extern Mix_Chunk* gHit3;
extern Mix_Chunk* gLevelup;
extern Mix_Chunk* gLaunch;
extern Mix_Chunk* gDeath;


extern LIXO trash;

#endif
