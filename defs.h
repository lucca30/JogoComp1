#ifndef DEFS_H
#define DEFS_H

#define true 1
#define false 0

//GAME constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

//BLOCK constants
#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 30
#define BLOCK_ADDRESS1 "./img/block1.png"

//BACKGROUND constants
#define BACKGROUND_ADRESS1 "./img/teste.png"
#define TELAINICIAL_ADRESS1 "./img/telapr.png"
#define FUNDOPAUSE_ADRESS1 "./img/telapause.png"

#define BUTTON1_ADRESS1 "./img/button1.png"

//BALL constants
#define BALL_WIDTH 24
#define BALL_HEIGHT 24
#define BALL_ADRESS "./img/ball.png"
#define BALL_CORRECT 2 /* Diminuir o índice de colisões com a quina */
#define BALL_MOVE_PRECISION 100 /*Determinar uma escala de precisão para o movimento */

//PAD constants
#define PAD_WIDTH 200
#define PAD_HEIGHT 44
#define PAD_ADDRESS "./img/pad.png"
#define PAD_CORRECT 20 /* Corrigir o tamanho da base superior */
#define Vmax 5

//PAD colision constants
#define PAD_COL_1E_x 20-PAD_WIDTH/2
#define PAD_COL_1D_x 180-PAD_WIDTH/2
#define PAD_COL_1_y 0-PAD_HEIGHT/2
#define PAD_COL_2E_x 15-PAD_WIDTH/2
#define PAD_COL_2D_x 185-PAD_WIDTH/2
#define PAD_COL_2_y 13-PAD_HEIGHT/2
#define PAD_COL_3E_x 10-PAD_WIDTH/2
#define PAD_COL_3D_x 190-PAD_WIDTH/2
#define PAD_COL_3_y 25-PAD_HEIGHT/2
#define PAD_COL_4E_x 0-PAD_WIDTH/2
#define PAD_COL_4D_x 200-PAD_WIDTH/2
#define PAD_COL_4_y 40-PAD_HEIGHT/2

//MACROS
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

typedef struct _GAMESTATS{
	int moving_ball;
} GAMESTATS;

#endif
