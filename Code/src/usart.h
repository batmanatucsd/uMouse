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

#undef errno
extern int errno;

/*
 read
 Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
 Returns -1 on error or blocks until the number of characters have been read.
 */


int _read(int file, char *ptr, int len) {
    int n;
    int num = 0;
    switch (file) {
    case STDIN_FILENO:
        for (n = 0; n < len; n++) {

// #if   STDIN_USART == 1
//             while ((USART1->SR & USART_FLAG_RXNE) == (uint16_t)RESET) {}
//             char c = (char)(USART1->DR & (uint16_t)0x01FF);
// #elif STDIN_USART == 2
//             while ((USART2->SR & USART_FLAG_RXNE) == (uint16_t) RESET) {}
//             char c = (char) (USART2->DR & (uint16_t) 0x01FF);
// #elif STDIN_USART == 3
//             while ((USART3->SR & USART_FLAG_RXNE) == (uint16_t)RESET) {}
//             char c = (char)(USART3->DR & (uint16_t)0x01FF);
// #endif
            char c = USART_Read();
            *ptr++ = c;
            num++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}

/*
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */
int _write(int file, char *ptr, int len) {
    int n;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++) {
            USART_Write(*ptr++ & (uint16_t) 0x01FF);
// #if STDERR_USART == 1
//             while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
//             USART1->DR = (*ptr++ & (uint16_t)0x01FF);
// #elif  STDERR_USART == 2
//             while ((USART2->SR & USART_FLAG_TC) == (uint16_t) RESET) {
//             }
//             USART2->DR = (*ptr++ & (uint16_t) 0x01FF);
// #elif  STDERR_USART == 3
//             while ((USART3->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
//             USART3->DR = (*ptr++ & (uint16_t)0x01FF);
// #endif

        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}


int _close(int file) {
    return -1;
}

/*
 sbrk
 Increase program data space.
 Malloc and related functions depend on this
 */
caddr_t _sbrk(int incr) {

    extern char _ebss; // Defined by the linker
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_ebss;
    }
    prev_heap_end = heap_end;

char * stack = (char*) __get_MSP();
     if (heap_end + incr >  stack)
     {
         _write (STDERR_FILENO, "Heap and stack collision\n", 25);
         errno = ENOMEM;
         return  (caddr_t) -1;
         //abort ();
     }

    heap_end += incr;
    return (caddr_t) prev_heap_end;

}



/*
 lseek
 Set position in a file. Minimal implementation:
 */
int _lseek(int file, int ptr, int dir) {
    return 0;
}



/*
 isatty
 Query whether output stream is a terminal. For consistency with the other minimal implementations,
 */
int _isatty(int file) {
    switch (file){
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        //errno = ENOTTY;
        errno = EBADF;
        return 0;
    }
}

/*
 fstat
 Status of an open file. For consistency with other minimal implementations in these examples,
 all files are regarded as character special devices.
 The `sys/stat.h' header file required is distributed in the `include' subdirectory for this C library.
 */
int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}



