CC = gcc
CFLAGS = -Wall -g
INCLUDE = -I./include
LIBS = -lGL -lGLU -lglut -lm

all: main opengl

main: main.c
	$(CC) $(CFLAGS) $(INCLUDE) main.c $(LIBS) -o main

opengl: opengl.c
	$(CC) $(CFLAGS) $(INCLUDE) opengl.c $(LIBS) -o opengl

clean:
	rm -f main opengl *.o debug.log
