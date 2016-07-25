#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
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
			if((game->mapa[i][j])!=0) {
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
	pushTrash(b[0].image);
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
	SDL_SetColorKey(b->image, SDL_TRUE, SDL_MapRGB( (b->image)->format, 0xFF, 0, 0xFF));
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
	if(game->moving_ball){
		b->posx += b->stepx;
		b->posy += b->stepy;
	}
	else{
		b->posx = p->posx;
		b->posy = p->posy - PAD_HEIGHT/2 - BALL_HEIGHT/2 + BALL_CORRECT;
	}
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
		return;
	}
	//Colisão com o topo
	if(b->posy <= BALL_HEIGHT/2 + BLOCK_HEIGHT){
		b->stepy = MOD(b->stepy);
		return;
	}
	//Colisão com o fundo
	if(b->posy >= SCREEN_HEIGHT - BALL_HEIGHT/2 ){
		game->moving_ball = false;
		player->lives--; printf("lives %d\n", player->lives);
		return;
	}
	int pad_sup_esq = p->posx - PAD_WIDTH/2 + PAD_CORRECT;
	int pad_sup_dir = p->posx + PAD_WIDTH/2 - PAD_CORRECT;
	int pad_base_sup = p->posy - PAD_HEIGHT/2 - BALL_HEIGHT/2;
	double f;
	//Colisão com a parte superior do pad
	if(b->posx >= pad_sup_esq &&  b->posx <= pad_sup_dir && b->posy >= pad_base_sup && b->posy <= pad_base_sup + PAD_HEIGHT/2){
		b->stepy = -MOD(b->stepy);
		if(b->posx > p->posx){
			f = 1.0 + (b->posx - p->posx)/(PAD_WIDTH*3);
		}
		else{
			f = 1.0 + (p->posx - b->posx)/(PAD_WIDTH*3);
		}
		//Com a parte esquerda
		if(b->posx >= pad_sup_esq && b->posx < p->posx && game->moving_ball){
			b->stepx *= b->stepx<0?f:1.0/f;
			corrige(&(b->stepx));
			return;
		}
		//Com a parte direita
		if(b->posx <= pad_sup_dir && b->posx > p->posx && game->moving_ball){
			b->stepx *= b->stepx<0?1.0/f:f;
			corrige(&(b->stepx));
			return;
		}
	}
	//Colisão com os pontos de colisão do pad
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_1E_x,p->posy+PAD_COL_1_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = -MOD(b->stepx);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_2E_x,p->posy+PAD_COL_2_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = -MOD(b->stepx);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_3E_x,p->posy+PAD_COL_3_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = -MOD(b->stepx);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_4E_x,p->posy+PAD_COL_4_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = -MOD(b->stepx);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_1D_x,p->posy+PAD_COL_1_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = MOD(b->stepx);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_2D_x,p->posy+PAD_COL_2_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = MOD(b->stepx);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_3D_x,p->posy+PAD_COL_3_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = MOD(b->stepx);
		return;
	}
	if(distancia(b->posx, b->posy,p->posx+PAD_COL_4D_x,p->posy+PAD_COL_4_y) <= BALL_WIDTH/2){
		b->stepy = -MOD(b->stepy);
		b->stepx = MOD(b->stepx);
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
						player->score+=100; printf("score %d\n", player->score);
						return;
					}
					//Colisão com a parte direita do bloco
					if(b->posx <= (j+1)*BLOCK_WIDTH + BALL_WIDTH/2 +BLOCK_WIDTH/2 && b->posx >= (j+1)*BLOCK_WIDTH){
						game->mapa[i][j]--;
						b->stepx = MOD(b->stepx);
						player->score+=100; printf("score %d\n", player->score);
						return;
					}
				}
				//Colisão com parte superior do bloco
				if(b->posx > j*BLOCK_WIDTH + BLOCK_WIDTH/2 && b->posx < (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2){
					if(b->posy >= i*BLOCK_HEIGHT - BALL_HEIGHT/2 && b->posy <= i*BLOCK_HEIGHT ){
						game->mapa[i][j]--;
						b->stepy = -MOD(b->stepy);
						player->score+=100; printf("score %d\n", player->score);
						return;
					}
					//Colisão com a parte inferior do bloco
					if(b->posy <= (i+1)*BLOCK_HEIGHT + BALL_HEIGHT/2 && b->posy >= (i+1)*BLOCK_HEIGHT ){
						game->mapa[i][j]--;
						b->stepy = MOD(b->stepy);
						player->score+=100; printf("score %d\n", player->score);
						return;
					}
				}
				if(distancia(b->posx, b->posy, j*BLOCK_WIDTH + BLOCK_WIDTH/2, i*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					game->mapa[i][j]--;
					b->stepx = -MOD(b->stepx);
					b->stepy = -MOD(b->stepy);
					player->score+=100; printf("score %d\n", player->score);
					return;
				}
				if(distancia(b->posx, b->posy, (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2, i*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					game->mapa[i][j]--;
					b->stepx = MOD(b->stepx);
					b->stepy = -MOD(b->stepy);
					player->score+=100; printf("score %d\n", player->score);
					return;
				}
				if(distancia(b->posx, b->posy, j*BLOCK_WIDTH + BLOCK_WIDTH/2, (i+1)*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					game->mapa[i][j]--;
					b->stepx = -MOD(b->stepx);
					b->stepy = MOD(b->stepy);
					player->score+=100; printf("score %d\n", player->score);
					return;
				}
				if(distancia(b->posx, b->posy, (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2, (i+1)*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					game->mapa[i][j]--;
					b->stepx = MOD(b->stepx);
					b->stepy = MOD(b->stepy);
					player->score+=100; printf("score %d\n", player->score);
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
    else{
        gWindow = SDL_CreateWindow("Ultimate Neotron HD PLUS PREMIUM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
		int i;
		for(i=0;i<trash.topo;i++){
			SDL_FreeSurface(trash.image[i]);
			trash.image[i] = NULL;
		}

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

int createPad(PAD *p){
	int success = true;
	p->image = loadSurface(PAD_ADDRESS);
	if(p->image==NULL){
		success = false;
		puts("Imagem do pad não foi carregada.");
	}
	SDL_SetColorKey(p->image, SDL_TRUE, SDL_MapRGB( (p->image)->format, 0xFF, 0, 0xFF));
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
	if(SDL_BlitSurface(p->image, &srcRct, gScreenSurface, &destRct) < 0){
		printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

void aceleratePad(PAD *p){
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_RIGHT] > 0 && p->vetor.x <= Vmax){
		p->vetor.x = p->vetor.x + 1;
		}
	else if (state[SDL_SCANCODE_LEFT] > 0 && p->vetor.x >= -Vmax){
		p->vetor.x = p->vetor.x - 1;
		}
	else{
		if(p->vetor.x==0){
			return;
		}
		p->vetor.x = (p->vetor.x)>0?(p->vetor.x-0.5):(p->vetor.x+0.5);
	}
}

double distancia(int x1, int y1, int x2, int y2){
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

int menuPause(void){
	SDL_Surface* menuTela = NULL;
	SDL_Surface* fundo = NULL;
	SDL_Rect destRct;

	destRct.x = 0;
	destRct.y = 0;
	destRct.w = 800;
	destRct.h = 600;

	int quit = 0;
	int quitGAME = 0;
	SDL_Event e;
	menuTela = SDL_GetWindowSurface( gWindow );

	fundo = loadSurface(FUNDOPAUSE_ADDRESS1);
	//teste se deu certo aqui

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
		SDL_FillRect(menuTela, NULL, SDL_MapRGB(menuTela->format, 0xFF, 0xFF, 0xFF));
		SDL_BlitSurface(fundo, NULL, menuTela, &destRct);
		SDL_UpdateWindowSurface(gWindow);
		SDL_Delay(1000/FPS);
		}
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

void menuPrincipal(void){
	SDL_Surface* menuTela = NULL;
	SDL_Surface* fundo = NULL;
	SDL_Surface* botao1 = NULL;

	//Loading Surfaces
	fundo = loadSurface(TELAINICIAL_ADDRESS1);
	if(fundo==NULL){
		puts("Imagem da Tela Pr não foi carregada.");
	}
	botao1 = loadSurface(BUTTON1_ADDRESS1);
	if(botao1==NULL){
		puts("Imagem da Botao1 não foi carregada.");
	}
	SDL_SetColorKey(botao1, SDL_TRUE, SDL_MapRGB( (botao1)->format, 0xFF, 0, 0xFF));

	//Rect da Imagem Principal
	SDL_Rect dstImgPr;
	dstImgPr.x = 0;
	dstImgPr.y = 0;
	dstImgPr.w = 800;
	dstImgPr.h = 600;

	//Rect do botao1
	SDL_Rect dstButt1;
	dstButt1.x = 400 - 334/2 ;
	dstButt1.y = 350;
	dstButt1.w = 334;
	dstButt1.h = 80;

	int quit = 0;
	SDL_Event event;
	menuTela = SDL_GetWindowSurface( gWindow );
	//Guarda qual opcao de botao foi acionada
	int opcaoSelecionada = 1;

	while(!quit){
		while(SDL_PollEvent(&event) != 0 ){
				switch(event.type){
					case SDL_QUIT:
						quit = 1;
						break;
					case SDL_KEYDOWN:
						if (opcaoSelecionada == 1){
							quit = 1;
							}
						break;
				}
			}
		SDL_FillRect(menuTela, NULL, SDL_MapRGB(menuTela->format, 0xFF, 0xFF, 0xFF));
		SDL_BlitSurface(fundo, NULL, menuTela, &dstImgPr);
		SDL_BlitSurface(botao1, NULL, menuTela, &dstButt1);
		SDL_UpdateWindowSurface(gWindow);
		SDL_Delay(1000/FPS);
		}
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
	FILE *file1;
	int i, j;
	PLAYERSTATS lidos[11];
	/*
	 * Aqui abre-se o arquivo binário para leitura e ordenação, colocando o
	 * jogador atual no final do mesmo.
	 */
	file1 = fopen("ranking.bin", "r");
	if(!file1) puts("Erro ao abrir ranking");
	fread(lidos, sizeof(PLAYERSTATS), 10, file1);
	lidos[10] = *player;
	fclose(file1);
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
	file1 = fopen("ranking.bin", "w");
	if(!file1) puts("Erro ao abrir ranking");
	fwrite(lidos, sizeof(PLAYERSTATS), 10, file1);
	fclose(file1);
	return;
}

/*
 * Função usada na sortRank, compara dois elementos PLAYERSTATS.
 */
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
