#ifndef _FLOODFILL_H_
#define _FLOODFILL_H_

#include "stm32f10x.h"


/*****************************************************************************/
// Globals
/*****************************************************************************/
uint16_t maze[16][16];   // Initially empty board in mouse memory
uint8_t location = 0x0;  // First four bits = ROW, last four = COL
uint8_t direction = 0x0; // 0x0 north 0x1 east 0x2 south 0x3 west

/*****************************************************************************/
// Bit masks
// xxxV_NESW_DIST
/*****************************************************************************/
uint16_t VISITED = 0x1000;
uint16_t NORTH_WALL = 0x0800;
uint16_t EAST_WALL = 0x0400;
uint16_t SOUTH_WALL = 0x0200;
uint16_t WEST_WALL = 0x0100;
uint16_t WALLS = 0x0f00;
uint16_t DIST = 0xff;
uint8_t ROW = 0xf0;
uint8_t COL = 0x0f;

/*****************************************************************************/
// Neighbor stack
/*****************************************************************************/

uint8_t stack[512];
uint16_t stackptr = 0;

/*****************************************************************************/
// Functions
/*****************************************************************************/
void setup(uint8_t loc, uint8_t dist, uint8_t back);
uint16_t init(uint16_t row, uint16_t col);
uint16_t initBack(uint16_t row, uint16_t col);
uint16_t initFlood(uint16_t row, uint16_t col);
void lookAhead();
void turn();
void actualTurn(uint8_t prev, uint8_t next);
void move(uint8_t flood);
void moveFast();
void update(uint16_t row, uint16_t col);

#endif
