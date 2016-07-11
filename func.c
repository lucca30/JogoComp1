#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "defs.h"
#include "globais.h"
#include "func.h"

int imprimeMapa(int mapa[10][10]){
	int i, j;
	int success = true;
	SDL_Rect srcRct, destRct;
	srcRct.x = 0;
	srcRct.y = 0;
	srcRct.w = BLOCK_WIDTH;
	srcRct.h = BLOCK_HEIGHT;
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			switch (mapa[i][j]) {
				case 1:
					destRct.x=j*BLOCK_WIDTH;
					destRct.y=i*BLOCK_HEIGHT;
					if(SDL_BlitSurface(gBlock1.image, &srcRct, gScreenSurface, &destRct) < 0){
						printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
						success = false;
					}
					break;
			}
		}
	}
	return success;
}

int createBlock(BLOCK *b, char *address){
	int success = true;
	b->image = loadSurface(address);
	if(b->image==NULL){
		success = false;
		puts("Imagem do bloco não foi carregada.");
	}
	return success;
}

int createBall(BALL *b, char *address){
	int success = true;
	b->image = loadSurface(address);
	if(b->image==NULL){
		success = false;
		puts("Imagem da bola não foi carregada.");
	}
	SDL_SetColorKey(b->image, SDL_TRUE, SDL_MapRGB( (b->image)->format, 0, 0, 0));
	b->posx = 400;
	b->posy = 400;
	b->stepx = -10;
	b->stepy = -5;
	return success;
}

int moveBall(BALL *b){
	int success = true;
	SDL_Rect srcRct, destRct;
	srcRct.x = 0;
	srcRct.y = 0;
	srcRct.w = BALL_WIDTH;
	srcRct.h = BALL_HEIGHT;

	b->posx += b->stepx;
	b->posy += b->stepy;
	destRct.x = b->posx - BALL_WIDTH/2;
	destRct.y = b->posy - BALL_HEIGHT/2;
	if(SDL_BlitSurface(b->image, &srcRct, gScreenSurface, &destRct) < 0){
		printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

void colisao(BALL *b){
	if(b->posx >= SCREEN_WIDTH - BALL_WIDTH/2 || b->posx <= BALL_WIDTH/2){
		b->stepx = -(b->stepx);
	}
	if(b->posy >= SCREEN_HEIGHT - BALL_HEIGHT/2 || b->posy <= BALL_HEIGHT/2){
		b->stepy = -(b->stepy);
	}
}

int init()
{
    int success = true;

    if( SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else{
        gWindow = SDL_CreateWindow("Teste 2.2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else{
						//Habilita o carregamento de imagens JPG
						//Colocar "IMG_INIT_PNG" caso queira trabalhar com imagens PNG
            int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags))
            {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }
            else{
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }
    return success;
}

void closing(){
		//Libera o espaço dos elementos globais
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;

		SDL_FreeSurface(gBlock1.image);
		gBlock1.image = NULL;

		SDL_FreeSurface(gBall.image);
		gBall.image = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

		//Fecha o SDL
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface(char *address){

    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load(address);
    if(loadedSurface == NULL){
        printf("Unable to load image %s! SDL_image Error: %s\n", address, IMG_GetError());
    }
    else{
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if( optimizedSurface == NULL ){
            printf("Unable to optimize image %s! SDL Error: %s\n", address, SDL_GetError());
        }
				//Libera o espaço utilizado para carregar a imagem que foi otimizada
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}
