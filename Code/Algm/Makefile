CC=gcc

CFLAGS=-Wall -std=c99

all: pid.o floodfill 

floodfill: floodfill.o

floodfill.o: floodfill.h testmaze.h

pid.o: pid.h

clean:
	rm -f *.o floodfill

