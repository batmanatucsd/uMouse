#ifndef _MCU_DELAY_H_
#define _MCU_DELAY_H_

#include <stdint.h>

void delay_init(void);
void delay_ms(uint16_t ms);
void delay_us(uint32_t us);

#endif
