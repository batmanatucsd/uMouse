#include <stdio.h>
#include "floodfill.h"
#include "testmaze.h"

/*****************************************************************************/
// setup(unsigned char loc, unsigned char dist, unsigned char back):
// 		Initialize 16x16 maze using 2D array, assumption that no walls exist
/*****************************************************************************/
void setup(unsigned char loc, unsigned char dist, unsigned char step) 
{

	// Initialize maze
	for (unsigned short row = 0; row < 16; row++)
	{
		for (unsigned short col = 0; col < 16; col++)
		{
            maze[row][col] &= 0xff00;
            switch(step)
            {
                case 1:
                    maze[row][col] |= init(row, col);
                    break;
                case 2:
                    maze[row][col] |= initBack(row, col);
                    break;
                case 3:
                    maze[row][col] |= initFlood(row, col);
                    break;
            }
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
	location = loc;					// maze[15,0]
	direction = dist;				// 0x0 = up direction
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

unsigned short initBack(unsigned short row, unsigned short col) 
{	

	return (col + (0x0f - row));
}

unsigned short initFlood(unsigned short row, unsigned short col) 
{	
	return ((row == 7 && col == 7) ||
            (row == 7 && col == 8) ||
            (row == 8 && col == 7) ||
            (row == 8 && col == 8)) ? 0: 255;
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
		if (maze[row][col + 1] & EAST_WALL && col + 2 <= 15) 
		{
			maze[row][col + 2] |= WEST_WALL;
		}

		// if east cell has north wall, and not topmost row
		// set south wall of cell above east wall
		if (maze[row][col + 1] & NORTH_WALL && row - 1 >= 0) 
		{
			maze[row - 1][col + 1] |= SOUTH_WALL;
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
		if (maze[row + 1][col] & EAST_WALL && col + 1 <= 15) 
		{
			maze[row + 1][col + 1] |= WEST_WALL;
		}

		// if south cell has west wall, and column is at least second column,
		// set east wall of cell left of north wall
		if (maze[row + 1][col] & WEST_WALL && col - 1 >= 0)
		{
			maze[row + 1][col - 1] |= EAST_WALL;
		}
		

		// if south cell has south wall, and it is not bottom row,
		// set north wall on cell below south cell
		if (maze[row + 1][col] & SOUTH_WALL && row + 2 <= 15) 
		{
			maze[row + 2][col] |= NORTH_WALL;
		}
	}

	// Fill in maze info for cells around WEST CELL
	else if (direction == 3 && !(maze[row][col] & WEST_WALL))
	{
		maze[row][col - 1] |= testmaze[row][col - 1];
		if (maze[row][col - 1] & WEST_WALL && col - 2 >= 0) 
		{
			maze[row][col - 2] |= EAST_WALL;
		}

		if (maze[row][col - 1] & NORTH_WALL && row - 1 >= 0) 
		{
			maze[row - 1][col - 1] |= SOUTH_WALL;
		}

		if (maze[row][col - 1] & SOUTH_WALL && row + 1 <= 15) 
		{
			maze[row + 1][col - 1] |= NORTH_WALL;
		}
	}
}

/*****************************************************************************/
// move(unsigned char flood):
//		Move to the cell closest to the center
/*****************************************************************************/
void move(unsigned char flood)
{
	unsigned char row = (location & ROW) >> 4;
	unsigned char col = location & COL;
	if (direction == 0 && !(maze[row][col] & NORTH_WALL)) --row;
	else if (direction == 1 && !(maze[row][col] & EAST_WALL)) ++col;
	else if (direction == 2 && !(maze[row][col] & SOUTH_WALL)) ++row;
	else if (direction == 3 && !(maze[row][col] & WEST_WALL)) --col;
	location = (row << 4) | col;
	lookAhead();
    if (flood != 'f')
    {
        maze[row][col] |= VISITED;
        // Actual move
    }
}

void moveFast() {
	unsigned char row = (location & ROW) >> 4;
	unsigned char col = location & COL;
	unsigned char priority = 1;

	if ((maze[row-1][col] & VISITED) && (maze[row-1][col] & DIST) == (maze[row][col] & DIST) - 1)
	{
		direction = 0;
		--row;
		priority = 0;
	}
	else if ((maze[row][col+1] & VISITED) && (maze[row][col+1] & DIST) == (maze[row][col] & DIST) - 1)
	{
		direction = 1;
		++col;
		priority = 0;
	}
	else if ((maze[row+1][col] & VISITED) && (maze[row+1][col] & DIST) == (maze[row][col] & DIST) - 1)
	{
		direction = 2;
		++row;
		priority = 0;
	}
	else if ((maze[row][col-1] & VISITED) && (maze[row][col-1] & DIST) == (maze[row][col] & DIST) - 1)
	{
		direction = 3;
		--col;
		priority = 0;
	}

	if (priority)
	{
		if ((maze[row-1][col] & DIST) == (maze[row][col] & DIST) - 1)
		{
			direction = 0;
			--row;
		}
		else if ((maze[row][col+1] & DIST) == (maze[row][col] & DIST) - 1)
		{
			direction = 1;
			++col;
		}
		else if ((maze[row+1][col] & DIST) == (maze[row][col] & DIST) - 1)
		{
			direction = 2;
			++row;
		}
		else if ((maze[row][col-1] & DIST) == (maze[row][col] & DIST) - 1)
		{
			direction = 3;
			--col;
		}
	}
	location = (row << 4) | col;
}

/*****************************************************************************/
// turn():
//		Turn to the cell closest to the center
/*****************************************************************************/
void turn()
{
	unsigned char row = (location & ROW) >> 4;
	unsigned char col = location & COL;

	unsigned char min = 255;
	unsigned short tile = maze[row][col];

	// If there is no NORTH wall for this cell
	if (!(tile & NORTH_WALL))
	{
		if ((maze[row - 1][col] & DIST) < min)
		{
			min = maze[row - 1][col] & DIST;
			direction = 0;
		}
	}

	// If there is no EAST wall for this cell
	if (!(tile & EAST_WALL))
	{
		if ((maze[row][col + 1] & DIST) < min)
		{
			min = maze[row][col + 1] & DIST;
			direction = 1;
		}
	}

	// If there is no SOUTH wall for this cell
	if (!(tile & SOUTH_WALL))
	{
		if ((maze[row + 1][col] & DIST) < min)
		{
			min = maze[row + 1][col] & DIST;
			direction = 2;
		}
	}

	// If ther`e is no WEST wall for this cell
	if (!(tile & WEST_WALL))
	{
		if ((maze[row][col - 1] & DIST) < min)
		{
			min = maze[row][col - 1] & DIST;
			direction = 3;
		}
	}
	lookAhead();
}

/*****************************************************************************/
// update(unsigned short row, unsigned short col):
//
// 		Changes current cell distance and check neighbors if necessary
/*****************************************************************************/
void update(unsigned short row, unsigned short col)
{
	unsigned short tile = maze[row][col];

    if ((tile & DIST) == 0) return;

	// Minimum open neighbor
	unsigned char min = 255;

	// If there is no NORTH wall for this cell, and the north cell is closer
	// to the center than current min, make it the new 'min'
	if (!(tile & NORTH_WALL))
	{
		if ((maze[row - 1][col] & DIST) < min)
		{
			min = maze[row - 1][col] & DIST;
		}
	}

	// If there is no EAST wall for this cell, and east cell is closer
	// to the center than current min, make it the new 'min'
	if (!(tile & EAST_WALL))
	{
		if ((maze[row][col + 1] & DIST) < min)
		{
			min = maze[row][col + 1] & DIST;
		}
	}

	// If there is no SOUTH wall for this cell, and south cell is closer
	// to the center than current min, make it the new 'min'
	if (!(tile & SOUTH_WALL))
	{
		if ((maze[row + 1][col] & DIST) < min)
		{
			min = maze[row + 1][col] & DIST;
		}
	}

	// If there is no WEST wall for this cell, and west cell is closer
	// to the center than current min, make it the new 'min'
	if (!(tile & WEST_WALL))
	{
		if ((maze[row][col - 1] & DIST) < min)
		{
			min = maze[row][col - 1] & DIST;
		}
	}

	// If cell value is wrong, push open neighbors onto stack
	if (min + 1 != (tile & DIST))
	{
		// Update distance
		maze[row][col] &= 0xff00;
		maze[row][col] |= min + 1;

		// Push open neighbors onto stack
		if (!(maze[row][col] & NORTH_WALL))
		{
			stack[stackptr++] = ((row - 1) << 4) | col;
		}
		if (!(maze[row][col] & EAST_WALL))
		{
			stack[stackptr++] = (row << 4) | (col + 1);
		}
		if (!(maze[row][col] & SOUTH_WALL))
		{
			stack[stackptr++] = ((row + 1) << 4) | col;
		}
		if (!(maze[row][col] & WEST_WALL))
		{
			stack[stackptr++] = (row << 4) | (col - 1);
		}
	}
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

void debug()
{
	print();
	printf("location is: %d, %d\n", (location & ROW) >> 4, (location & COL));
	printf("direction is: %d\n", direction);
	unsigned short tile = maze[(location & ROW) >> 4][location & COL];
	printf("north: %d, east: %d, south: %d, west: %d\n", tile & NORTH_WALL, tile & EAST_WALL, tile & SOUTH_WALL, tile & WEST_WALL);
}

int main() {

	// Initialize maze and mouse location
	char name[99999];
    unsigned char tmpLoc, tmpDir;
	setup(0xf0, 0, 1);
	setupTest(0xf0, 0);

	printf("Maze in mouse memory: \n");
	print();

	// While location is not in one of the endpoint cells
	while (location != 0x77 && location != 0x78 &&
		location != 0x87 && location != 0x88)
	{
/*
	  	printf("Press RETURN to contine");
	    fgets(name, sizeof(name), stdin);
*/
		update((location & ROW) >> 4, location & COL);
		while (stackptr > 0)
		{
			--stackptr;
			update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);
		}
        tmpLoc = location;
        tmpDir = direction;
		turn();
		move('m');
		//debug();
	}
	print();

	/* Going from center->back */
	setup(location, direction, 2);

	while (location != 0xf0)
	{
/*
	  	printf("Press RETURN to contine");
	    fgets(name, sizeof(name), stdin);
*/
		update((location & ROW) >> 4, location & COL);
		while (stackptr > 0)
		{
			--stackptr;
			update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);
		}
		turn();
		move('m');
		//debug();
	}
	print();

    // Floodfill
    setup(tmpLoc, tmpDir, 3);
    location = tmpLoc;
    direction = tmpDir;
    
    // While location is not in one of the endpoint cells
	while (location != 0x77 && location != 0x78 &&
		location != 0x87 && location != 0x88)
	{
	  	printf("Press RETURN to contine");
	    fgets(name, sizeof(name), stdin);

		update((location & ROW) >> 4, location & COL);
		while (stackptr > 0)
		{
			--stackptr;
			update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);
		}
		turn();
		move('f');
		debug();
	}
	print();

    location = 0xf0;
    direction = 0;

	// While location is not in one of the endpoint cells
	while (location != 0x77 && location != 0x78 &&
		location != 0x87 && location != 0x88)
	{
	  	printf("Press RETURN to contine");
	    fgets(name, sizeof(name), stdin);
		moveFast();
		print();
	}
	print();
}
