#include <stdio.h>

#include "floodfill.h"

void setup() {

	// Initialize the board
	for (unsigned short row = 0; row < 16; row++) {
		for (unsigned short col = 0; col < 16; col++) {
			board[row][col] = init(row, col);
			if (row == 0) board[row][col] |= NORTH_WALL;
			if (col == 0) board[row][col] |= WEST_WALL;
			if (col == 15) board[row][col] |= EAST_WALL;
			if (row == 15) board[row][col] |= SOUTH_WALL;
		}
	}
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
			printf("+%s", board[row][col] & NORTH_WALL?
				"---": "   ");
		}
		printf("+\n");
		for (unsigned short col = 0; col < 16; col++) {
			printf("%s%3d", board[row][col] & WEST_WALL?
				"|": " ", board[row][col] & VALUE);
			// TODO Print the visited cells and mouse location and direction
		}
		printf("%s\n", board[row][15] & EAST_WALL?
			"|": " ");
	}
	for (unsigned short col = 0; col < 16; col++) {
		printf("+%s", board[15][col] & SOUTH_WALL?
			"---": "   ");
	}
	printf("+\n");
}

int main() {
	setup();

	// Push first cell into stack
	stack[stackptr++] = board[15][0];
	print();
/*
	//while (dist > 0)
	while (stackptr > 0) {
		update(15, 0);
	}
*/
}