unsigned short board[16][16];

// Bit masks
unsigned short VISITED = 0x1000;
unsigned short NORTH_WALL = 0x0800;
unsigned short EAST_WALL = 0x0400;
unsigned short SOUTH_WALL = 0x0200;
unsigned short WEST_WALL = 0x0100;
unsigned short VALUE = 0xff;

// DFS stack
unsigned short stack[512];
unsigned short stackptr = 0;

void setup();

unsigned short init(unsigned short row, unsigned short col);

void update(unsigned short row, unsigned short col);

void print();