#include <stdio.h>

#include "floodfill.h"

void setup() {

	// Initialize board
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
	direction = 0x0;
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

	unsigned char dist = stack[--stackptr];

	// Search valid neighbors
	neighbor = {255, 255, 255, 255};
	if (row - 1 >= 0 && !(board[row][col] & NORTH_WALL)) {
		neighbor[0] = board[row - 1][col] & DIST;
	}
	if (col + 1 <= 15 && !(board[row][col] & EAST_WALL)) {
		neighbor[1] = board[row][col + 1] & DIST;
	}
	if (row + 1 <= 15 && !(board[row][col] & SOUTH_WALL)) {
		neighbor[2] = board[row + 1][col] & DIST;
	}
	if (col - 1 >= 0 && !(board[row][col] & WEST_WALL)) {
		neighbor[3] = board[row][col - 1] & DIST;
	}

	// Minimum neighbor
	unsigned char min = neighbor[0];
	for (unsigned char i = 1; i < 4; i++) {
		if (min > neighbor[i]) min = neighbor[i];
	}

	// Update distance
	if (min + 1 != dist) {
		board[row][col] &= 0xffff0000;
		board[row][col] |= min + 1;
	}
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

			printf("%s", board[row][col] & WEST_WALL?
				"|": " ");

			// Location direction
			if (row == (location & ROW) >> 4 &&
				col == (location & COL)) {
				switch (direction) {
					case 0x0: printf("^");
								break;
					case 0x1: printf(">");
								break;
					case 0x2: printf("v");
								break;
					case 0x3: printf("<");
				}
			} else if (board[row][col] & VISITED) {
				printf("*");
			} else {
				printf(" ");
			}

			// Print 2 digits
			printf("%2d", (board[row][col] & DIST) % 100);
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
	stack[stackptr++] = location;
	print();
/*
	// Not at destination cells
	while (location != 0x77 || location != 0x78 ||
		location != 0x87 || location != 0x88) {
		while (stackptr > 0) {
			update(location & ROW >> 4, location & COL);
		}
	}
*/
}