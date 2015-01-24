CC=gcc

CFLAGS=-Wall

all: pid.o floodfill testmaze

floodfill: floodfill.o

floodfill.o: floodfill.h

pid.o: pid.h

testmaze: testmaze.o

testmaze.o: testmaze.h

clean:
	rm -f *.o

