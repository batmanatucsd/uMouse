/****************************************************************************/
// testmaze.h
//		This actually 
//
/****************************************************************************/

unsigned short testMaze[16][16];		//This represents the actual board, 
										//mouse does not know this yet.

/****************************************************************************/
// setupTest():
//		This actually just hardcodes the maze... lmao
/****************************************************************************/
void setupTest() {

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
	// *** THIS WILL ALWAYS BE THE SAME ***
	board[15][0] = board[15][0] | WEST_WALL;




}