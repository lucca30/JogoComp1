/*
 * Esse programa le o ranking.bin para diagnóstico. Compilar manualmente.
 */

#include <stdio.h>

typedef struct _PLAYERSTATS{
	char playerName[5];
	int score;
	int lives;
} PLAYERSTATS;

int main(void) {
    PLAYERSTATS lidos[10];
    FILE *file1;
    int i;

    file1 = fopen("./ranking.bin", "r");
    fread(lidos, sizeof(PLAYERSTATS), 10, file1);
    for(i = 0; i < 10; i++) {
        printf("pontos %d\n", lidos[i].score);
    }

    fclose(file1);

    return 0;


}
