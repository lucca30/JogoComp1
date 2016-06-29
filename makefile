CC=gcc

CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT
#CFLAGS=-g -Wall -D_REENTRANT
IMAGE_FLAGS=-lSDL2_image
SFLAGS=-lSDL2
SOURCES=main.c
BINARIES=main

#all: ex0 ex1 ex2 ex3 ex4 ex5 ex6 ex7 grafico texto
all: $(BINARIES)

main: main.c
	$(CC) -o main main.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) 

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out
