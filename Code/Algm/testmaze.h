/****************************************************************************/
// testMaze.h
//		This actually 
//
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned short testMaze[16][16];		//This represents the actual testMaze, 
										//mouse does not know this yet.

/****************************************************************************/
// setupTest():
//		This actually just hardcodes the maze... lmao
//		Moves through columns from 0->15, left to right. 
//		Then for each column, rows 0->15, top to bottom. 
/****************************************************************************/
void setupTest() {

	//Initialize edge cells -------------------------------------------------
	for (unsigned short row = 0; row < 16; row++) {
		for (unsigned short col = 0; col < 16; col++) {
			//Pre-fills cells with distance from center, assuming no walls
			if (row == 0) testMaze[row][col] |= NORTH_WALL;
			if (col == 0) testMaze[row][col] |= WEST_WALL;
			if (col == 15) testMaze[row][col] |= EAST_WALL;
			if (row == 15) testMaze[row][col] |= SOUTH_WALL;
		}
	}

	//Initialize mouse, position in bottom left corner, facing up
	location = 0xf0;					// testMaze[15,0]
	direction = 0x0;					// 0x0 = up direction

	//Initialize the right wall for the bottom left corner, always
	testMaze[15][0] |= EAST_WALL;

	//Open text file that represents a maze to read walls in, read-only
	FILE *file = fopen("testmaze2.txt", "r");
	if (!file) {
		printf("Sorry, that test maze file does not exist.\n");
		exit(1);
	}

	//Read the text file to create maze
	//int i = 0;
	char * line = NULL;
	unsigned short value;
	int row = 0;
	int col = 0;
	size_t len = 0;
	ssize_t read;

	//Read til the end of file
	while(!feof(file)) {
		//Skip lines that denote column number.
		if ((read = getline(&line, &len, file)) != -1 && line[0] != '/') {
			
			//Convert the value in file into an int
			printf("\nraw line value: %s", line);
			value = atoi(line);
			
			//int value should be = 0000 [RAWLINE] 0000 0000
			value = value << 8;
			printf("int value: %d\n", value);

			row++;
			//Move onto next column after 15 rows have been filled
			if (row == 15) {
				col++;
				row = 0;
			}

			//Create testMaze by OR'ing the values inputted in txt file
			testMaze [row][col] |= value;

			//Reset line value
			memset(line, 0, sizeof(*line));
		}
	}
	printf("Testing testmaze access: %u\n",testMaze[0][0]);

}


	//Loop through text file to create the maze.
	/**unsigned short c = 0;
	for (int row = 0; row < 16; row++) {
		for (int col = 0; col < 16; col++) {
			testMaze[row][col] = c;
		}
	} **/


