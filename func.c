#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "defs.h"
#include "globais.h"
#include "func.h"

int imprimeMapa(int mapa[10][9]){
	int i, j;
	int success = true;
	SDL_Rect srcRct, destRct;
	srcRct.x = 0;
	srcRct.y = 0;
	srcRct.w = BLOCK_WIDTH;
	srcRct.h = BLOCK_HEIGHT;
	for(i=0;i<10;i++){
		for(j=0;j<9;j++){
			switch (mapa[i][j]) {
				case 1:
					destRct.x=j*BLOCK_WIDTH + BLOCK_WIDTH/2;
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
	SDL_SetColorKey(b->image, SDL_TRUE, SDL_MapRGB( (b->image)->format, 0xFF, 0, 0xFF));
	b->posx = 400;
	b->posy = 400;
	b->stepx = -5;
	b->stepy = -3;
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

void colisao(BALL *b, int mapa[10][9], PAD *p){
	int i, j;
	//Colisão com as laterais das paredes
	if(b->posx >= SCREEN_WIDTH - BALL_WIDTH/2 - BLOCK_WIDTH/2 || b->posx <= BALL_WIDTH/2 + BLOCK_WIDTH/2){
		b->stepx = -(b->stepx);
		return;
	}
	//Colisão com o topo
	if(b->posy >= SCREEN_HEIGHT - BALL_HEIGHT/2 || b->posy <= BALL_HEIGHT/2 + BLOCK_HEIGHT){
		b->stepy = -(b->stepy);
		return;
	}
	int pad_sup_esq = p->posx - PAD_WIDTH/2 + PAD_CORRECT;
	int pad_sup_dir = p->posx + PAD_WIDTH/2 - PAD_CORRECT;
	int pad_base_sup = p->posy - PAD_HEIGHT/2 - BALL_HEIGHT/2;
	//Colisão com a parte superior do pad
	if(b->posx >= pad_sup_esq &&  b->posx <= pad_sup_dir && b->posy >= pad_base_sup && b->posy <= pad_base_sup + PAD_HEIGHT/2){
		b->stepy = -MOD(b->stepy);
		return;
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
			if(mapa[i][j]!=0){
				if(b->posy > (i)*BLOCK_HEIGHT && b->posy < (i+1)*BLOCK_HEIGHT){
					//Colisão com a parte esquerda do bloco
					if(b->posx >= j*BLOCK_WIDTH - BALL_WIDTH/2 + BLOCK_WIDTH/2 && b->posx <= j*BLOCK_WIDTH + BLOCK_WIDTH){
						mapa[i][j]=0;
						b->stepx = -MOD(b->stepx);
						return;
					}
					//Colisão com a parte direita do bloco
					if(b->posx <= (j+1)*BLOCK_WIDTH + BALL_WIDTH/2 +BLOCK_WIDTH/2 && b->posx >= (j+1)*BLOCK_WIDTH){
						mapa[i][j]=0;
						b->stepx = MOD(b->stepx);
						return;
					}
				}
				//Colisão com parte superior do bloco
				if(b->posx > j*BLOCK_WIDTH + BLOCK_WIDTH/2 && b->posx < (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2){
					if(b->posy >= i*BLOCK_HEIGHT - BALL_HEIGHT/2 && b->posy <= i*BLOCK_HEIGHT ){
						mapa[i][j]=0;
						b->stepy = -MOD(b->stepy);
						return;
					}
					//Colisão com a parte inferior do bloco
					if(b->posy <= (i+1)*BLOCK_HEIGHT + BALL_HEIGHT/2 && b->posy >= (i+1)*BLOCK_HEIGHT ){
						mapa[i][j]=0;
						b->stepy = MOD(b->stepy);
						return;
					}
				}
				if(distancia(b->posx, b->posy, j*BLOCK_WIDTH + BLOCK_WIDTH/2, i*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					mapa[i][j] = 0;
					b->stepx = -MOD(b->stepx);
					b->stepy = -MOD(b->stepy);
					return;
				}
				if(distancia(b->posx, b->posy, (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2, i*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					mapa[i][j] = 0;
					b->stepx = MOD(b->stepx);
					b->stepy = -MOD(b->stepy);
					return;
				}
				if(distancia(b->posx, b->posy, j*BLOCK_WIDTH + BLOCK_WIDTH/2, (i+1)*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					mapa[i][j] = 0;
					b->stepx = -MOD(b->stepx);
					b->stepy = MOD(b->stepy);
					return;
				}
				if(distancia(b->posx, b->posy, (j+1)*BLOCK_WIDTH + BLOCK_WIDTH/2, (i+1)*BLOCK_HEIGHT) < BALL_WIDTH/2 - BALL_CORRECT){
					mapa[i][j] = 0;
					b->stepx = MOD(b->stepx);
					b->stepy = MOD(b->stepy);
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

int createPad(PAD *p, char *address){
	int success = true;
	p->image = loadSurface(address);
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
		p->vetor.x = (p->vetor.x)>0?(p->vetor.x-1):(p->vetor.x+1);
	}
}

double distancia(int x1, int y1, int x2, int y2){
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
