#include <stdio.h>

#include "floodfill.h"

void setup()
{
	// Initialize board
	for (unsigned short row = 0; row < 16; row++)
	{
		for (unsigned short col = 0; col < 16; col++)
		{
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


void update(unsigned short row, unsigned short col)
{
	unsigned char tile = board[row][col];

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
    unsigned char min = tile;
    unsigned char next = (row << 4) | col;
	if (row - 1 >= 0 && !(tile & NORTH_WALL))
	{
		if ((board[row - 1][col] & DIST) < min)
		{
			min = board[row - 1][col] & DIST;
			next = ((row - 1) << 4) | col;
			direction = 0x0;
		}
	}
	if (col + 1 <= 15 && !(tile & EAST_WALL))
	{
		if ((board[row][col + 1] & DIST) < min)
		{
			min = board[row][col + 1] & DIST;
			next = (row << 4) | (col + 1);
			direction = 0x1;
		}
	}
	if (row + 1 <= 15 && !(tile & SOUTH_WALL))
	{
		if ((board[row + 1][col] & DIST) < min)
		{
			min = board[row + 1][col] & DIST;
			next = ((row + 1) << 4) | col;
			direction = 0x2;
		}
	}
	if (col - 1 >= 0 && !(tile & WEST_WALL))
	{
		if ((board[row][col - 1] & DIST) < min)
		{
			min = board[row][col - 1] & DIST;
			next = (row << 4) | (col - 1);
			direction = 0x3;
		}
	}

	location = next;

	if (min + 1 != (tile & DIST))
	{
		// Update distance
		board[row][col] &= 0xffff0000;
		board[row][col] |= min + 1;

		// Push open neighbors onto stack
		if (row - 1 >= 0 && !(board[row][col] & NORTH_WALL))
		{
			stack[stackptr++] = board[row - 1][col];
		}
		if (col + 1 <= 15 && !(board[row][col] & EAST_WALL))
		{
			stack[stackptr++] = board[row][col + 1];
		}
		if (row + 1 <= 15 && !(board[row][col] & SOUTH_WALL))
		{
			stack[stackptr++] = board[row + 1][col];
		}
		if (col - 1 >= 0 && !(board[row][col] & WEST_WALL))
		{
			stack[stackptr++] = board[row][col - 1];
		}
	}
}


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
			} else
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

int main()
{
	setup();

	// Push first cell into stack
	stack[stackptr++] = location;
	print();
	while (location != 0x77 && location != 0x78 &&
		location != 0x87 && location != 0x88)
	{
		update((location & ROW) >> 4, location & COL);
		print();
	}
}
