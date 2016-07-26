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
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <time.h>
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
	Player.incremento = 0;
	TTF_Font *fonteScore;
	clock_t tempo_i, tempo_f;
	double tempo_gasto;
	/*
	 * Esse incremento conta quantas vidas extras serão conferidas ao jogador
	 * a cada mil pontos.
	 */

	trash.topo = 0;
	if(!init()){
  		printf("Failed to initialize!\n");
  	}
	else {

		menuPrincipal(&Game);

		if(!createBlock(Blocks) || !createBall(&Ball) || !createPad(&Pad) || !createBackground(BACKGROUND_ADDRESS1) ){
			quit=false;
		}
		//Sessão Temporária do TTF-niichan
		fonteScore = preparaFonte("fonteScore.ttf", 35);



		while(!quit){
			tempo_i = clock();
			while(SDL_PollEvent(&e) != 0 ){
				switch(e.type){
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym){
							case SDLK_ESCAPE:
								quit = menuPause();
								tempo_i=clock();
								break;
							case SDLK_q:
								quit = true;
								break;
							case SDLK_SPACE:
								Game.moving_ball = true;
								break;
							case SDLK_r:
								telaRanking();
								tempo_i = clock();
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
			printPlayerStats(Player,fonteScore);
			tempo_f = clock();
			updatePlayer(&Player, &Game, &Pad);
			SDL_UpdateWindowSurface(gWindow);
			tempo_gasto = (double)(tempo_f-tempo_i)/CLOCKS_PER_SEC;
			SDL_Delay((int)(1000/FPS - 1000*tempo_gasto)) ;
		}
	}

	closing();

	return 0;
}
