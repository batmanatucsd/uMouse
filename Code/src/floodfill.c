#include "floodfill.h"

uint16_t testmaze[16][16];

/*****************************************************************************/
// setup(uint8_t loc, uint8_t dist, uint8_t back):
// 		Initialize 16x16 maze using 2D array, assumption that no walls exist
/*****************************************************************************/
void setup(uint8_t loc, uint8_t dist, uint8_t step)/*{{{*/
{
<<<<<<< HEAD
	uint16_t row, col,i;

	// Initialize maze
	for (row = 0; row < 16; row++)
	{
		for (col = 0; col < 16; col++)
		{
=======
  uint16_t i, row, col;

	// Initialize maze
	for (row = 0; row < 16; row++) {
		for (col = 0; col < 16; col++) {
>>>>>>> d8c63bb26d9bd3d686e8f8bee02724a034dcd19a
            maze[row][col] &= 0xff00;
            switch(step) {
                case 1:
                    maze[row][col] |= init(row, col);
                    break;
                case 2:
                    maze[row][col] |= initBack(row, col);
                    break;
                case 3:
                    maze[row][col] |= initFlood(row, col);
                    break;
<<<<<<< HEAD
            }
		}
	}
=======
            } // @end of switch
		} // @end of inner for loop
	} //@end of outter for loop

>>>>>>> d8c63bb26d9bd3d686e8f8bee02724a034dcd19a
	for (i = 0; i < 16; i++)
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
}/*}}}*/

/*****************************************************************************/
// init(uint16_t row, uint16_t col):
// 		Pre-fill cell with values that determine its distance from the center.
// 		Initially assumes a wall-less maze.
/*****************************************************************************/
uint16_t init(uint16_t row, uint16_t col)/*{{{*/
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
}/*}}}*/

uint16_t initBack(uint16_t row, uint16_t col)/*{{{*/
{
	return (col + (0x0f - row));
}/*}}}*/

uint16_t initFlood(uint16_t row, uint16_t col)/*{{{*/
{
	return ((row == 7 && col == 7) ||
            (row == 7 && col == 8) ||
            (row == 8 && col == 7) ||
            (row == 8 && col == 8)) ? 0: 255;
}/*}}}*/

/*****************************************************************************/
// lookAhead():
// 		Reads in the walls of the cell ahead of the mouse into memory
/*****************************************************************************/
void lookAhead(void)/*{{{*/
{
	// Grab the row and column bits
	uint8_t row = (location & ROW) >> 4;
	uint8_t col = location & COL;

	// Fill in maze info for cells around NORTH CELL
	if (direction == 0 && !(maze[row][col] & NORTH_WALL))/*{{{*/
	{
        // TODO: change to sensor readings
		maze[row - 1][col] |= testmaze[row - 1][col];
        // TODO

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
	}/*}}}*/

	// Fill in maze info for cells around EAST CELL
	else if (direction == 1 && !(maze[row][col] & EAST_WALL))/*{{{*/
	{
        // TODO: change to sensor readings
		maze[row][col + 1] |= testmaze[row][col + 1];
        // TODO

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
	}/*}}}*/

	// Fill in maze info for cells around SOUTH CELL
	else if (direction == 2 && !(maze[row][col] & SOUTH_WALL))/*{{{*/
	{
        // TODO: change to sensor readings
		maze[row + 1][col] |= testmaze[row + 1][col];
        // TODO

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
	}/*}}}*/

	// Fill in maze info for cells around WEST CELL
	else if (direction == 3 && !(maze[row][col] & WEST_WALL))/*{{{*/
	{
        // TODO: change to sensor readings
		maze[row][col - 1] |= testmaze[row][col - 1];
        // TODO

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
	}/*}}}*/
}/*}}}*/

/*****************************************************************************/
// move(uint8_t flood):
//		Move to the cell closest to the center
/*****************************************************************************/
void move(uint8_t flood)/*{{{*/
{
	uint8_t row = (location & ROW) >> 4;
	uint8_t col = location & COL;
	if (direction == 0 && !(maze[row][col] & NORTH_WALL)) --row;
	else if (direction == 1 && !(maze[row][col] & EAST_WALL)) ++col;
	else if (direction == 2 && !(maze[row][col] & SOUTH_WALL)) ++row;
	else if (direction == 3 && !(maze[row][col] & WEST_WALL)) --col;
	location = (row << 4) | col;
	lookAhead();
  if (flood != 'f')
  {
      maze[row][col] |= VISITED;
      //TODO
      forward();
      //TODO
  }
}/*}}}*/

