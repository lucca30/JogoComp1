/*
 * Teste 2.2 de Computação 1
 *
 * Autores: Henrique Vermelho, Lucca Martins, Pedro Vitor
 *
 * Descrição: Este programa gera uma janela na qual contém duas imagens, ao clicarmos em uma delas,
 *            trocamos suas posições (uma pela outra). A janela deve fechar ao apertarmos Q, ESC ou
 *            quando apertarmos o botão fechar da janela.
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//Constantes

#define true 1
#define false 0

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 30
//Matriz do Mapa possui 10 linhas e 10 colunas

typedef struct _BLOCK{
	int imgW, imgH;
	SDL_Surface* image;
} BLOCK;

//Variáveis Globais

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

BLOCK Brick1;


//Bibliotecas extras
#include "func.h"

//Protótipos em desenvolvimento

void imprimeMapa(int mapa[10][10]);

int main(int argc, char* args[]){
	int quit = false;
	SDL_Event e;
	if(!init()){
  		printf("Failed to initialize!\n");
  	}
	else {
		while(!quit){
			while(SDL_PollEvent(&e) != 0 ){
				switch(e.type){
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym){
							case SDLK_ESCAPE:
							case SDLK_q:
					  		quit = true;
					  		break;
						}
						break;
				}
			}
			SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x00, 0x88, 0xFF));

			SDL_UpdateWindowSurface(gWindow);
		}
	}


	closing();

	return 0;
}


//Funções em desenvolvimento
