//Protótipos
int init();

void closing();

SDL_Surface* loadSurface(char *address);
/*Retorna o endereço de uma superfície com a imagem desejada carregada,
 *Permite não usar variáveis globais pra supefícies dos sprites
 */

int imprimeMapa(int mapa[10][10]);

int createBlock(BLOCK *b, char *address);
