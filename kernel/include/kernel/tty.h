#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage */
#endif
void terminal_initialize(void);
#if defined(__cplusplus)
extern "C" /* Use C linkage */
#endif
void terminal_putchar(char c);
#if defined(__cplusplus)
extern "C" /* Use C linkage */
#endif
void terminal_write(const char* data, size_t size);
#if defined(__cplusplus)
extern "C" /* Use C linkage */
#endif
void terminal_writestring(const char* data);

#endif
