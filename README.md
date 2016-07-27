# CastleBreak #

## Projeto de Jogo de Computação 1 - UFRJ 2016.1 ##

## Autores ##

* Henrique Vermelho (DRE:116076549)
* Lucca Martins (DRE:116058800)
* Pedro Nascimento (DRE:116033748)

## Manual de Jogo ##

1 – Como compilar:
	Para compilar o jogo você deve ter SDL2 instalado em seu computador. 
	Se já estiver instalado, basta abrir o terminal na pasta do jogo e executar 
	o comando “make”. Em seguida execute “./main” para jogar.

2 – Como jogar:
	Assim que iniciar o jogo estará de frente para o menu onde você pode jogar ou 
	ver o ranking dos jogadores. Antes de jogar você deve Digitar um nickname (apelido) 
	para ficar gravado no sistema.
	Para navegar entre as opções utilize as setas do teclado.

3 – Comando básicos:
	Em CastleBreak você pode deslocar seu Pad para direita e esquerda e rebater a bola em 
	jogo ao entrar em contato com a mesma.

4 – Objetivo e Mecânica:
	O objetivo do jogador e conseguir o máximo de pontos quebrando os blocos (cada um dá 100 
	pontos). Blocos de cores diferentes tem resistência diferente variando entre um e três “hits” 
	para “morte”. O jogador perde uma vida quando a bola encosta na parte de baixo da tela. 
	Ele começa com 3 vidas e ganha uma a cada 10000 pontos. Com a lateral diagonal do Pad a bola 
	pode se rebatida de forma a voltar pelo caminho que veio. De acordo com o posicionamento da 
	bola ao bater no Pad sua velocidade muda (mais devagar próximo ao meio).
	Durante o game o jogador tem acesso a sua pontuação, sua quantidade de vida e ao nível que 
	chegou. Cada nível tem um mapa diferente.
