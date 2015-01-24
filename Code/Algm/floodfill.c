#include <stdio.h>
#include "floodfill.h"
#include "testmaze.h"

/*****************************************************************************/
// setup():
// 		Initialize 16x16 maze using 2D array, assumption that no walls exist
/*****************************************************************************/
void setup() 
{
	// Initialize maze
	for (unsigned short row = 0; row < 16; row++)
	{
		for (unsigned short col = 0; col < 16; col++)
		{
			maze[row][col] = init(row, col);

			// Maze borders
			if (row == 0) maze[row][col] |= NORTH_WALL;
			if (col == 0) maze[row][col] |= WEST_WALL;
			if (col == 15) maze[row][col] |= EAST_WALL;
			if (row == 15) maze[row][col] |= SOUTH_WALL;
		}
	}

	// Initialize mouse, position in bottom left corner, facing up
	current = 0xf0;
	location = 0xf0;				// maze[15,0]
	direction = 0x0;				// 0x0 = up direction


}

/*****************************************************************************/
// init(unsigned short row, unsigned short col):
// 		Pre-fill cell with values that determine its distance from the center.
// 		Initially assumes a wall-less maze.
/*****************************************************************************/
unsigned short init(unsigned short row, unsigned short col) 
{	
	// 2nd and 4th quadrant
	if(row > 0x07) {
		row = 0x07 - (row - 0x08);
	}
	
	// 3rd and 4th quadrant
	if(col > 0x07) {
		col = 0x07 - (col - 0x08);
	}

	return 0x0e - col - row;
}

/*****************************************************************************/
// update(unsigned short row, unsigned short col):
//
// 		Changes current cell distance and check neighbors if necessary
/*****************************************************************************/
void update(unsigned short row, unsigned short col)
{
	unsigned short tile = maze[row][col];

	// Update wall map
	printf("\nTEST LOCATION:%u\n", location);
	printf("\nTEST TILE:%u\n\n", tile);
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

    // If (current row - 1) exists, and there is no NORTH wall for this cell
	if (row - 1 >= 0 && !(tile & NORTH_WALL))
	{
		if ((maze[row - 1][col] & DIST) < min)
		{
			min = maze[row - 1][col] & DIST;
			next = ((row - 1) << 4) | col;
			//direction = 0x0;
		}
	}

	// If (current col + 1) exists, and there is no EAST wall for this cell
	if (col + 1 <= 15 && !(tile & EAST_WALL))
	{
		if ((maze[row][col + 1] & DIST) < min)
		{
			min = maze[row][col + 1] & DIST;
			next = (row << 4) | (col + 1);
			//direction = 0x1;
		}
	}

	// If (current row + 1) exists, and there is no SOUTH wall for this cell
	if (row + 1 <= 15 && !(tile & SOUTH_WALL))
	{
		if ((maze[row + 1][col] & DIST) < min)
		{
			min = maze[row + 1][col] & DIST;
			next = ((row + 1) << 4) | col;
			//direction = 0x2;
		}
	}

	// If (current cel - 1) exists, and there is no WEST wall for this cell
	if (col - 1 >= 0 && !(tile & WEST_WALL))
	{
		if ((maze[row][col - 1] & DIST) < min)
		{
			min = maze[row][col - 1] & DIST;
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
		maze[row][col] &= 0xff00;
		maze[row][col] |= min + 1;

		// Push open neighbors onto stack
		if (row - 1 >= 0 && !(maze[row][col] & NORTH_WALL))
		{
			stack[stackptr++] = ((row - 1) << 4) | col;
		}
		if (col + 1 <= 15 && !(maze[row][col] & EAST_WALL))
		{
			stack[stackptr++] = (row << 4) | (col + 1);
		}
		if (row + 1 <= 15 && !(maze[row][col] & SOUTH_WALL))
		{
			stack[stackptr++] = ((row + 1) << 4) | col;
		}
		if (col - 1 >= 0 && !(maze[row][col] & WEST_WALL))
		{
			stack[stackptr++] = (row << 4) | (col - 1);
		}
	}
	current = next;
}

/*****************************************************************************/
// print():
// 		Print out the cell, with the mouse location designated with a carrot
// 		sign pointing in direction it faces
/*****************************************************************************/
void print() {

	for (unsigned short row = 0; row < 16; row++)
	{
		// North wall
		for (unsigned short col = 0; col < 16; col++)
		{
			printf("+%s", maze[row][col] & NORTH_WALL?
				"---": "   ");
		}
		printf("+\n");

		for (unsigned short col = 0; col < 16; col++)
		{
			printf("%s", maze[row][col] & WEST_WALL?
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
			else if (maze[row][col] & VISITED)
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
			printf("%2d", (maze[row][col] & DIST) % 100);
		}

		printf("%s\n", maze[row][15] & EAST_WALL?
			"|": " ");
	}

	// South wall
	for (unsigned short col = 0; col < 16; col++)
	{
		printf("+%s", maze[15][col] & SOUTH_WALL?
			"---": "   ");
	}
	printf("+\n");
}

int main() {

	// Initialize maze and mouse location
	char name[99999];
	setup();
	setupTest();

	// Push first cell into stack
	stack[stackptr++] = location;

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

}
