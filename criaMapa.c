#include<stdio.h>

typedef struct _GAMESTATS{
	int moving_ball;
	int mapa[10][9];
	int level;
} GAMESTATS;


int main(void){
	FILE *pArq;
	GAMESTATS temp;
	int i, j, num, k;
	pArq = fopen("./level.bin", "w");
	temp.moving_ball = 0;
	scanf("%d", &k);
	for(;k>0;k--){
		for(i=0;i<10;i++){
			for(j=0;j<9;j++){
				scanf("%d", &num);
				temp.mapa[i][j] = num;
			}
		}
		fwrite(&temp, sizeof(GAMESTATS), 1, pArq);
	}
	return 0;
}
