#include <stdio.h>
#include "floodfill.h"
#include "testmaze.h"

/*****************************************************************************/
// setup(unsigned char loc, unsigned char dist, unsigned char back):
//      Initialize 16x16 maze using 2D array, assumption that no walls exist
/*****************************************************************************/
void setup(unsigned char loc, unsigned char dist, unsigned char back) 
{
    // If going back to start cell, remember maze state and set start
    // cell as destination with distance 0
    if (back == 'b') {
        maze[15][0] = 0x0700;
        initBack();
        return;
    }

    // Initialize maze
    for (unsigned short row = 0; row < 16; row++) {
        for (unsigned short col = 0; col < 16; col++) {
            maze[row][col] = init(row, col);
        }
    }
    for (unsigned short i = 0; i < 16; i++) {
        maze[0][i] |= NORTH_WALL;
        maze[i][0] |= WEST_WALL;
        maze[i][15] |= EAST_WALL;
        maze[15][i] |= SOUTH_WALL;
    }

    // Initialize mouse, position in bottom left corner, facing up
    location = loc;                 // maze[15,0]
    direction = dist;               // 0x0 = up direction
    maze[15][0] |= EAST_WALL;
    maze[15][1] |= WEST_WALL;
}

/*****************************************************************************/
// init(unsigned short row, unsigned short col):
//      Pre-fill cell with values that determine its distance from the center.
//      Initially assumes a wall-less maze.
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
// initBack():
//      Initialize cell distances when going from center->start cell.
//      Distances should be 0 at start cell, increasing as it moves outward
//          from start cell.
/*****************************************************************************/
void initBack() 
{   
    int r = 0, c = 0;

    for (c = 0; c < 15; c++) {
        for (r = 15; r >=0; r--) {
            maze[r][c] = (15 - c) - r;
        }
    }
}

/*****************************************************************************/
// move():
//      Move to the cell closest to the center
/*****************************************************************************/
void move()
{
    unsigned char row = (location & ROW) >> 4;
    unsigned char col = location & COL;
    if (direction == 0 && !(maze[row][col] & NORTH_WALL)) --row;
    else if (direction == 1 && !(maze[row][col] & EAST_WALL)) ++col;
    else if (direction == 2 && !(maze[row][col] & SOUTH_WALL)) ++row;
    else if (direction == 3 && !(maze[row][col] & WEST_WALL)) --col;
    location = (row << 4) | col;
}

/*****************************************************************************/
// getWalls(unsigned short row, unsigned short col):
//
//      Gets wall information for current cells
/*****************************************************************************/
void getWalls(unsigned short row, unsigned short col) {
    printf("********************************************");
    printf("*** getWalls() ***\n");
    printf("ROW:%d, COL:%d\n\n", row, col);

    if ((testmaze[row][col] & NORTH_WALL)) {
        maze[row][col] |= NORTH_WALL;
        maze[row-1][col] |= SOUTH_WALL;
    }

    // If there is no EAST wall for this cell, and east cell is closer
    // to the center than current min, make it the new 'min'
    if ((testmaze[row][col] & EAST_WALL)) {
        maze[row][col] |= EAST_WALL;
        maze[row][col+1] |= WEST_WALL;
    }

    // If there is no SOUTH wall for this cell, and south cell is closer
    // to the center than current min, make it the new 'min'
    if ((testmaze[row][col] & SOUTH_WALL)) {
        maze[row][col] |= SOUTH_WALL;
        maze[row+1][col] |= NORTH_WALL; 
    }

    // If there is no WEST wall for this cell, and west cell is closer
    // to the center than current min, make it the new 'min'
    if ((testmaze[row][col] & WEST_WALL)) {
        maze[row][col] |= WEST_WALL;
        maze[row][col-1] |= EAST_WALL;
    }
}

