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
#include "defs.h"
#include "globais.h"
#include "func.h"



int main(int argc, char* args[]){
	int quit = false;
	SDL_Event e;
	if(!init()){
  		printf("Failed to initialize!\n");
  	}
	else {
		if(!createBlock(&gBlock1, BLOCK_ADDRESS1) || !createBall(&gBall, BALL_ADRESS) || !createPad(&gPad, PAD_ADDRESS)){
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
			aceleratePad(&gPad);
			SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x00, 0x88, 0xFF));
			colisao(&gBall);
			if(!moveBall(&gBall)){
				quit = false;
				puts("Problemas ao imprimir a bola.\n");
			}
			if(!imprimeMapa(Mapa1)){
				quit = false;
				puts("Problemas ao imprimir um bloco.\n");
			}
			if(!movePad(&gPad)){
				quit = false;
				puts("Problemas ao imprimir o pad.\n");				
				}
			SDL_UpdateWindowSurface(gWindow);
			SDL_Delay(1000/FPS);
		}
	}


	closing();

	return 0;
}
