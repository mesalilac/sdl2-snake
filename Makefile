CC=gcc
CFLAGS=-Wall -Wextra -Werror
IDIR=src/include
LIBS=-lSDL2 -lSDL2_ttf
SRCS=src/main.c $(IDIR)/render.c $(IDIR)/logic.c
OUTPUT=snake

build:
	$(CC) $(CFLAGS) -I$(IDIR)/ -o $(OUTPUT) $(SRCS) $(LIBS)

run: build
	./$(OUTPUT)

