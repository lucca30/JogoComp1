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
#define BLOCK_ADDRESS1 "./img/block1.jpg"
//Matriz do Mapa possui 10 linhas e 10 colunas

typedef struct _BLOCK{
	SDL_Surface* image;
} BLOCK;

//Variáveis Globais

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

BLOCK gBlock1;


//Bibliotecas extras
#include "func.h"

//Protótipos em desenvolvimento
int imprimeMapa(int mapa[10][10]);

int createBlock(BLOCK *b, char *address);

int main(int argc, char* args[]){
	int quit = false;
	SDL_Event e;
	if(!init()){
  		printf("Failed to initialize!\n");
  	}
	else {
		if(!createBlock(&gBlock1, BLOCK_ADDRESS1)){
			quit=false;
		}
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
			if(!imprimeMapa(Mapa1)){
				quit = false;
				puts("Problemas ao imprimir um bloco");
			}
			SDL_UpdateWindowSurface(gWindow);
		}
	}


	closing();

	return 0;
}


//Funções em desenvolvimento
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
