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
	char line[15];
    unsigned short value;
    int row = 0;
    int col = 0;

	//Read til the end of file
	while(!feof(file)) {
		fgets(line, 15, file);

        //If the value is not "/" or whitespace, grab value to fill testmaze
        if (line[0] != '/' && line[0] != ' ') 
        {
			//Convert the value in file into an int
			printf("\nraw line value: %s", line);
			value = atoi(line);
			
			//int value should be = 0000 [RAWLINE] 0000 0000
			value = value << 8;
			printf("int value: %d\n", value);

			row++;

			//Move onto next column after 15 rows have been filled
			if (row == 15) 
			{
				col++;
				row = 0;
			}

			//Create testMaze by OR'ing the values inputted in txt file
			testMaze [row][col] |= value;

			//Reset line value
			memset(line, 0, sizeof(line));
		}
	}
	printf("Testing testmaze access: %u\n",testMaze[0][0]);

}


