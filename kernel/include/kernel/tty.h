#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
	
	void terminal_initialize(void);

	void terminal_putchar(char c);
	
	void terminal_putchar(char c);

	void terminal_write(const char* data, size_t size);

	void terminal_writestring(const char* data);
}
#endif
#endif