/*****************************************************************************/
// update(unsigned short row, unsigned short col):
//
//      Changes current cell distance and check neighbors if necessary
/*****************************************************************************/
void update(unsigned short row, unsigned short col)
{
    printf("********************************************");
    printf("*** update() ***\n");
    printf("ROW:%d, COL:%d\n\n", row, col);

    unsigned short tile = maze[row][col];

    // Minimum open neighbor
    unsigned char min = 255;

    // If there is no NORTH wall for this cell, and the north cell is closer
    // to the center than current min, make it the new 'min'
    if (!(tile & NORTH_WALL)) {
        if ((maze[row - 1][col] & DIST) < min) {
            min = maze[row - 1][col] & DIST;
            direction = 0;
        }
    }

    // If there is no EAST wall for this cell, and east cell is closer
    // to the center than current min, make it the new 'min'
    if (!(tile & EAST_WALL)) {
        if ((maze[row][col + 1] & DIST) < min) {
            min = maze[row][col + 1] & DIST;
            direction = 1;
        }
    }

    // If there is no SOUTH wall for this cell, and south cell is closer
    // to the center than current min, make it the new 'min'
    if (!(tile & SOUTH_WALL)) {
        if ((maze[row + 1][col] & DIST) < min) {
            min = maze[row + 1][col] & DIST;
            direction = 2;
        }
    }

    // If there is no WEST wall for this cell, and west cell is closer
    // to the center than current min, make it the new 'min'
    if (!(tile & WEST_WALL)) {
        if ((maze[row][col - 1] & DIST) < min) {
            min = maze[row][col - 1] & DIST;
            direction = 3;
        }
    }

    // Set cells with 3 walls to DIST = 255
    if ( (tile & WALLS)>>8 == 7 || (tile & WALLS)>>8 == 11 ||
       (tile & WALLS)>>8 == 13 || (tile & WALLS)>>8 == 14 ) {
        if (row != 15 && col != 0) {
            printf("WE NEED TO CHANGE THIS CELL TO 255!!!"); 
            maze[row][col] &= 0xff00;
            maze[row][col] |= 255;   
        }

        // Push open neighbors onto stack
        // if (!(maze[row][col] & NORTH_WALL)) {
        //     stack[stackptr++] = ((row - 1) << 4) | col;
        // }
        // if (!(maze[row][col] & EAST_WALL)) {
        //     stack[stackptr++] = (row << 4) | (col + 1);
        // }
        // if (!(maze[row][col] & SOUTH_WALL)) {
        //     stack[stackptr++] = ((row + 1) << 4) | col;
        // }
        // if (!(maze[row][col] & WEST_WALL)) {
        //     stack[stackptr++] = (row << 4) | (col - 1);
        // }  
    }


    // If no adjacent cell distance < current cell, push neighbors on stack
    if (min + 1 != (tile & DIST)) {
        // Update distance
        maze[row][col] &= 0xff00;
        maze[row][col] |= min + 1;

        // Push open neighbors onto stack
        if (!(maze[row][col] & NORTH_WALL)) {
            stack[stackptr++] = ((row - 1) << 4) | col;
        }
        if (!(maze[row][col] & EAST_WALL)) {
            stack[stackptr++] = (row << 4) | (col + 1);
        }
        if (!(maze[row][col] & SOUTH_WALL)) {
            stack[stackptr++] = ((row + 1) << 4) | col;
        }
        if (!(maze[row][col] & WEST_WALL)) {
            stack[stackptr++] = (row << 4) | (col - 1);
        }
    }
}

/*****************************************************************************/
// print(unsigned short m):
//      Print out the cell, with the mouse location designated with a carrot
//      sign pointing in direction it faces
/*****************************************************************************/
void print() {
    for (unsigned short row = 0; row < 16; row++) {
        // North wall
        for (unsigned short col = 0; col < 16; col++) {
            printf("+%s", maze[row][col] & NORTH_WALL?
                "---": "   ");
        }
        printf("+\n");

        for (unsigned short col = 0; col < 16; col++) {
            printf("%s", maze[row][col] & WEST_WALL?
                "|": " ");

            // Location direction
            if (row == (location & ROW) >> 4 &&
                col == (location & COL)) {
                switch (direction) {
                    case 0x0: printf("^");
                                break;
                    case 0x1: printf(">");
                                break;
                    case 0x2: printf("v");
                                break;
                    case 0x3: printf("<");
                }
            }
            else if (maze[row][col] & VISITED) {
                printf("*");
            }
            else {
                printf(" ");
            }

            // Print 2 digits
            printf("%2d", (maze[row][col] & DIST) % 100);
        }

        printf("%s\n", maze[row][15] & EAST_WALL?
            "|": " ");
    }

    // South wall
    for (unsigned short col = 0; col < 16; col++) {
        printf("+%s", maze[15][col] & SOUTH_WALL?
            "---": "   ");
    }
    printf("+\n");
}

int main() {
    // Initialize maze and mouse location
    char name[99999];
    setup(0xf0, 0, 'f');
    setupTest(0xf0, 0);

    printf("Maze in mouse memory: \n");
    print();

    // While location is not in one of the endpoint cells
    while (location != 0x77 && location != 0x78 &&
        location != 0x87 && location != 0x88)
    {
        printf("Press RETURN to continue");
        fgets(name, sizeof(name), stdin);
        
        // get wall information of current & surrounding cells 
        getWalls((location & ROW) >> 4, location & COL);

        // get distance information of surrounding cells
        update((location & ROW) >> 4, location & COL);

        // if cells on stack, update cell distances
        while (stackptr > 0)
        {
            --stackptr; 

            update((stack[stackptr] & ROW) >> 4, stack[stackptr] & COL);
            /**
            // DEBUG -----------------------------------------------------------
            printf("Current cell: %d,%d\n", (stack[stackptr - 1] & ROW) >> 4, stack[stackptr - 1] & COL);
            printf("Current stack: ");
            for (int i = 0; i < stackptr; i++)
                printf("(%d, %d)", (stack[i] & ROW) >> 4, stack[i] & COL);
            printf("\n");**/
            // DEBUG -----------------------------------------------------------
        }

        // move into open adjacent cell with min distance
        move();
        print(); 
    }
    print();
    printf("OOOOOO YAYY!!!!!!\n");


    // going center -> start cell
    while (stackptr > 0) {
        printf("heh");
    }
}









