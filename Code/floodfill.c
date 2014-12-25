#include "floodfill.h"

void init() {
	for (unsigned short row = 0; row < 16; row++) {
		for (unsigned short col = 0; col < 16; col++) {
			board[row][col] = calc(row, col);
		}
	}
}

unsigned short calc(unsigned short row1, unsigned short col1) {
	
	if(row1>0x08) {
		nr = row1 – 0x09;
		row1 = 0x08 – nr;
	}
	
	if(col1>0x08) {
		nc = col1 – 0x09;
		col1 = 0x08 – nc;
	}

	return(0x0f – (col1-0x01) – (row1 – 0x01));
}