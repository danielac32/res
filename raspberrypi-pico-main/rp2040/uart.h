
#include "rp2040.h"
#include <stdarg.h>

#ifndef UART_H_
#define UART_H_

void uart_init();
uint uart_read(char *buff, uint len);
uint uart_write(char *buff, uint len);
void uart_print(char *s);
void uart_hex(uint x);
 
void
printint(int xx, int base, int sgn);

//void
//printptr(uint64_t x);

// Print to the given CONSOLE. Only understands %d, %x, %p, %s.
void
kvprintf(const char *fmt, va_list ap);
void
kfprintf(const char *fmt, ...);

void
kprintf(const char *fmt, ...);
#endif