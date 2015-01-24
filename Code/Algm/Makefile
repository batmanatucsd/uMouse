CC=gcc

CFLAGS=-Wall -std=c99

all: pid.o floodfill testmaze

floodfill: floodfill.o

floodfill.o: floodfill.h

pid.o: pid.h

testmaze: testmaze.o

testmaze.o: testmaze.h

clean:
	rm -f *.o floodfill

