/*****************************************************************************/
// Globals
/*****************************************************************************/
unsigned short maze[16][16]; // Initially empty board in mouse memory
unsigned short current = 0x0;  // Cell on stack mouse is looking at
unsigned char location = 0x0;  // First four bits = ROW, last four = COL
unsigned char direction = 0x0; // 0x0 north 0x1 east 0x2 south 0x3 west

/*****************************************************************************/
// Bit masks
/*****************************************************************************/
unsigned short VISITED = 0x1000;
unsigned short NORTH_WALL = 0x0800;
unsigned short EAST_WALL = 0x0400;
unsigned short SOUTH_WALL = 0x0200;
unsigned short WEST_WALL = 0x0100;
unsigned short DIST = 0xff;
unsigned char ROW = 0xf0;
unsigned char COL = 0x0f;

/*****************************************************************************/
// Neighbor stack
/*****************************************************************************/

unsigned char stack[512];
unsigned short stackptr = 0;

/*****************************************************************************/
// Functions
/*****************************************************************************/
void setup();
unsigned short init(unsigned short row, unsigned short col);
void lookAhead();
void move();
void update(unsigned short row, unsigned short col);
void print();s
