#include "mcu_delay.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>

static uint8_t  factor_us = 0;
static uint16_t factor_ms = 0;

void delay_init(void)
{
    //set systick to AHB/8 = 72000000/8 = 9000000
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	factor_us = (uint8_t)(rcc_ahb_frequency / (8*1000000));
	factor_ms = (uint16_t)factor_us*1000;
}

//  note the range for us
//  systick -> load is 24-bit reg
//  so max delay = 0xffffff * 1000000 / factor_us (in us)
//  for 72Mhz, max delay = 1864135 us
void delay_us(uint32_t us)
{
    systick_set_reload(us*factor_us);// load time
	systick_clear();               // clear counter
	systick_counter_enable();      // enable counter
	while(!systick_get_countflag());   // wait until counter flag set
	systick_counter_disable();     // disable counter
	systick_clear();               // clear counter
}

//  note the range for ms
//  systick -> load is 24-bit reg
//  so max delay = 0xffffff * 1000 / factor_us (in ms)
//  for 72Mhz, max delay = 1864 ms
void delay_ms(uint16_t ms)
{
    systick_set_reload(ms*factor_ms);// load time
    systick_clear();               // clear counter
    systick_counter_enable();      // enable counter
    while(!systick_get_countflag());   // wait until counter flag set
    systick_counter_disable();     // disable counter
    systick_clear();               // clear counter
}
