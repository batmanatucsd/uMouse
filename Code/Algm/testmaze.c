/*****************************************************************************/
// CREATES TEST MAZES
//		The wall information will be hardcoded for now....
//		Walls are going to be stored in TILE 
//		
/*****************************************************************************/
#include <stdio.h>
#include "floodfill.h"
#include "testmaze.h"


/*****************************************************************************/
// setup()
// 		Initializes an empty board
/*****************************************************************************/
void setup() {

	// Initialize, empty 
	for (unsigned short row = 0; row < 16; row++) {
		for (unsigned short col = 0; col < 16; col++) {
			board[row][col] = init(row, col);
			if (row == 0) board[row][col] |= NORTH_WALL;
			if (col == 0) board[row][col] |= WEST_WALL;
			if (col == 15) board[row][col] |= EAST_WALL;
			if (row == 15) board[row][col] |= SOUTH_WALL;
		}
	}

	// Initialize mouse, position in bottom left corner, facing up
	location = 0xf0;				// board[15,0]
	direction = 0x0;				// 0x0 = up direction

	// Initialize the right wall for the bottom left corner
	board[15][0] = board[15][0] | WEST_WALL;


}

/*****************************************************************************/
// init(unsigned short row, unsigned short col)
// 		Pre-fill cell with values that determine its distance from the center.
// 		Initially assumes a wall-less maze.
/*****************************************************************************/
unsigned short init(unsigned short row, unsigned short col) {

	//For bottom half of maze, distance from center is the same as top half
	if(row > 0x07) {
		row = 0x07 - (row - 0x08);
	}

	//For right half of maze, distance from center is the same as left half
	if(col > 0x07) {
		col = 0x07 - (col - 0x08);
	}

	return 0x0e - col - row;
}
/*****************************************************************************/
// print():
// 		Print out the cell, with the mouse location designated with a carrot
// 		sign pointing in 
/*****************************************************************************/
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


int main () {

	// Initialize the right wall for the bottom left corner
	board[15][0] = board[15][0] | WEST_WALL;
	print();
}



