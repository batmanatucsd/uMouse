CC=gcc

CFLAGS=-Wall -std=c99

all: floodfill floodfillback

floodfill: floodfill.o

floodfillback: floodfillback.o

floodfill.o: floodfill.c testmaze.h 

floodfillback.o: floodfillback.c testmaze.h

clean:
	rm -f *.o *.gch a.out floodfill

