/*
 * ULTIMATE NEOTRON HD
 *
 * Autores: Henrique Vermelho, Lucca Martins, Pedro Vitor
 *
 * Descrição: Este programa escrito em C com a biblioteca gŕafica SDL simula o jogo
 *            Breakout
 *
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "defs.h"
#include "globais.h"
#include "func.h"



int main(int argc, char* args[]){
	int quit = false;
	SDL_Event e;
	BLOCK Blocks[3];
	BALL Ball;
	PAD Pad;
	GAMESTATS Game;
	PLAYERSTATS Player;
	Game.level = 0;
	Player.score = 0;
	Player.lives = 3;
	/*
	 * Esse incremento conta quantas vidas extras serão conferidas ao jogador
	 * a cada mil pontos.
	 */
	int incremento = 0;

	trash.topo = 0;
	if(!init()){
  		printf("Failed to initialize!\n");
  	}
	else {
		loadLevel(&Game);
		menuPrincipal();

		if(!createBlock(Blocks) || !createBall(&Ball) || !createPad(&Pad) || !createBackground(BACKGROUND_ADDRESS1) ){
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
								quit = menuPause();
								break;
							case SDLK_q:
								quit = true;
								break;
							case SDLK_SPACE:
								Game.moving_ball = true;
								break;
						}
						break;
				}
			}

			aceleratePad(&Pad);
			SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));

			if (!blitBackground()){
				quit = false;
				puts("Problemas ao imprimir o fundo.\n");
				}
			if(!moveBall(&Ball, &Pad, &Game)){
				quit = false;
				puts("Problemas ao imprimir a bola.\n");
			}




			colisao(&Ball, &Pad, &Game, &Player);



			if(!imprimeMapa(&Game, Blocks)){
				quit = false;
				puts("Problemas ao imprimir um bloco.\n");
			}
			if(!movePad(&Pad)){
				quit = false;
				puts("Problemas ao imprimir o pad.\n");
				}

			/*
			 * Essa série de ifs verifica as vidas extras, as vidas,
			 * e ordena as pontuações até então..
			 */
			if(Player.score%1000 != 0) {
				incremento = 1;
			}
			if(Player.score%1000 == 0 && Player.score!=0 && incremento) {
				incremento = 0;
				Player.lives++;
				printf("vidas %d\n", Player.lives);
			}
			if(Player.lives < 0){
				sortRank(&Player);
				//Aqui tem que abrir uma tela de gameover/continue.
				Player.lives = 3;
				Player.score = 0;
				menuPrincipal();
			}


			SDL_UpdateWindowSurface(gWindow);
			SDL_Delay(1000/FPS);
		}
	}

	closing();

	return 0;
}
