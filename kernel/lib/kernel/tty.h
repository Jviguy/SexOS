#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
 
#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_putchar(char c, uint8_t color);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
 
#endif
