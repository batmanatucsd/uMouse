#ifndef USART_H
#define USART_H

/*
 * newlib_stubs.c
 *
 *  Created on: 2 Nov 2010
 *      Author: nanoage.co.uk
 */
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include "stm32f10x_usart.h"

#define USART_BAUDRATE 9600

#undef errno
extern int errno;

/*****************************************************************************/
// For USART
/*****************************************************************************/
void USART_Configuration(void);
void USART_Write(uint16_t);
uint8_t USART_Read(void);
void USART_SendInt(uint16_t);

/*****************************************************************************/
// Stub Functions
/*****************************************************************************/
int _read(int, char *, int);
int _write(int, char *, int);
int _close(int);

caddr_t _sbrk(int);
int _lseek(int, int, int);
int _isatty(int);
int _fstat(int, struct stat *);

#endif // USART_H
