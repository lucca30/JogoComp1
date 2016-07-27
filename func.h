//Protótipos
int init();

void closing();
/*
 * Libera todas as superfícies armazenadas pela lixeira
 */

SDL_Surface* loadSurface(char *address);
/*
 * Retorna o endereço de uma superfície com a imagem desejada carregada,
 * Permite não usar variáveis globais pra supefícies dos sprites
 */

int imprimeMapa(GAMESTATS *game, BLOCK *b);
/*
 *  Interpreta a Projeção dos Blocos em formato de matriz e
 * faz o BLit na Tela
 */

int createBlock(BLOCK *b);
/*
 * Realiza uma atribuição básica dos elementos que o compoem (imagem)
 */

int createBall(BALL *b);
/*
 * Realiza uma atribuição aos elementos que a compoem
 */

int moveBall(BALL *b, PAD *p, GAMESTATS *game);
/*
 *  Responsável por "Blitar" movimento da bola, através do stepx e stepy
 * atribuídos a ela
 */

void colisao(BALL *b, PAD *p, GAMESTATS *game, PLAYERSTATS *player);
/*
 * Versão Final:
 * Identifica colisão com a extremidade da tela
 * Identifica colisão com os blocos
 * Identifica colisão com o PAD, na sua parte superior, quinas e laterais
 * Possui mecância de reação determinada pela localização do toque
 */

int createPad(PAD *p);
/*
 * Define os atributos do pad.
 */

int movePad(PAD *p);
/*
 * Move(baseado na velocidade) e blita o pad.
 */

void acceleratePad(PAD *p);
/*
 * Se teclas ESQ ou DIR forem pressionadas, a velocidade do PAD vai alterar
 */

double distancia(int x1, int y1, int x2, int y2);
/*
 * Retorna a distancia entre dois pontos
 */

int menuPause();
/*
 * Abre na tela um menu de pause.
 */

int createBackground(char *adress);
/*
 * Inicia o Background
 */

int blitBackground(void);
/*
 * Imprime Background
 */

int menuPrincipal(GAMESTATS *game, PLAYERSTATS *player);
/*
 * Menu principal
 */

void corrige(double *stepx);
 /*
  * Ajuste para vmax e vmin da bola
  */

void pushTrash(SDL_Surface *surf);
/*
 * Adiciona a superficie à pilha da lixeira
 */

void sortRank(PLAYERSTATS *player);
/*
 * Ordena os melhores jogadores por pontuação
 */

int compare(PLAYERSTATS *a, PLAYERSTATS *b);
/*
 * Usada na sortRank
 */

void loadLevel(GAMESTATS *game);
/*
 * Responsável por acessar o disco rígido em busca do arquivo Binário
 * que contém as informações do mapa
 */

int updatePlayer(PLAYERSTATS *Player, GAMESTATS *game, PAD *p);
/*
 * Atualiza as informações do jogador, tais como vida, score
 * Responsável por reconhecer fim de jogo e nova fase
 */

TTF_Font* preparaFonte(char* fonte, int size);
/*
 * Retorna arquivo do tipo TTF_Font* já iniciado com uma fonte e um tamanho.
 */

SDL_Surface* createSurfaceTTF(char* texto,TTF_Font* fonte,int colorR,int colorG,int colorB);
/*
 * Cria uma superfície baseado em uma string, um tipo de fonte, uma corRed, uma corGreen e uma corBlue.
 */

void printPlayerStats(PLAYERSTATS player, TTF_Font* fonteScore, GAMESTATS game);
/*
 * Responsável pelo blit superior in-game com as informações do Player
 */

void gameoverTela();
/*
 * Blita a tela de gameover
 */

void printRanking(void);
/*
 * Chamada por telaRanking, fica responsável em acessar o disco rígido para
 * leitura dos dados dos recordistas
 */

int telaRanking(void);
/*
 * Organiza toda a mecânica da tela de Ranking
 */

void telaLevel(GAMESTATS *game);
/*
 * Imprime a tela trasição de níveis
 */

void blitaNome(char nome[6], char stringTemp[6]);
/*
 * Encarregado de fazer o display do nome digitado pelo
 * Jogador no começo do game
 */

void logoTela(void);
/*
 * Chama o logo da produtora (nóis :P)
 */

int playSound(Mix_Chunk *effect);
/*
 * Responsável por trabalhar os áudios
 */
