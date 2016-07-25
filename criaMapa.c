#include<stdio.h>

typedef struct _GAMESTATS{
	int moving_ball;
	int mapa[10][9];
	int level;
	int total_level_score;
} GAMESTATS;


int main(void){
	FILE *pArq;
	GAMESTATS temp;
	int i, j, num, k;
	pArq = fopen("./level.bin", "w");
	temp.moving_ball = 0;
	scanf("%d", &k);
	for(;k>0;k--){
		temp.total_level_score = 0;
		for(i=0;i<10;i++){
			for(j=0;j<9;j++){
				scanf("%d", &num);
				temp.mapa[i][j] = num;
				temp.total_level_score += num*100;
			}
		}
		fwrite(&temp, sizeof(GAMESTATS), 1, pArq);
	}
	return 0;
}
