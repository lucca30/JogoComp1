/*
 * Teste 2.2 de Computação 1
 *
 * Autores: Henrique Vermelho, Lucca Martins, Pedro Vitor
 *
 * Descrição: Este programa gera uma janela na qual contém duas imagens, ao clicarmos em uma delas,
 *            trocamos suas posições (uma pela outra). A janela deve fechar ao apertarmos Q, ESC ou
 *            quando apertarmos o botão fechar da janela.
 */

#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>


//Constantes

#define true 1
#define false 0

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define IMAGE_WIDTH 400
#define IMAGE_HEIGHT 300

#define ADDRESS1 "./img/background.jpg"
#define ADDRESS2 "./img/pad1.jpg"

#define POSX1 0
#define POSY1 0
#define POSX2 300
#define POSY2 200


typedef struct _IMAGE{
	int posX, posY;
	int imgW, imgH;
	SDL_Surface* image;
} IMAGE;
//imgW e imgH só serão necessárias caso as imagens pudessem ter tamanhos distintos

//Variáveis Globais

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;


//Protótipos

int init();

void closing();

SDL_Surface* loadSurface(char *address);
/*Retorna o endereço de uma superfície com a imagem desejada carregada,
 *Permite não usar variáveis globais pra supefícies dos sprites
 */

int createImage(IMAGE *dest , int posX, int posY, int imgW, int imgH, char *address);
//Atribui valores para as dimensões e posições da imagem. Address é o endereço da imagem a ser atribuída

int isIn(int x, int y);
//Determina se o clique foi ou não dentro de algum dos dois retângulos

int main(int argc, char* args[]){
	IMAGE image1, image2;
	image1.image = NULL;
	image2.image = NULL;
	int quit = false;
	SDL_Rect srcRect, dstRect;
	SDL_Event e;
	int tempX, tempY;
	int mouseX, mouseY;
	if(!init()){
  		printf("Failed to initialize!\n");
  	}
	else {
		if(!createImage(&image1, POSX1, POSY1, IMAGE_WIDTH, IMAGE_HEIGHT, ADDRESS1) || !createImage(&image2, POSX2, POSY2, IMAGE_WIDTH, IMAGE_HEIGHT, ADDRESS2))
		{
			printf("Failed to creat Images\n");
		}
		else {
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
						case SDL_MOUSEBUTTONDOWN:
							SDL_GetMouseState(&mouseX, &mouseY);
							if(isIn(mouseX, mouseY)){
								tempX = image1.posX;
								tempY = image1.posY;
								image1.posX = image2.posX;
								image1.posY = image2.posY;
								image2.posX = tempX;
								image2.posY = tempY;
							}
							break;
					}
				}
				SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x00, 0x88, 0xFF));

				srcRect.x = 0;
				srcRect.y = 0;

				srcRect.w = IMAGE_WIDTH;
				srcRect.h = IMAGE_HEIGHT;

				dstRect.x = image1.posX;
				dstRect.y = image1.posY;

				if(SDL_BlitSurface(image1.image, &srcRect, gScreenSurface, &dstRect) < 0)
				{
					printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
					quit = true;
				}

				dstRect.x = image2.posX;
				dstRect.y = image2.posY;

				if(SDL_BlitSurface(image2.image, &srcRect, gScreenSurface, &dstRect) < 0)
				{
					printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
					quit = true;
				}

				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}


	SDL_FreeSurface(image1.image);
	image1.image = NULL;
	SDL_FreeSurface(image2.image);
	image2.image = NULL;
	/*	Por se tratar de structs locais, faz-se necessário realizar o FreeSurface na main
	 *pois colocar parametros em closing é mais custoso
	 */
	closing();

	return 0;
}


int init()
{
    int success = true;

    if( SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else{
        gWindow = SDL_CreateWindow("Teste 2.2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else{
						//Habilita o carregamento de imagens JPG
						//Colocar "IMG_INIT_PNG" caso queira trabalhar com imagens PNG
            int imgFlags = IMG_INIT_JPG;
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

int createImage(IMAGE *dest, int posX, int posY, int imgW, int imgH, char *address){
	int success=true;
	//Atribuindo posição
	dest->posX = posX;
	dest->posY = posY;
	//Atribuindo dimensões
	dest->imgW = imgW;
	dest->imgH = imgH;
	//Atribuindo a superfície com a imagem carregada
	dest->image = loadSurface(address);
	if(dest->image==NULL){
   	printf("Failed to load media!\n");
		success = false;
	}

	return success;
}

int isIn(int x, int y){
	//Caso esteja dentro do retangulo 1
	if(x>POSX1 && x<POSX1+IMAGE_WIDTH && y>POSY1 && y<POSY1+ IMAGE_HEIGHT){
		return true;
	}
	//Caso esteja dentro do retângulo 2
	if(x>POSX2 && x<POSX2+IMAGE_WIDTH && y>POSY2 && y<POSY2+ IMAGE_HEIGHT){
		return true;
	}
	return false;
}
