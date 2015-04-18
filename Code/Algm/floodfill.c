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
		}
	}
	for (unsigned short i = 0; i < 16; i++)
	{
		maze[0][i] |= NORTH_WALL;
		maze[i][0] |= WEST_WALL;
		maze[i][15] |= EAST_WALL;
		maze[15][i] |= SOUTH_WALL;
	}

	// Initialize mouse, position in bottom left corner, facing up
	current = 0xf0;
	location = 0xf0;				// maze[15,0]
	direction = 0x0;				// 0x0 = up direction
	maze[15][0] |= EAST_WALL;
	maze[15][1] |= WEST_WALL;
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
// lookAhead():
// 		Reads in the walls of the cell ahead of the mouse into memory
/*****************************************************************************/
void lookAhead()
{
	// Grab the row and column bits
	unsigned char row = (location & ROW) >> 4;
	unsigned char col = location & COL;

	// Fill in maze info for cells around NORTH CELL
	if (direction == 0 && !(maze[row][col] & NORTH_WALL))
	{
		maze[row - 1][col] |= testmaze[row - 1][col];

		// if north cell has west wall, and column is at least second column,
		// set east wall of cell left of north wall
		if (maze[row - 1][col] & WEST_WALL && col - 1 >= 0)
		{
			maze[row - 1][col - 1] |= EAST_WALL;
		}

		// if north cell has north wall, and there are at least 3 cells ahead
		// set south wall on cell above north wall 
		if (maze[row - 1][col] & NORTH_WALL && row - 2 >= 0)
		{
			maze[row - 2][col] |= SOUTH_WALL;
		}

		// if north cell has east wall, and there are columns to right of north cell
		// set west wall on cell to the right of the north cell
		if (maze[row - 1][col] & EAST_WALL && col + 1 <= 15)
		{
			maze[row - 1][col + 1] |= WEST_WALL;
		}
	}

	// Fill in maze info for cells around EAST CELL
	else if (direction == 1 && !(maze[row][col] & EAST_WALL))
	{
		maze[row][col + 1] |= testmaze[row][col + 1];

		// if east cell has east wall, and column is at least second clumn,
		// set the west wall of cell to right of east cell
		if (maze[row][col + 1] & EAST_WALL && col + 2 >= 15) 
		{
			maze[row][col + 2] |= WEST_WALL;
		}

		// if east cell has north wall, and not topmost row
		// set south wall of cell above east wall
		if (maze[row][col + 1] & NORTH_WALL && row - 1 >= 0) 
		{
			maze[row - 1][col - 1] |= SOUTH_WALL;
		}

		// if east cell has south wall, and it is not bottom row,
		// set north wall on cell below east cell
		if (maze[row][col + 1] & SOUTH_WALL && row + 1 <= 15) 
		{
			maze[row + 1][col + 1] |= NORTH_WALL;
		}
	}

	// Fill in maze info for cells around SOUTH CELL
	else if (direction == 2 && !(maze[row][col] & SOUTH_WALL))
	{
		maze[row + 1][col] |= testmaze[row + 1][col];
		
		// if south cell has east wall, and column is at least second column,
		// set the west wall of cell to the right of east cell
		if (maze[row + 1][col] & EAST_WALL && col + 2 >= 15) 
		{
			maze[row + 1][col + 1] |= WEST_WALL;
		}

		// if south cell has west wall, and column is at least second column,
		// set east wall of cell left of north wall
		if (maze[row - 1][col] & WEST_WALL && col - 1 >= 0)
		{
			maze[row + 1][col - 1] |= EAST_WALL;
		}
		
	}

	// Fill in maze info for cells around WEST CELL
	else if (direction == 3 && !(maze[row][col] & WEST_WALL))
	{
		// Fill in maze info for cells around west cell 
	}
}

/*****************************************************************************/
// move():
//		Move to the cell closest to the center
/*****************************************************************************/
void move()
{
	unsigned char row = (location & ROW) >> 4;
	unsigned char col = location & COL;

	unsigned char min = 255;
	unsigned char next = (row << 4) | col;
	unsigned short tile = maze[row][col];

	// If there is no NORTH wall for this cell
	if (!(tile & NORTH_WALL))
	{
		if ((maze[row - 1][col] & DIST) < min)
		{
			min = maze[row - 1][col] & DIST;
			next = ((row - 1) << 4) | col;
			direction = 0;
		}
	}

	// If there is no EAST wall for this cell
	if (!(tile & EAST_WALL))
	{
		if ((maze[row][col + 1] & DIST) < min)
		{
			min = maze[row][col + 1] & DIST;
			next = (row << 4) | (col + 1);
			direction = 1;
		}
	}

	// If there is no SOUTH wall for this cell
	if (!(tile & SOUTH_WALL))
	{
		if ((maze[row + 1][col] & DIST) < min)
		{
			min = maze[row + 1][col] & DIST;
			next = ((row + 1) << 4) | col;
			direction = 2;
		}
	}

	// If there is no WEST wall for this cell
	if (!(tile & WEST_WALL))
	{
		if ((maze[row][col - 1] & DIST) < min)
		{
			min = maze[row][col - 1] & DIST;
			next = (row << 4) | (col - 1);
			direction = 3;
		}
	}
	location = next;
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
	printf("\nTEST LOCATION:%u,%u\n", (location & ROW) >> 4, location & COL);
	printf("\nTEST TILE:%u\n\n", tile & DIST);
	printf("Direction:%u\n", direction);

	// Minimum open neighbor
	unsigned char min = 255;
	unsigned char next = (row << 4) | col;

	// If there is no NORTH wall for this cell
	if (!(tile & NORTH_WALL))
	{
		if ((maze[row - 1][col] & DIST) < min)
		{
			min = maze[row - 1][col] & DIST;
			next = ((row - 1) << 4) | col;
		}
	}

	// If there is no EAST wall for this cell
	if (!(tile & EAST_WALL))
	{
		if ((maze[row][col + 1] & DIST) < min)
		{
			min = maze[row][col + 1] & DIST;
			next = (row << 4) | (col + 1);
		}
	}

	// If there is no SOUTH wall for this cell
	if (!(tile & SOUTH_WALL))
	{
		if ((maze[row + 1][col] & DIST) < min)
		{
			min = maze[row + 1][col] & DIST;
			next = ((row + 1) << 4) | col;
		}
	}

	// If there is no WEST wall for this cell
	if (!(tile & WEST_WALL))
	{
		if ((maze[row][col - 1] & DIST) < min)
		{
			min = maze[row][col - 1] & DIST;
			next = (row << 4) | (col - 1);
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
// print(unsigned short m):
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

	printf("Maze in mouse memory: \n");
	print();
	// Push first cell into stack
	stack[stackptr++] = location;

	// While location is not in one of the endpoint cells
	while (location != 0x77 && location != 0x78 &&
		location != 0x87 && location != 0x88)
	{
	  	printf("Press RETURN to contine");
	    fgets(name, sizeof(name), stdin);
		lookAhead();
		print();

		while (stackptr > 1)
		{
			--stackptr;
			update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);

		    	// DEBUG
			printf("Current cell: %d,%d\n", (stack[stackptr - 1] & ROW) >> 4, stack[stackptr - 1] & COL);
			printf("Current stack: ");
			for (int i = 0; i < stackptr; i++)
				printf("(%d, %d)", (stack[i] & ROW) >> 4, stack[i] & COL);
			printf("\n");
			// DEBUG
		}
		move();
	}

}
