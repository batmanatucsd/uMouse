#include <stdio.h>

#include "floodfill.h"

void setup() {

	// Initialize the board
	for (unsigned short row = 0; row < 16; row++) {
		for (unsigned short col = 0; col < 16; col++) {
			board[row][col] = init(row, col);
		}
	}
	// Alternative for loop
	/*  for (unsigned short cell = 0; cell < 256; cell++) {
			 board[cell / 16][cell % 16] = init(cell / 16, cell % 16);
		}
	*/
}

unsigned short init(unsigned short row, unsigned short col) {
	
	if(row > 0x07) {
		row = 0x07 - (row - 0x08);
	}
	
	if(col > 0x07) {
		col = 0x07 - (col - 0x08);
	}

	return 0x0e - col - row;
}


void update(unsigned short row, unsigned short col) {
	unsigned short cell = stack[--stackptr];
	// Update 4 walls and neighbors
}


void print() {

	for (unsigned short row = 0; row < 16; row++) {
		for (unsigned short col = 0; col < 16; col++) {
			// TODO
			// Print walls and posts
			printf("%d\t", board[row][col] & VALUE);
		}
		printf("\n");
	}
}

int main() {
	setup();

	// Push first cell into stack
	stack[stackptr++] = board[15][0];
	print();

	while (stackptr > 0) {
		update(15, 0);
	}
}