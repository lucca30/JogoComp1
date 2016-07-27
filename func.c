#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <string.h>
#include "defs.h"
#include "globais.h"
#include "func.h"

int imprimeMapa(GAMESTATS *game, BLOCK *b){
	int i, j;
	int success = true;
	SDL_Rect srcRct, destRct;
	srcRct.x = 0;
	srcRct.y = 0;
	srcRct.w = BLOCK_WIDTH;
	srcRct.h = BLOCK_HEIGHT;
	for(i=0;i<10;i++){
		for(j=0;j<9;j++){
			if((game->mapa[i][j])!=0) { //Se o BLoco não for vazio
				destRct.x=j*BLOCK_WIDTH + BLOCK_WIDTH/2;
				destRct.y=i*BLOCK_HEIGHT;
				if(SDL_BlitSurface(b[game->mapa[i][j]-1].image, &srcRct, gScreenSurface, &destRct) < 0){
					printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

int createBlock(BLOCK *b){
	int success = true;
	b[0].image = loadSurface(BLOCK_ADDRESS1);
	if(b->image==NULL){
		success = false;
		puts("Imagem do bloco não foi carregada.");
	}
	pushTrash(b[0].image);//Guardando a imagem para futuro closing()
	SDL_SetColorKey(b[0].image, SDL_TRUE, SDL_MapRGB( (b[0].image)->format, 0xFF, 0, 0xFF));
	b[1].image = loadSurface(BLOCK_ADDRESS2);
	if(b->image==NULL){
		success = false;
		puts("Imagem do bloco não foi carregada.");
	}
	pushTrash(b[1].image);
	SDL_SetColorKey(b[1].image, SDL_TRUE, SDL_MapRGB( (b[1].image)->format, 0xFF, 0, 0xFF));
	b[2].image = loadSurface(BLOCK_ADDRESS3);
	if(b->image==NULL){
		success = false;
		puts("Imagem do bloco não foi carregada.");
	}
	pushTrash(b[2].image);
	SDL_SetColorKey(b[2].image, SDL_TRUE, SDL_MapRGB( (b[2].image)->format, 0xFF, 0, 0xFF));

	return success;
}

int createBall(BALL *b){
	int success = true;
	b->image = loadSurface(BALL_ADDRESS);
	if(b->image==NULL){
		success = false;
		puts("Imagem da bola não foi carregada.");
	}
	//Chroma key
	SDL_SetColorKey(b->image, SDL_TRUE, SDL_MapRGB( (b->image)->format, 0xFF, 0, 0xFF));

//Atrinbuições iniciais
	b->posx = 400;
	b->posy = 400;
	b->stepx = BALL_INIT_SPEED_X;
	b->stepy = BALL_INIT_SPEED_Y;
	return success;
}

int moveBall(BALL *b, PAD *p, GAMESTATS *game){
	int success = true;
	SDL_Rect srcRct, destRct;
	srcRct.x = 0;
	srcRct.y = 0;
	srcRct.w = BALL_WIDTH;
	srcRct.h = BALL_HEIGHT;
	//Bola livre do player
	if(game->moving_ball){
		b->posx += b->stepx;
		b->posy += b->stepy;
	}
	else{
		//Bola presa ao PAD
		b->posx = p->posx;
		b->posy = p->posy - PAD_HEIGHT/2 - BALL_HEIGHT/2 + BALL_CORRECT;
	}

	//Reajuste do posicionamento para que a posição da bola seja baseada no centro
	destRct.x = (int) b->posx - BALL_WIDTH/2;
	destRct.y = (int) b->posy - BALL_HEIGHT/2;
	if(SDL_BlitSurface(b->image, &srcRct, gScreenSurface, &destRct) < 0){
		printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

void colisao(BALL *b, PAD *p, GAMESTATS *game, PLAYERSTATS *player){
	int i, j;

	//Colisão com as laterais das paredes
	if(b->posx >= SCREEN_WIDTH - BALL_WIDTH/2 - BLOCK_WIDTH/2 || b->posx <= BALL_WIDTH/2 + BLOCK_WIDTH/2){
		b->stepx = -(b->stepx);
		playSound(gHit3);
		return;
	}

	//Colisão com o topo
	if(b->posy <= BALL_HEIGHT/2 + BLOCK_HEIGHT){
		b->stepy = MOD(b->stepy);
		playSound(gHit3);
		return;
	}

	//Colisão com o fundo
	if(b->posy >= SCREEN_HEIGHT - BALL_HEIGHT/2 ){
		game->moving_ball = false;//conectar a bola ao PAD
		player->lives--;
		playSound(gDeath);
		return;
	}
	//Limites laterais da base superior do pad
	int pad_sup_esq = p->posx - PAD_WIDTH/2 + PAD_CORRECT;
	int pad_sup_dir = p->posx + PAD_WIDTH/2 - PAD_CORRECT;
	//Altura da base superior do PAD
	int pad_base_sup = p->posy - PAD_HEIGHT/2 - BALL_HEIGHT/2;

	//Coeficiente para alterar a velocidade da bola
	double f;

	//Colisão com a parte superior do pad
	if(b->posx >= pad_sup_esq &&  b->posx <= pad_sup_dir && b->posy >= pad_base_sup && b->posy <= pad_base_sup + PAD_HEIGHT/2){
		b->stepy = -MOD(b->stepy);
		if(b->posx > p->posx){
			f = 1.0 + (b->posx - p->posx)/(PAD_WIDTH*2);
		}
		else{
			f = 1.0 + (p->posx - b->posx)/(PAD_WIDTH*2);
		}

		//Com a parte esquerda
		if(b->posx >= pad_sup_esq && b->posx < p->posx && game->moving_ball){
			b->stepx *= b->stepx<0?f:1.0/f;
			corrige(&(b->stepx));
			playSound(gHit2);
			return;
		}
		//Com a parte direita
		if(b->posx <= pad_sup_dir && b->posx > p->posx && game->moving_ball){
			b->stepx *= b->stepx<0?1.0/f:f;
			corrige(&(b->stepx));
			playSound(gHit2);
			return;
		}
	}

	//Colisão com os pontos de colisão do pad (são 8)
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_1E_x,p->posy+PAD_COL_1_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = -MOD(b->stepx);
		playSound(gHit2);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_2E_x,p->posy+PAD_COL_2_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = -MOD(b->stepx);
		playSound(gHit2);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_3E_x,p->posy+PAD_COL_3_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = -MOD(b->stepx);
		playSound(gHit2);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_4E_x,p->posy+PAD_COL_4_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = -MOD(b->stepx);
		playSound(gHit2);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_1D_x,p->posy+PAD_COL_1_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = MOD(b->stepx);
		playSound(gHit2);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_2D_x,p->posy+PAD_COL_2_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = MOD(b->stepx);
		playSound(gHit2);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_3D_x,p->posy+PAD_COL_3_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = MOD(b->stepx);
		playSound(gHit2);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_4D_x,p->posy+PAD_COL_4_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = MOD(b->stepx);
		playSound(gHit2);
		return;
	}

	//Colisão de Blocos
	for(i=9;i>=0;i--){
		for(j=8;j>=0;j--){
			if(game->mapa[i][j]!=0){
				if(b->posy > (i)*BLOCK_HEIGHT && b->posy < (i+1)*BLOCK_HEIGHT){
					//Colisão com a parte esquerda do bloco
					if(b->posx >= j*BLOCK_WIDTH - BALL_WIDTH/2 + BLOCK_WIDTH/2 && b->posx <= j*BLOCK_WIDTH + BLOCK_WIDTH){
						game->mapa[i][j]--;;
						b->stepx = -MOD(b->stepx);
						player->score+=100;
						playSound(gHit);
						return;
					}
					//Colisão com a parte direita do bloco
					if(b->posx <= (j+1)*BLOCK_WIDTH + BALL_WIDTH/2 +BLOCK_WIDTH/2 && b->posx >= (j+1)*BLOCK_WIDTH){
						game->mapa[i][j]--;
						b->stepx = MOD(b->stepx);
						player->score+=100;
						playSound(gHit);
						return;
					}
				}
				//Colisão com parte superior do bloco
				if(b->posx > j*BLOCK_WIDTH + BLOCK_WIDTH/2 && b->posx < (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2){
					if(b->posy >= i*BLOCK_HEIGHT - BALL_HEIGHT/2 && b->posy <= i*BLOCK_HEIGHT ){
						game->mapa[i][j]--;
						b->stepy = -MOD(b->stepy);
						player->score+=100;
						playSound(gHit);
						return;
					}
					//Colisão com a parte inferior do bloco
					if(b->posy <= (i+1)*BLOCK_HEIGHT + BALL_HEIGHT/2 && b->posy >= (i+1)*BLOCK_HEIGHT ){
						game->mapa[i][j]--;
						b->stepy = MOD(b->stepy);
						player->score+=100;
						playSound(gHit);
						return;
					}
				}
				//Colisão com as quinas dos Blocos (são 4)

				//Superior esquerda
				if(distancia(b->posx, b->posy, j*BLOCK_WIDTH + BLOCK_WIDTH/2, i*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					game->mapa[i][j]--;
					b->stepx = -MOD(b->stepx);
					b->stepy = -MOD(b->stepy);
					player->score+=100;
					playSound(gHit);
					return;
				}
				//Superior direita
				if(distancia(b->posx, b->posy, (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2, i*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					game->mapa[i][j]--;
					b->stepx = MOD(b->stepx);
					b->stepy = -MOD(b->stepy);
					player->score+=100;
					playSound(gHit);
					return;
				}
				//Inferior esquerda
				if(distancia(b->posx, b->posy, j*BLOCK_WIDTH + BLOCK_WIDTH/2, (i+1)*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					game->mapa[i][j]--;
					b->stepx = -MOD(b->stepx);
					b->stepy = MOD(b->stepy);
					player->score+=100;
					playSound(gHit);
					return;
				}
				//Inferior direita
				if(distancia(b->posx, b->posy, (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2, (i+1)*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					game->mapa[i][j]--;
					b->stepx = MOD(b->stepx);
					b->stepy = MOD(b->stepy);
					player->score+=100;
					playSound(gHit);
					return;
				}
			}
		}
	}
}

int init(){
    int success = true;

    if( SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    if (TTF_Init() == -1){
		printf("SDL could not initialize TTF! SDL Error: %s\n", SDL_GetError());
        success = false;
	}

	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
		success = false;
	}

	else{
        gWindow = SDL_CreateWindow("CastleBreak", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else{
			//Carregamento do áudio
			gLevel = Mix_LoadWAV(SE1_ADDRESS);
			gHit = Mix_LoadWAV(SE2_ADDRESS);
			gHit2 = Mix_LoadWAV(SE3_ADDRESS);
			gHit3 = Mix_LoadWAV(SE4_ADDRESS);
			gLevelup = Mix_LoadWAV(SE5_ADDRESS);
			gLaunch = Mix_LoadWAV(SE6_ADDRESS);
			gDeath = Mix_LoadWAV(SE7_ADDRESS);
			if(!gLevel || !gHit || !gHit2 || !gHit3 || !gLevelup || !gLaunch || !gDeath) {
				success = false;
			}

			//Carregamento de imagens
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
	int i;

	//Libera o espaço dos elementos globais
  SDL_FreeSurface(gScreenSurface);
  gScreenSurface = NULL;

	//Limpeza da lixeira
	for(i=0;i<trash.topo;i++){
		SDL_FreeSurface(trash.image[i]);
		trash.image[i] = NULL;
	}

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

	//Fecha o SDL
  IMG_Quit();
  SDL_Quit();
  TTF_Quit();
	Mix_FreeChunk(gLevel);
	Mix_CloseAudio();

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

int createPad(PAD *p){
	int success = true;
	p->image = loadSurface(PAD_ADDRESS);
	if(p->image==NULL){
		success = false;
		puts("Imagem do pad não foi carregada.");
	}
	//Chroma Key
	SDL_SetColorKey(p->image, SDL_TRUE, SDL_MapRGB( (p->image)->format, 0xFF, 0, 0xFF));

	//Atribuições iniciais
	p->posx = 400;
	p->posy = 540;
	p->vetor.x = 0;
	p->vetor.y = 0;
	return success;
}

int movePad(PAD *p){
	int success = true;
	SDL_Rect srcRct, destRct;
	srcRct.x = 0;
	srcRct.y = 0;
	srcRct.w = PAD_WIDTH;
	srcRct.h = PAD_HEIGHT;

	//Movimentação baseada na velocidade
	p->posx += p->vetor.x;
	p->posy += p->vetor.y;
	if(p->posx>=SCREEN_WIDTH - PAD_WIDTH/2 - BLOCK_WIDTH/2){
		p->posx = SCREEN_WIDTH - PAD_WIDTH/2 - BLOCK_WIDTH/2;
	}
	if(p->posx<=PAD_WIDTH/2 + BLOCK_WIDTH/2){
		p->posx = PAD_WIDTH/2 + BLOCK_WIDTH/2;
	}
	destRct.x = p->posx - PAD_WIDTH/2;
	destRct.y = p->posy - PAD_HEIGHT/2;

	//impressão do movimento
	if(SDL_BlitSurface(p->image, &srcRct, gScreenSurface, &destRct) < 0){
		printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

void acceleratePad(PAD *p){
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//Acelerar para direita
	if (state[SDL_SCANCODE_RIGHT] > 0 && p->vetor.x <= Vmax){
		p->vetor.x = p->vetor.x + 1;
		}
	else if (state[SDL_SCANCODE_LEFT] > 0 && p->vetor.x >= -Vmax){
		//Acelerar para a esquerda
		p->vetor.x = p->vetor.x - 1;
		}
	else{
		if(p->vetor.x==0){
			return;
		}
		//Caso não haja nenhuma aceleração, verificar se tem alguma velocidade restante
		//se sim, diminuir até freiar
		p->vetor.x = (p->vetor.x)>0?(p->vetor.x-0.5):(p->vetor.x+0.5);
	}
}

double distancia(int x1, int y1, int x2, int y2){
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

int menuPause(void){
	SDL_Surface* fundo = NULL;
	SDL_Rect destRct = {0,0,800,600};

	int quit = 0;
	int quitGAME = 0;
	SDL_Event e;

	fundo = loadSurface(FUNDOPAUSE_ADDRESS1);

	while(!quit){
		while(SDL_PollEvent(&e) != 0 ){
				switch(e.type){
					case SDL_QUIT:
						quitGAME = 1;
						quit = 1;
						break;
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym){
							case SDLK_ESCAPE:
								quit = 1;
								break;
							}
						break;
				}
			}

		SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
		SDL_BlitSurface(fundo, NULL, gScreenSurface, &destRct);
		SDL_UpdateWindowSurface(gWindow);
		}
	SDL_FreeSurface(fundo);
	return quitGAME;
	}

int createBackground(char *address){
	int success = true;

	gBackground = loadSurface(address);
	if(gBackground==NULL){
		puts("Imagem do background não foi carregada.");
		success = false;
	}
	return success;
	}

int blitBackground(void){
	int success = true;
	SDL_Rect dstRectBackground;

	dstRectBackground.x = 0;
	dstRectBackground.y = 0;
	dstRectBackground.w = 800;
	dstRectBackground.h = 600;


	if(SDL_BlitSurface(gBackground, NULL, gScreenSurface, &dstRectBackground) < 0){
		printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
	}

int menuPrincipal(GAMESTATS *game, PLAYERSTATS *player){
	SDL_Surface* fundo = NULL;
	SDL_Surface* botao1 = NULL;
	SDL_Surface* botao2 = NULL;
	SDL_Surface* ponteiro = NULL;

	loadLevel(game);
	//Loading Surfaces
	fundo = loadSurface(TELAINICIAL_ADDRESS1);
	if(fundo==NULL){puts("Imagem da Tela Pr não foi carregada.");}
	botao1 = loadSurface(BUTTON1_ADDRESS1);

	if(botao1==NULL){puts("Imagem da Botao1 não foi carregada.");}
	SDL_SetColorKey(botao1, SDL_TRUE, SDL_MapRGB( (botao1)->format, 0xFF, 0, 0xFF));

	botao2 = loadSurface(BUTTON2_ADDRESS1);
	if(botao2==NULL){puts("Imagem da Botao2 não foi carregada.");}
	SDL_SetColorKey(botao2, SDL_TRUE, SDL_MapRGB( (botao2)->format, 0xFF, 0, 0xFF));

	ponteiro = loadSurface(PONTEIRO_ADDRESS1);
	if(ponteiro==NULL){puts("Imagem da Ponteiro não foi carregada.");}
	SDL_SetColorKey(ponteiro, SDL_TRUE, SDL_MapRGB( (ponteiro)->format, 0xFF, 0, 0xFF));

	//Rect da Imagem Principal
	SDL_Rect dstImgPr = {0,0,800,600};
	//Rect do botao1
	SDL_Rect dstButt1 = {400 - 334/2,310,334,80};
	//Rect do botao2
	SDL_Rect dstButt2 = {400 - 334/2,410,334,80};
	//Rect do botao2
	SDL_Rect dstPont = {400 - 334/2 - 50,400,334,80};

	int quit = 0;
	char name[6]={""}, string[6];
	SDL_Event event;
	int opcaoSelecionada = 1;//Guarda qual opcao de botao foi acionada
	int alterado = true;
	while(!quit){
		//Se apertar para baixo na ultima opcao volta para a de cima
		if (opcaoSelecionada > 2){opcaoSelecionada = 1;}
		if (opcaoSelecionada < 1){opcaoSelecionada = 2;}

		dstPont.y = 220 + (opcaoSelecionada*100);

		while(SDL_PollEvent(&event) != 0 ){
				switch(event.type){
					case SDL_QUIT:
						quit = 1;
						return 1;
						break;
					case SDL_TEXTINPUT:
					//Manipulação do nome inserido pelo usuário
						if(strlen(name)<5){
							strcat(name, event.text.text);
							alterado = true;
						}
						break;
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym){
							case SDLK_DOWN:
								opcaoSelecionada++;
								alterado = true;
								break;
							case SDLK_UP:
								opcaoSelecionada--;
								alterado = true;
								break;
							case SDLK_BACKSPACE:
								//apagar o último caracter
								name[strlen(name)-1] = '\0';
								alterado = true;
								break;
							case SDLK_SPACE:
								if (opcaoSelecionada == 1){
									alterado = true;
									strcpy(player->playerName, name);
									quit = 1;
								}
								if (opcaoSelecionada == 2){
									quit = telaRanking();
									if(quit){
										return quit;
									}
									alterado = true;
								}
						}
				}
			}
			//Só atualiza a tela em caso so usuário interagir
			if(alterado){
				SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
				SDL_BlitSurface(fundo, NULL, gScreenSurface, &dstImgPr);
				SDL_BlitSurface(botao1, NULL, gScreenSurface, &dstButt1);
				SDL_BlitSurface(botao2, NULL, gScreenSurface, &dstButt2);
				SDL_BlitSurface(ponteiro, NULL, gScreenSurface, &dstPont);
				blitaNome(name, string);
				SDL_UpdateWindowSurface(gWindow);
				if(alterado==2){
					alterado = false;
				}
				else{
					alterado++;
				}
			}
			SDL_Delay(1000/FPS);
		}
		SDL_FreeSurface(fundo);
		SDL_FreeSurface(botao1);
		SDL_FreeSurface(botao2);
		SDL_FreeSurface(ponteiro);

	return 0;
	}

void corrige(double *stepx){
	double corrigido;
	double modulo = *stepx;
	if(*stepx < 0.0){
		modulo = -(*stepx);
	}
	if(modulo >BALL_MAX_SPEED_X){
		corrigido = BALL_MAX_SPEED_X;
	}
	else if(modulo < BALL_MIN_SPEED_X){
		corrigido = BALL_MIN_SPEED_X;
	}
	else{
		corrigido = modulo;
	}
	if(*stepx>0){
		*stepx = corrigido;
	}
	else{
		*stepx = -corrigido;
	}
}

void pushTrash(SDL_Surface *surf){
	trash.image[trash.topo++] = surf;
}

void sortRank(PLAYERSTATS *player){
	FILE *pArq;
	int i, j;
	PLAYERSTATS lidos[11];
	/*
	 * Aqui abre-se o arquivo binário para leitura e ordenação, colocando o
	 * jogador atual no final do mesmo.
	 */
	pArq = fopen(RANK_ADDRESS, "r");
	if(!pArq) puts("Erro ao abrir ranking");
	fread(lidos, sizeof(PLAYERSTATS), 10, pArq);
	lidos[10] = *player;
	fclose(pArq);
	/*
	 * Aqui realiza-se o ordenamento desse vetor.
	 */
	for(i = 0; i < 11-1; i++){
        for(j = 0; j < 11-(1+i); j++){
            if(compare(&(lidos[j]), &(lidos[j+1])) < 0 ){
                PLAYERSTATS temp = lidos[j];
                lidos[j] = lidos[j+1];
                lidos[j+1] = temp;
            }
        }
    }
	/*
	 * Aqui os arquivos são retornados ao arquivo binário.
	 */
	pArq = fopen(RANK_ADDRESS, "w");
	if(!pArq) puts("Erro ao abrir ranking");
	fwrite(lidos, sizeof(PLAYERSTATS), 10, pArq);
	fclose(pArq);
	return;
}

int compare(PLAYERSTATS *a, PLAYERSTATS *b){
	if(a->score < b->score) return -1;
	else if(a->score > b->score) return 0;
	else return 0;
}

void loadLevel(GAMESTATS *game){
	FILE *pArq;
	int fase = game->level;
	pArq = fopen(MAP_ADDRESS,"r");
	fseek(pArq, fase*sizeof(GAMESTATS), SEEK_SET);
	fread(game, sizeof(GAMESTATS), 1, pArq);
	fclose(pArq);
	game->level = fase;
}

int updatePlayer(PLAYERSTATS *Player, GAMESTATS *game, PAD *p){
	int quitGAME = 0;
	if(Player->score%10000 != 0) {
		Player->incremento = 1;
	}
	if(Player->score%10000 == 0 && Player->score!=0 && Player->incremento) {
		Player->incremento = 0;
		Player->lives++;
		playSound(gLevelup);
	}
	if(Player->lives < 0){
		sortRank(Player);
		gameoverTela();
		Player->lives = 3;
		Player->score = 0;
		quitGAME = menuPrincipal(game,Player);
		game->level = 0;
		p->posx = SCREEN_WIDTH/2;
		loadLevel(game);
	}
	if(Player->score == game->total_level_score){
		game->level++;
		playSound(gLevel);
		telaLevel(game);
		loadLevel(game);
	}
	return quitGAME;
}

TTF_Font* preparaFonte(char* arquivoFonte, int size){
	TTF_Font* fonte;
	fonte = TTF_OpenFont(arquivoFonte, size);
	return fonte;
	}

SDL_Surface* createSurfaceTTF(char* texto,TTF_Font* fonte,int colorR,int colorG,int colorB){
	SDL_Color cor = {colorR, colorG, colorB};
	SDL_Surface* superficieTexto = TTF_RenderText_Blended(fonte, texto, cor);
	return superficieTexto;
}

void printPlayerStats(PLAYERSTATS player, TTF_Font* fonteScore, GAMESTATS game){
	char stringTemp[1000];

	SDL_Surface* scoreSuperficie;
	SDL_Rect rectTemp;
	rectTemp.x = 50;
	rectTemp.y = 0;
	rectTemp.w = 0;
	rectTemp.h = 0;

	sprintf(stringTemp,"Score: %5d",player.score);
	scoreSuperficie = createSurfaceTTF(stringTemp,fonteScore,255,255,255);
	SDL_BlitSurface(scoreSuperficie, NULL, gScreenSurface, &rectTemp);

	SDL_FreeSurface(scoreSuperficie);

	rectTemp.x = 325;
	sprintf(stringTemp,"Vidas: %2d", player.lives);
	scoreSuperficie = createSurfaceTTF(stringTemp,fonteScore,255,255,255);
	SDL_BlitSurface(scoreSuperficie, NULL, gScreenSurface, &rectTemp);

	SDL_FreeSurface(scoreSuperficie);

	rectTemp.x = 620;
	sprintf(stringTemp,"Level:%d", game.level+1);
	scoreSuperficie = createSurfaceTTF(stringTemp,fonteScore,255,255,255);
	SDL_BlitSurface(scoreSuperficie, NULL, gScreenSurface, &rectTemp);

	SDL_FreeSurface(scoreSuperficie);
}

void gameoverTela(void){
	SDL_Surface* fundo = NULL;

	//Loading Surfaces
	fundo = loadSurface(GAMEOVER_ADDRESS1);
	if(fundo==NULL){
		puts("Imagem da Tela Gameover não foi carregada.");
	}

	//Rect da Imagem
	SDL_Rect dstImg = {0, 0, 800, 600};
	//Guarda qual opcao de botao foi acionada

	SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_BlitSurface(fundo, NULL, gScreenSurface, &dstImg);
	SDL_UpdateWindowSurface(gWindow);
	SDL_FreeSurface(fundo);
	SDL_Delay(3000);

}

void printRanking(void){
	char stringTemp[50];
	FILE *ranking;
	PLAYERSTATS lidos[10];
	int i;
	SDL_Surface* scoreSuperficie;
	SDL_Rect destRct;
	TTF_Font* fonteScore = preparaFonte(FONT_ADDRESS, 24);
	TTF_Font* title = preparaFonte(FONT_ADDRESS, 70);

	ranking = fopen(RANK_ADDRESS, "r");
	if(!ranking) return;

	fread(lidos, sizeof(PLAYERSTATS), 10, ranking);

	for(i = 0; i < 10; i++) {
		sprintf(stringTemp,"Player: %s", lidos[i].playerName);
		scoreSuperficie = createSurfaceTTF(stringTemp,fonteScore,255,255,255);
		destRct.x = 175;
		destRct.y = 50*i + 110;
		SDL_BlitSurface(scoreSuperficie, NULL, gScreenSurface, &destRct);
		SDL_FreeSurface(scoreSuperficie);

		sprintf(stringTemp,"Score: %d", lidos[i].score);
		scoreSuperficie = createSurfaceTTF(stringTemp,fonteScore,255,255,255);
		destRct.x = 475;
		SDL_BlitSurface(scoreSuperficie, NULL, gScreenSurface, &destRct);
		SDL_FreeSurface(scoreSuperficie);

	}
	destRct.x = 250;
	destRct.y = 10;
	sprintf(stringTemp, "RANKING");
	scoreSuperficie = createSurfaceTTF(stringTemp, title, 255,255,255);
	SDL_BlitSurface(scoreSuperficie, NULL, gScreenSurface, &destRct);
	SDL_FreeSurface(scoreSuperficie);
}

int telaRanking(void){
	SDL_Surface* fundo = NULL;
	SDL_Rect destRct;

	destRct.x = 0;
	destRct.y = 0;
	destRct.w = 800;
	destRct.h = 600;

	int quit = 0;
	int quitGAME = 0;
	SDL_Event e;
	fundo = loadSurface("./img/teste.png");
	SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_BlitSurface(fundo, NULL, gScreenSurface, &destRct);
	printRanking();
	SDL_UpdateWindowSurface(gWindow);
	while(!quit){
		while(SDL_PollEvent(&e) != 0 ){
				switch(e.type){
					case SDL_QUIT:
						quitGAME = 1;
						quit = 1;
						break;
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym){
							case SDLK_ESCAPE:
								quit = 1;
								break;
							}
						break;
				}
			}
		SDL_Delay(1000/FPS);
		}
	SDL_FreeSurface(fundo);
	return quitGAME;
}

void telaLevel(GAMESTATS *game){
	SDL_Surface *fundo, *message;
	SDL_Rect destRct;
	char stringTemp[50];
	TTF_Font* font = preparaFonte(FONT_ADDRESS, 100);

	destRct.x = 220;
	destRct.y = 240;

	fundo = loadSurface(LEVEL_ADDRESS);
	SDL_BlitSurface(fundo, NULL, gScreenSurface, NULL);

	sprintf(stringTemp,"Level: %d", game->level+1);
	message = createSurfaceTTF(stringTemp,font,255,255,255);
	SDL_BlitSurface(message, NULL, gScreenSurface, &destRct);
	SDL_UpdateWindowSurface(gWindow);

	SDL_FreeSurface(fundo);
	SDL_FreeSurface(message);

	SDL_Delay(1000);
	}


void blitaNome(char nome[6], char stringTemp[6]){
	int i;
	SDL_Surface *message;
	SDL_Rect destRct;
	destRct.y = 230;
	TTF_Font* font = preparaFonte(FONT_ADDRESS, 50);
	for(i=0;i<6;i++){
		if(nome[i]=='\0'){break;}
		destRct.x = 350 + 45*i;
		sprintf(stringTemp, "%c", nome[i]);
		message = createSurfaceTTF(stringTemp,font,0,0,0);
		SDL_BlitSurface(message, NULL, gScreenSurface, &destRct);
		SDL_FreeSurface(message);
	}
}


void logoTela(void){
	SDL_Surface* fundo = NULL;

	//Loading Surfaces
	fundo = loadSurface(LOGO_ADDRESS1);
	if(fundo==NULL){
		puts("Imagem da Tela Logo não foi carregada.");
		}

	//Rect da Imagem
	SDL_Rect dstImg = {0, 0, 800, 600};

	SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_BlitSurface(fundo, NULL, gScreenSurface, &dstImg);
	SDL_UpdateWindowSurface(gWindow);

	SDL_FreeSurface(fundo);

	SDL_Delay(3000);
	}

int playSound(Mix_Chunk *effect){
	if(Mix_PlayChannel(-1, effect, 0) == -1){
		return 1;
	}

	return 0;
	}
