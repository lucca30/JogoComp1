//Protótipos
int init();

void closing();

SDL_Surface* loadSurface(char *address);
/*
 * Retorna o endereço de uma superfície com a imagem desejada carregada,
 * Permite não usar variáveis globais pra supefícies dos sprites
 */
int imprimeMapa(int mapa[10][10]);
/*
 *  Interpreta a Projeção dos Blocos em formato de matriz e
 * faz o BLit na Tela
 */
int createBlock(BLOCK *b, char *address);
/*
 * Realiza uma atribuição básica dos elementos que o compoem (imagem)
 */
int createBall(BALL *b, char *address);
/*
 * Realiza uma atribuição aos elementos que a compoem
 */
int moveBall(BALL *b);
/*
 *  Responsável por "Blitar" movimento da bola, através do stepx e stepy
 * atribuídos a ela
 */
void colisao(BALL *b);
/*
 * Versão Alpha:
 * Identifica apenas colisão com a extremidade da tela
 */
int createPad(PAD *p, char *address);
/*
 * Define os atributos do pad.
 */
int movePad(PAD *p);
/*
 * Move e blita o pad.
 */
void aceleratePad(PAD *p);
/*
 * Se teclas ESQ ou DIR forem pressionadas, move o pad.
 */
