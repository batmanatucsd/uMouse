/****************************************************************************/
// testMaze.h
//		This actually 
//
/****************************************************************************/

unsigned short testMaze[16][16];		//This represents the actual testMaze, 
										//mouse does not know this yet.

/****************************************************************************/
// setupTest():
//		This actually just hardcodes the maze... lmao
//		Order of set up will start from bottom left, then going up the row,
//		then to next column, and going up the row [15 -> 0]
//		column order will be [0 -> 15]
/****************************************************************************/
void setupTest() {

	// Initialize, empty 
	for (unsigned short row = 0; row < 16; row++) {
		for (unsigned short col = 0; col < 16; col++) {
			if (row == 0) testMaze[row][col] |= NORTH_WALL;
			if (col == 0) testMaze[row][col] |= WEST_WALL;
			if (col == 15) testMaze[row][col] |= EAST_WALL;
			if (row == 15) testMaze[row][col] |= SOUTH_WALL;
		}
	}

	// Initialize mouse, position in bottom left corner, facing up
	location = 0xf0;				// testMaze[15,0]
	direction = 0x0;				// 0x0 = up direction


	// Initialize the right wall for the bottom left corner 
	// *** THIS WILL ALWAYS BE THE SAME ***
	testMaze[15][0] |= EAST_WALL;

	//Column 0 WALLS:
	//testMaze[14][0] |= 
	testMaze[13][0] |= EAST_WALL;
	testMaze[12][0] |= EAST_WALL;
	testMaze[11][0] |= NORTH_WALL;
	testMaze[10][0] |= (SOUTH_WALL | NORTH_WALL);
	testMaze[9][0] |= (SOUTH_WALL | NORTH_WALL);
	testMaze[8][0] |= SOUTH_WALL;
	//testMaze[7][0] |= 
	testMaze[6][0] |= EAST_WALL;
	//testMaze[5][0] |= 
	testMaze[4][0] |= EAST_WALL;
	//testMaze[3][0] |= 
	testMaze[2][0] |= EAST_WALL;
	testMaze[1][0] |= EAST_WALL; 
	//testMaze[0][0] |=

	//Column 1 WALLS:
	testMaze[15][1] |= WEST_WALL;
	testMaze[14][1] |= EAST_WALL;
	testMaze[13][1] |= WEST_WALL;
	testMaze[12][1] |= (NORTH_WALL | WEST_WALL);
	testMaze[11][1] |= (SOUTH_WALL | NORTH_WALL);
	testMaze[10][1] |= (SOUTH_WALL | NORTH_WALL);
	testMaze[9][1] |= (SOUTH_WALL | NORTH_WALL);
	testMaze[8][1] |= (SOUTH_WALL | NORTH_WALL);
	//testMaze[7][0] |= 
	testMaze[6][1] |= EAST_WALL;
	//testMaze[5][0] |= 
	testMaze[4][1] |= EAST_WALL;
	//testMaze[3][0] |= 
	testMaze[2][1] |= EAST_WALL;
	testMaze[1][1] |= EAST_WALL; 
	//testMaze[0][0] |=




}
