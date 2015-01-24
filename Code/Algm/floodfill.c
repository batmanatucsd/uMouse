#include <stdio.h>

#include "floodfill.h"

/*****************************************************************************/
// setup
//
// Initializes the board with the assumption that no walls exist
/*****************************************************************************/
void setup()
{
	// Initialize board
	for (unsigned short row = 0; row < 16; row++)
	{
		for (unsigned short col = 0; col < 16; col++)
		{
			board[row][col] = init(row, col);

			// Maze borders
			if (row == 0) board[row][col] |= NORTH_WALL;
			if (col == 0) board[row][col] |= WEST_WALL;
			if (col == 15) board[row][col] |= EAST_WALL;
			if (row == 15) board[row][col] |= SOUTH_WALL;
		}
	}

	// Initialize mouse
	current = 0xf0;
	location = 0xf0;
	direction = 0x0;
}

/*****************************************************************************/
// init
//
// Calculates the distance from given cell to center (no walls)
/*****************************************************************************/
unsigned short init(unsigned short row, unsigned short col)
{	
	if(row > 0x07) {
		row = 0x07 - (row - 0x08);
	}
	
	if(col > 0x07) {
		col = 0x07 - (col - 0x08);
	}

	return 0x0e - col - row;
}

/*****************************************************************************/
// update
//
// Changes current cell distance and check neighbors if necessary
/*****************************************************************************/
void update(unsigned short row, unsigned short col)
{
	unsigned short tile = board[row][col];

	// Update wall map
/*
	// left wall
	if (abs(analogRead(IR_IN1) - THRESHOLD) > abs(analogREAD(IR_IN1) - leftAmbientLight))
	{
	}
	// right wall
	...
	// front wall
	...
	// Also update adjacent cells
*/

	// TODO
	// Once we figure out the encoding for the gyro we can probably optimize the open neighbor code
	// TODO

	// Minimum open neighbor
    unsigned char min = 255;
    unsigned char next = (row << 4) | col;
	if (row - 1 >= 0 && !(tile & NORTH_WALL))
	{
		if ((board[row - 1][col] & DIST) < min)
		{
			min = board[row - 1][col] & DIST;
			next = ((row - 1) << 4) | col;
			//direction = 0x0;
		}
	}
	if (col + 1 <= 15 && !(tile & EAST_WALL))
	{
		if ((board[row][col + 1] & DIST) < min)
		{
			min = board[row][col + 1] & DIST;
			next = (row << 4) | (col + 1);
			//direction = 0x1;
		}
	}
	if (row + 1 <= 15 && !(tile & SOUTH_WALL))
	{
		if ((board[row + 1][col] & DIST) < min)
		{
			min = board[row + 1][col] & DIST;
			next = ((row + 1) << 4) | col;
			//direction = 0x2;
		}
	}
	if (col - 1 >= 0 && !(tile & WEST_WALL))
	{
		if ((board[row][col - 1] & DIST) < min)
		{
			min = board[row][col - 1] & DIST;
			next = (row << 4) | (col - 1);
			//direction = 0x3;
		}
	}

	// Continue if distances are correct
	if (stackptr == 0 && min + 1 == (tile & DIST))
	{
		current = next;
		stack[stackptr++] = next;
	}
	else if (min + 1 != (tile & DIST))
	{
		// Update distance
		board[row][col] &= 0xff00;
		board[row][col] |= min + 1;

		// Push open neighbors onto stack
		if (row - 1 >= 0 && !(board[row][col] & NORTH_WALL))
		{
			stack[stackptr++] = ((row - 1) << 4) | col;
		}
		if (col + 1 <= 15 && !(board[row][col] & EAST_WALL))
		{
			stack[stackptr++] = (row << 4) | (col + 1);
		}
		if (row + 1 <= 15 && !(board[row][col] & SOUTH_WALL))
		{
			stack[stackptr++] = ((row + 1) << 4) | col;
		}
		if (col - 1 >= 0 && !(board[row][col] & WEST_WALL))
		{
			stack[stackptr++] = (row << 4) | (col - 1);
		}
	}
	current = next;
}

/*****************************************************************************/
// print
//
// Print out current state of maze
/*****************************************************************************/
void print() {

	for (unsigned short row = 0; row < 16; row++)
	{
		// North wall
		for (unsigned short col = 0; col < 16; col++)
		{
			printf("+%s", board[row][col] & NORTH_WALL?
				"---": "   ");
		}
		printf("+\n");

		for (unsigned short col = 0; col < 16; col++)
		{
			printf("%s", board[row][col] & WEST_WALL?
				"|": " ");

			// Location direction
			if (row == (location & ROW) >> 4 &&
				col == (location & COL))
			{
				switch (direction)
				{
					case 0x0: printf("^");
								break;
					case 0x1: printf(">");
								break;
					case 0x2: printf("v");
								break;
					case 0x3: printf("<");
				}
			}
			else if (board[row][col] & VISITED)
			{
				printf("*");
			}
			else if (row == (current & ROW) >> 4 &&
				col == (current & COL))
			{
				printf("c");
			}
			else
			{
				printf(" ");
			}

			// Print 2 digits
			printf("%2d", (board[row][col] & DIST) % 100);
		}

		printf("%s\n", board[row][15] & EAST_WALL?
			"|": " ");
	}

	// South wall
	for (unsigned short col = 0; col < 16; col++)
	{
		printf("+%s", board[15][col] & SOUTH_WALL?
			"---": "   ");
	}
	printf("+\n");
}

int main() {
  	char name[99999];
	setup();

	// Add test walls here
	for (int i = 0; i <= 15; i++)
	{
		if (i != 10)
		{
			board[i][0] |= EAST_WALL; board[i][1] |= WEST_WALL;
		}
	}
	// end

	// Push first cell into stack
	stack[stackptr++] = current;
	
	while (current != 0x77 && current != 0x78 &&
		current != 0x87 && current != 0x88)
	{
	  	printf("Press RETURN to contine");
    	fgets(name, sizeof(name), stdin);
		printf("Current cell: %d,%d\n", (stack[stackptr - 1] & ROW) >> 4, stack[stackptr - 1] & COL);
		printf("Current stack: ");
		for (int i = 0; i < stackptr; i++)
			printf("(%d, %d)", (stack[i] & ROW) >> 4, stack[i] & COL);
		printf("\n");
		print();
		--stackptr;
		update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);
	}
	print();
}
