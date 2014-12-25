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

	// Initialize mouse
	location = 0xf0;
	direction = 0x1;
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
	
	// Update 4 walls and neighbors
	unsigned short dist = stack[--stackptr] & DIST;
}


void print() {

	for (unsigned short row = 0; row < 16; row++) {

		// North wall
		for (unsigned short col = 0; col < 16; col++) {
			printf("+%s", board[row][col] & NORTH_WALL?
				"---": "   ");
		}
		printf("+\n");

		for (unsigned short col = 0; col < 16; col++) {

			// West wall
			printf("%s", board[row][col] & WEST_WALL?
				"|": " ");

			// Location direction
			if (row == (location & ROW) >> 4 &&
				col == (location & COL)) {
				switch (direction) {
					case 0x1: printf("^");
								break;
					case 0x2: printf(">");
								break;
					case 0x4: printf("v");
								break;
					case 0x8: printf("<");
				}
			} else if (board[row][col] & VISITED) {
				printf("*");
			} else {
				printf(" ");
			}
			printf("%-2d", board[row][col] & DIST);
		}
		printf("%s\n", board[row][15] & EAST_WALL?
			"|": " ");
	}

	// South wall
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
	while (dist > 0) {
		while (stackptr > 0) {
			update(15, 0);
		}
	}
*/
}