/**	//Column 0 WALLS:
	testMaze[14][0] |= 
=======
	// Initialize mouse, position in bottom left corner, facing up
	location = 0xf0;				// testMaze[15,0]
	direction = 0x0;				// 0x0 = up direction


	// Initialize the right wall for the bottom left corner 
	// *** THIS WILL ALWAYS BE THE SAME ***
	testMaze[15][0] |= EAST_WALL;

	//Column 0 WALLS:
	//testMaze[14][0] |= 
>>>>>>> 2e2c19e543b5718f4f7a4a934154fa1f9896301a
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

<<<<<<< HEAD
	//Column 1 WALLS: --------------------------------------------------------
=======
	//Column 1 WALLS:
>>>>>>> 2e2c19e543b5718f4f7a4a934154fa1f9896301a
	testMaze[15][1] |= WEST_WALL;
	testMaze[14][1] |= EAST_WALL;
	testMaze[13][1] |= WEST_WALL;
	testMaze[12][1] |= (NORTH_WALL | WEST_WALL);
	testMaze[11][1] |= (SOUTH_WALL | NORTH_WALL);
	testMaze[10][1] |= (SOUTH_WALL | NORTH_WALL);
	testMaze[9][1] |= (SOUTH_WALL | NORTH_WALL);
	testMaze[8][1] |= (SOUTH_WALL | NORTH_WALL);
<<<<<<< HEAD
	testMaze[7][1] |= (SOUTH_WALL | EAST_WALL);
	testMaze[6][1] |= (NORTH_WALL | WEST_WALL);
	testMaze[5][1] |= (SOUTH_WALL | EAST_WALL);
	testMaze[4][1] |= (NORTH_WALL | WEST_WALL);
	testMaze[3][1] |= (SOUTH_WALL | EAST_WALL);
	testMaze[2][1] |= (NORTH_WALL | WEST_WALL);
	testMaze[1][1] |= (NORTH_WALL | EAST_WALL | SOUTH_WALL | WEST_WALL); 
	testMaze[0][1] |= SOUTH_WALL;

	//Column 2 WALLS: --------------------------------------------------------
	testMaze[15][2] |= NORTH_WALL;
	testMaze[14][2] |= (NORTH_WALL | SOUTH_WALL | WEST_WALL);
	testMaze[13][2] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[12][2] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[11][2] |= SOUTH_WALL;
	testMaze[10][2] |= NORTH_WALL;
	testMaze[9][2] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[8][2] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[7][2] |= (NORTH_WALL | EAST_WALL | SOUTH_WALL | WEST_WALL); 
	testMaze[6][2] |= (EAST_WALL | SOUTH_WALL);
	testMaze[5][2] |= (NORTH_WALL | WEST_WALL);
	testMaze[4][2] |= (EAST_WALL | SOUTH_WALL);
	testMaze[3][2] |= (NORTH_WALL | WEST_WALL);
	testMaze[2][2] |= (EAST_WALL | SOUTH_WALL);
	testMaze[1][2] |= (NORTH_WALL | WEST_WALL);
	testMaze[0][2] |= SOUTH_WALL;

	//Column 3 WALLS: --------------------------------------------------------
	testMaze[15][3] |= NORTH_WALL;
	testMaze[14][3] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[13][3] |= SOUTH_WALL;
	testMaze[12][3] |= (NORTH_WALL | EAST_WALL);
	testMaze[11][3] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[10][3] |= SOUTH_WALL;
	testMaze[9][3] |= NORTH_WALL;
	testMaze[8][3] |= (EAST_WALL | SOUTH_WALL);
	testMaze[7][3] |= (EAST_WALL | WEST_WALL);
	testMaze[6][3] |= (NORTH_WALL | WEST_WALL);
	testMaze[5][3] |= (EAST_WALL | SOUTH_WALL);
	testMaze[4][3] |= (NORTH_WALL | WEST_WALL);
	testMaze[3][3] |= (EAST_WALL | SOUTH_WALL);
	testMaze[2][3] |= (NORTH_WALL | WEST_WALL);
	testMaze[1][3] |= (EAST_WALL | SOUTH_WALL);
	//testMaze[0][3] |=

	//Column 4 WALLS: --------------------------------------------------------
	testMaze[15][4] |= NORTH_WALL;
	testMaze[14][4] |= SOUTH_WALL;
	testMaze[13][4] |= EAST_WALL;
	testMaze[12][4] |= WEST_WALL;
	testMaze[11][4] |= NORTH_WALL;
	testMaze[10][4] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[9][4] |= SOUTH_WALL;
	testMaze[8][4] |= (EAST_WALL | WEST_WALL);
	testMaze[7][4] |= (NORTH_WALL | WEST_WALL);
	testMaze[6][4] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[5][4] |= (NORTH_WALL | EAST_WALL | SOUTH_WALL | WEST_WALL); 
	testMaze[4][4] |= (EAST_WALL | SOUTH_WALL);
	testMaze[3][4] |= (NORTH_WALL | WEST_WALL);
	testMaze[2][4] |= (EAST_WALL | SOUTH_WALL);
	testMaze[1][4] |= (NORTH_WALL | WEST_WALL);
	testMaze[0][4] |= SOUTH_WALL;

	//Column 5 WALLS: --------------------------------------------------------
	testMaze[15][5] |= NORTH_WALL;
	testMaze[14][5] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[13][5] |= (NORTH_WALL | EAST_WALL | SOUTH_WALL | WEST_WALL);
	testMaze[12][5] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[11][5] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[10][5] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[9][5] |= SOUTH_WALL;
	testMaze[8][5] |= (EAST_WALL | WEST_WALL);
	testMaze[7][5] |= (NORTH_WALL | WEST_WALL);
	testMaze[6][5] |= (EAST_WALL | SOUTH_WALL);
	testMaze[5][5] |= (EAST_WALL | WEST_WALL);
	testMaze[4][5] |= (NORTH_WALL | WEST_WALL);
	testMaze[3][5] |= (NORTH_WALL | SOUTH_WALL);
	testMaze[2][5] |= (NORTH_WALL | SOUTH_WALL | WEST_WALL);
	testMaze[1][5] |= (EAST_WALL | SOUTH_WALL);
	//testMaze[0][5] |=

	//Column 6 WALLS: --------------------------------------------------------
	testMaze[15][6] |= NORTH_WALL;
	testMaze[14][6] |= SOUTH_WALL;
	testMaze[13][6] |= EAST_WALL | WEST_WALL;
	testMaze[12][6] |= NORTH_WALL;
	testMaze[11][6] |= EAST_WALL | SOUTH_WALL;
	testMaze[10][6] |= NORTH_WALL;
	testMaze[9][6] |= NORTH_WALL | SOUTH_WALL;
	testMaze[8][6] |= SOUTH_WALL | WEST_WALL;
	testMaze[7][6] |= EAST_WALL | WEST_WALL;
	testMaze[6][6] |= WEST_WALL;
	testMaze[5][6] |= EAST_WALL | WEST_WALL;
	testMaze[4][6] |= EAST_WALL;
	testMaze[3][6] |= NORTH_WALL | EAST_WALL;
	testMaze[2][6] |= SOUTH_WALL;
	testMaze[1][6] |= (EAST_WALL | WEST_WALL);
	//testMaze[0][6] |=

	//Column 7 WALLS:
	testMaze[15][7] |= NORTH_WALL;
	testMaze[14][7] |= NORTH_WALL | SOUTH_WALL;
	testMaze[13][7] |=
	testMaze[12][7] |=
	testMaze[11][7] |=
	testMaze[10][7] |=
	testMaze[9][7] |=
	testMaze[8][7] |=
	testMaze[7][7] |=
	testMaze[6][7] |=
	testMaze[5][7] |=
	testMaze[4][7] |=
	testMaze[3][7] |=
	testMaze[2][7] |=
	testMaze[1][7] |=
	testMaze[0][7] |= SOUTH_WALL;

	//Column 8 WALLS: --------------------------------------------------------
	testMaze[15][8] |= NORTH_WALL;
	testMaze[14][8] |= NORTH_WALL | SOUTH_WALL;
	testMaze[13][8] |=
	testMaze[12][8] |=
	testMaze[11][8] |=
	testMaze[10][8] |=
	testMaze[9][8] |=
	testMaze[8][8] |=
	testMaze[7][8] |=
	testMaze[6][8] |=
	testMaze[5][8] |=
	testMaze[4][8] |=
	testMaze[3][8] |=
	testMaze[2][8] |=
	testMaze[1][8] |=
	//testMaze[0][8] |=

	//Column 9 WALLS: --------------------------------------------------------
	testMaze[15][9] |= NORTH_WALL;
	testMaze[14][9] |= NORTH_WALL | SOUTH_WALL;
	testMaze[13][9] |=
	testMaze[12][9] |=
	testMaze[11][9] |=
	testMaze[10][9] |=
	testMaze[9][9] |=
	testMaze[8][9] |=
	testMaze[7][9] |=
	testMaze[6][9] |=
	testMaze[5][9] |=
	testMaze[4][9] |=
	testMaze[3][9] |=
	testMaze[2][9] |=
	testMaze[1][9] |=
	testMaze[0][9] |= SOUTH_WALL;

	//Column 10 WALLS: --------------------------------------------------------
	testMaze[15][10] |= NORTH_WALL;
	testMaze[14][10] |= NORTH_WALL | SOUTH_WALL;
	testMaze[13][10] |=
	testMaze[12][10] |=
	testMaze[11][10] |=
	testMaze[10][10] |=
	testMaze[9][10] |=
	testMaze[8][10] |=
	testMaze[7][10] |=
	testMaze[6][10] |=
	testMaze[5][10] |=
	testMaze[4][10] |=
	testMaze[3][10] |=
	testMaze[2][10] |=
	testMaze[1][10] |=
	testMaze[0][10] |= SOUTH_WALL;

	//Column 11 WALLS: --------------------------------------------------------
	testMaze[15][11] |= NORTH_WALL;
	testMaze[14][11] |= NORTH_WALL | SOUTH_WALL;
	testMaze[13][11] |=
	testMaze[12][11] |=
	testMaze[11][11] |=
	testMaze[10][11] |=
	testMaze[9][11] |=
	testMaze[8][11] |=
	testMaze[7][11] |=
	testMaze[6][11] |=
	testMaze[5][11] |=
	testMaze[4][11] |=
	testMaze[3][11] |=
	testMaze[2][11] |=
	testMaze[1][11] |=

	//Column 12 WALLS: --------------------------------------------------------
	testMaze[15][11] |= NORTH_WALL;
	testMaze[14][11] |= NORTH_WALL | SOUTH_WALL;
	testMaze[13][11] |=
	testMaze[12][11] |=
	testMaze[11][11] |=
	testMaze[10][11] |=
	testMaze[9][11] |=
	testMaze[8][11] |=
	testMaze[7][11] |=
	testMaze[6][11] |=
	testMaze[5][11] |=
	testMaze[4][11] |=
	testMaze[3][11] |=
	testMaze[2][11] |=
	testMaze[1][11] |= 



} 

	//testMaze[7][0] |= 
	testMaze[6][1] |= EAST_WALL;
	//testMaze[5][0] |= 
	testMaze[4][1] |= EAST_WALL;
	//testMaze[3][0] |= 
	testMaze[2][1] |= EAST_WALL;
	testMaze[1][1] |= EAST_WALL; 
	//testMaze[0][0] |=**/






