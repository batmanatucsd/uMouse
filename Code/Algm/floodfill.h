unsigned short board[16][16];
unsigned char location = 0x0;
unsigned char direction = 0x0; // 0x0 north 0x1 east 0x2 south 0x3 west

// Bit masks
unsigned short VISITED = 0x1000;
unsigned short NORTH_WALL = 0x0800;
unsigned short EAST_WALL = 0x0400;
unsigned short SOUTH_WALL = 0x0200;
unsigned short WEST_WALL = 0x0100;
unsigned short DIST = 0xff;
unsigned char ROW = 0xf0;
unsigned char COL = 0x0f;

// DFS stack
unsigned char stack[512];
unsigned short stackptr = 0;

void setup();

unsigned short init(unsigned short row, unsigned short col);

void update(unsigned short row, unsigned short col);

void print();