/****************************************************************************/
// 
//
/****************************************************************************/
unsigned short board[16][16];			//This represents the actual board, 
										//mouse does not know this yet.
void setup();

/**
 * Pre-fill cell with values that determine its distance from the center.
 * Initially assumes a wall-less maze.
 **/
unsigned short init(unsigned short row, unsigned short col);

void update(unsigned short row, unsigned short col);

void print();