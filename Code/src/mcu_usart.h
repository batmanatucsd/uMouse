//handle all usart communication

#ifndef _MCU_USART_H_
#define _MCU_USART_H_

#include <stdio.h>
#include <errno.h>

int _write(int file, char *ptr, int len);
//int _read(int file, char *ptr, int len);

void USART_init(void);

#endif // _USART_H_
