#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "string.h"
#if defined(__linux__)
#error "Please set your compiler targets correctly!"
#endif
 
#if !defined(__i386__)
#error "This os only runs in x86 envs!"
#endif

namespace std {
	enum vga_color {
		COLOR_BLACK = 0,
		COLOR_BLUE = 1,
		COLOR_GREEN = 2,
		COLOR_CYAN = 3,
		COLOR_RED = 4,
		COLOR_MAGENTA = 5,
		COLOR_BROWN = 6,
		COLOR_LIGHT_GREY = 7,
		COLOR_DARK_GREY = 8,
		COLOR_LIGHT_BLUE = 9,
		COLOR_LIGHT_GREEN = 10,
		COLOR_LIGHT_CYAN = 11,
		COLOR_LIGHT_RED = 12,
		COLOR_LIGHT_MAGENTA = 13,
		COLOR_LIGHT_BROWN = 14,
		COLOR_WHITE = 15,
	};

	const char* ANSI_BLACK = "\e[0;30m";
	const char* ANSI_BLUE  = "\e[0;34m";
	const char* ANSI_GREEN = "\e[0;32m";
	const char* ANSI_CYAN  = "\e[0;36m";
	const char* ANSI_RED   = "\e[0;31m";
	const char* ANSI_PURPLE = "\e[0;35m";
	const char* ANSI_BROWN  = "\e[0;33m";
	const char* ANSI_GRAY   = "\e[0;37m";
	const char* ANSI_DARK_GRAY = "\e[1;30m";
	const char* ANSI_LIGHT_BLUE = "\e[1;34m";
	const char* ANSI_LIGHT_GREEN ="\e[1;32m";
	const char* ANSI_LIGHT_CYAN = "\e[1;36m";
	const char* ANSI_LIGHT_RED = "\e[1;31m";
	const char* ANSI_LIGHT_MAGENTA = "\e[1;35m";
	const char* ANSI_LIGHT_BROWN = "\e[1;33m";
	const char* ANSI_WHITE = "\e[1;37m";
	
	static uint8_t ansi_to_vga(char* cstr) {
		char* str = const_cast<char *> (cstr);
		if (str == ANSI_BLACK) {
			return COLOR_BLACK;
		} else if (str == ANSI_BLUE) {
			return COLOR_BLUE;
		} else if (str == ANSI_GREEN) {
			return COLOR_GREEN;
		} else if (str == ANSI_CYAN) {
			return COLOR_CYAN;
		} else if (str == ANSI_RED) {
			return COLOR_RED;
		} else if (str == ANSI_PURPLE) {
			return COLOR_MAGENTA;
		} else if (str == ANSI_BROWN) {
			return COLOR_BROWN;
		} else if (str == ANSI_GRAY) {
			return COLOR_LIGHT_GREY;
		} else if (str == ANSI_DARK_GRAY) {
			return COLOR_DARK_GREY;
		} else if (str == ANSI_LIGHT_BLUE) {
			return COLOR_LIGHT_BLUE;
		} else if (str == ANSI_LIGHT_GREEN) {
			return COLOR_LIGHT_GREEN;
		} else if (str == ANSI_LIGHT_CYAN) {
			return COLOR_LIGHT_CYAN;
		} else if (str == ANSI_LIGHT_RED) {
			return COLOR_LIGHT_RED;
		} else if (str == ANSI_LIGHT_MAGENTA) {
			return COLOR_LIGHT_MAGENTA;
		} else if (str == ANSI_LIGHT_BROWN) {
			return COLOR_LIGHT_BROWN;
		} else if (str == ANSI_WHITE) {
			return COLOR_WHITE;
		} else return 16;
	}
	
	uint8_t make_color(enum vga_color fg, enum vga_color bg) {
		return fg | bg << 4;
	}

	uint16_t make_vgaentry(char c, uint8_t color) {
		uint16_t c16 = c;
		uint16_t color16 = color;
		return c16 | color16 << 8;
	}

	size_t strlen(const char* str) {
		size_t ret = 0;
		while ( str[ret] != 0 )
				ret++;
		return ret;
	}
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(std::COLOR_LIGHT_GREY, std::COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = std::make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
		terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
		const size_t index = y * VGA_WIDTH + x;
		terminal_buffer[index] = std::make_vgaentry(c, color);
}

void terminal_putchar(char c, uint8_t color = terminal_color) {
	if (c == '\n') {
		terminal_row++;
		terminal_column = 0;
		return;
	}
	terminal_putentryat(c, color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
			terminal_row = 0;
		}
	}
}
 
void terminal_writestring(const char* data) {
	size_t datalen = std::strlen(data);
	char temp[100];
	uint8_t color = terminal_color;
	for (size_t i = 0; i < datalen; i++) {
		std::strcat(temp, const_cast<char*>(&data[i]));
		if (std::ansi_to_vga(temp) != 16) {
			color = std::ansi_to_vga(temp);
		}	
		terminal_putchar(data[i], color);
	}
}
