#ifndef _USART_H_
#define _USART_H_

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
//#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <errno.h>

int _write(int file, char *ptr, int len);
//int _read(int file, char *ptr, int len);

void USART_Configuration(void);

#endif // USART_H
