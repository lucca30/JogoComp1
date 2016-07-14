CC=gcc

CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT
#CFLAGS=-g -Wall -D_REENTRANT
IMAGE_FLAGS=-lSDL2_image
SFLAGS=-lSDL2
MATH_FLAGS=-lm
SOURCES=main.c func.c globais.c
OBJECTS=main.o func.o globais.o
BINARIES=main

all: $(BINARIES)

main: main.o func.o globais.o
	$(CC) -o main main.o func.o globais.o $(SFLAGS) $(IMAGE_FLAGS) $(MATH_FLAGS)

main.o: main.c
	$(CC) -c main.c  $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(MATH_FLAGS)

func.o: func.c
	$(CC) -c func.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(MATH_FLAGS)

globais.o: globais.c
	$(CC) -c globais.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(MATH_FLAGS)

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out

limpa:
	del *.o *.exe *.bak *.c~ $(BINARIES) core