void moveFast(void)/*{{{*/
{
	uint8_t row = (location & ROW) >> 4;
	uint8_t col = location & COL;
	uint8_t priority = 1;
  uint8_t tmp = direction;

	if ((maze[row-1][col] & VISITED) && (maze[row-1][col] & DIST) == (maze[row][col] & DIST) - 1) {/*{{{*/
		direction = 0;
		--row;
		priority = 0;
	}
	else if ((maze[row][col+1] & VISITED) && (maze[row][col+1] & DIST) == (maze[row][col] & DIST) - 1) {
		direction = 1;
		++col;
		priority = 0;
	}
	else if ((maze[row+1][col] & VISITED) && (maze[row+1][col] & DIST) == (maze[row][col] & DIST) - 1) {
		direction = 2;
		++row;
		priority = 0;
	}
	else if ((maze[row][col-1] & VISITED) && (maze[row][col-1] & DIST) == (maze[row][col] & DIST) - 1) {
		direction = 3;
		--col;
		priority = 0;
	}/*}}}*/

	if (priority) {/*{{{*/
		if ((maze[row-1][col] & DIST) == (maze[row][col] & DIST) - 1) {
			direction = 0;
			--row;
		}
		else if ((maze[row][col+1] & DIST) == (maze[row][col] & DIST) - 1) {
			direction = 1;
			++col;
		}
		else if ((maze[row+1][col] & DIST) == (maze[row][col] & DIST) - 1) {
			direction = 2;
			++row;
		}
		else if ((maze[row][col-1] & DIST) == (maze[row][col] & DIST) - 1) {
			direction = 3;
			--col;
		}
	}/*}}}*/

	location = (row << 4) | col;
    //TODO
    actualTurn(tmp, direction);
    forward();
    //TODO
}/*}}}*/

/*****************************************************************************/
// turn():
//		Turn to the cell closest to the center
/*****************************************************************************/
void turn(void)/*{{{*/
{
	uint8_t row = (location & ROW) >> 4;
	uint8_t col = location & COL;
  //TODO
  uint8_t tmp = direction;
  //TODO

	uint8_t min = 255;
	uint16_t tile = maze[row][col];

	// If there is no NORTH wall for this cell
	if (!(tile & NORTH_WALL))/*{{{*/
	{
		if ((maze[row - 1][col] & DIST) < min)
		{
			min = maze[row - 1][col] & DIST;
			direction = 0;
		}
	}/*}}}*/

	// If there is no EAST wall for this cell
	if (!(tile & EAST_WALL))/*{{{*/
	{
		if ((maze[row][col + 1] & DIST) < min)
		{
			min = maze[row][col + 1] & DIST;
			direction = 1;
		}
	}/*}}}*/

	// If there is no SOUTH wall for this cell
	if (!(tile & SOUTH_WALL))/*{{{*/
	{
		if ((maze[row + 1][col] & DIST) < min)
		{
			min = maze[row + 1][col] & DIST;
			direction = 2;
		}
	}/*}}}*/

	// If ther`e is no WEST wall for this cell
	if (!(tile & WEST_WALL))/*{{{*/
	{
		if ((maze[row][col - 1] & DIST) < min)
		{
			min = maze[row][col - 1] & DIST;
			direction = 3;
		}
	}/*}}}*/

	lookAhead();
  //TODO
  actualTurn(tmp, direction);
  //TODO
}/*}}}*/

//TODO
/*****************************************************************************/
// turn():
//		Turn to the cell closest to the center
/*****************************************************************************/
void actualTurn(uint8_t prev, uint8_t next)/*{{{*/
{
    if ((prev == 0 && next == 1) || (prev == 1 && next == 2) || (prev == 2 && next == 3) || (prev == 3 && next == 0)) rightTurn();
    else if ((prev == 1 && next == 0) || (prev == 2 && next == 1) || (prev == 3 && next == 2) || (prev == 0 && next == 3)) leftTurn();
    else fullTurn();
}/*}}}*/
//TODO

/*****************************************************************************/
// update(uint16_t row, uint16_t col):
//
// 		Changes current cell distance and check neighbors if necessary
/*****************************************************************************/
void update(uint16_t row, uint16_t col)/*{{{*/
{
	uint16_t tile = maze[row][col];

    if ((tile & DIST) == 0) return;

	// Minimum open neighbor
	uint8_t min = 255;

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
}/*}}}*/

void print()/*{{{*/
{

}/*}}}*/

int flood_main(void) /*{{{*/
{
	// Initialize maze and mouse location
	uint8_t tmpLoc, tmpDir;
  setup(0xf0, 0, 1);

	// First run: find walls, move to center -----------------------------------
	while (location != 0x77 && location != 0x78 &&
		     location != 0x87 && location != 0x88) {

		update((location & ROW) >> 4, location & COL);
		while (stackptr > 0) {
			--stackptr;
			update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);
		}

    tmpLoc = location;
    tmpDir = direction;
		turn();
		move('m');
	}

  // Going from center->back --------------------------------------------------
	setup(location, direction, 2);

	while (location != 0xf0) {
		update((location & ROW) >> 4, location & COL);
		while (stackptr > 0) {
			--stackptr;
			update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);
		}
		turn();
		move('m');
	}
	print();

	// Floodfill ----------------------------------------------------------------
  setup(tmpLoc, tmpDir, 3);
  location = tmpLoc;
  direction = tmpDir;

  while (location != 0x77 && location != 0x78 &&
         location != 0x87 && location != 0x88)
	{
		update((location & ROW) >> 4, location & COL);
		while (stackptr > 0)
		{
			--stackptr;
			update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);
		}
		turn();
		move('f');
	}
	print();

  // TODO: put this run on different mouse state
  // Using floodfill and wall info, make fastest move to center ---------------
  location = 0xf0;
  direction = 0;

  while (location != 0x77 && location != 0x78 &&
         location != 0x87 && location != 0x88) {
    moveFast();
  }
}/*}}}*/
