#ifndef DEFS_H
#define DEFS_H

#define true 1
#define false 0

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 30

#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 30
#define BLOCK_ADDRESS1 "./img/block1.jpg"

#define BALL_WIDTH 24
#define BALL_HEIGHT 24
#define BALL_ADRESS "./img/ball.png"

#define PAD_WIDTH 200
#define PAD_HEIGHT 80
#define PAD_ADDRESS "./img/pad.png"
#define PAD_CORRECT 20 /* Corrigir o tamanho da base superior */
#define Vmax 10

#define MOD(a)a>0?a:-a

typedef struct _BLOCK{
	SDL_Surface* image;
} BLOCK;

typedef struct _BALL{
	SDL_Surface* image;
	int posx, posy; //Relativos ao centro da bola
	int stepx, stepy;
} BALL;

typedef struct _VETOR{
	float x, y;
} VETOR;

typedef struct _PAD{
	SDL_Surface* image;
	int posx, posy;
	VETOR vetor;
} PAD;

#endif
