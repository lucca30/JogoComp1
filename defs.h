#ifndef DEFS_H
#define DEFS_H

#define true 1
#define false 0

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 30
#define BLOCK_ADDRESS1 "./img/block1.jpg"

typedef struct _BLOCK{
	SDL_Surface* image;
} BLOCK;

#endif
