/*
 * Esse programa preenche o vetor ranking.bin com valores iniciais. Compilar
 * manualmente.
 */

#include <stdio.h>

typedef struct _PLAYERSTATS{
	char playerName[5];
	int score;
	int lives;
} PLAYERSTATS;

int main(void) {
    int i;
    FILE *file1;
    PLAYERSTATS jogadores[10];

    for(i = 0; i < 10; i++) {
        jogadores[i].score = 100;
    }

    file1 = fopen("ranking.bin", "w");
    if(!file1) return 666;
    fwrite(jogadores, sizeof(PLAYERSTATS), 10, file1);

    fclose(file1);
    return 0;
}
