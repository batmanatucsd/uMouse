CC=gcc

CFLAGS=-Wall -std=c99

all: floodfill

floodfill: floodfill.o

floodfill.o: floodfill.c testmaze.h pid.h

pid.o: pid.h

clean:
	rm -f *.o floodfill

