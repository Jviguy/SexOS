#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>
#include <sys/io.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

#define VGA_GFX_REG            0x3CE
#define VGA_SEQ_REG            0x3C4
#define VGA_CRT_REG            0x3D4

#define VGA_GFX_I_RESET         0x00
#define VGA_GFX_I_ENABLE      0x01
#define VGA_GFX_I_COLORCMP      0x02
#define VGA_GFX_I_ROTATE      0x03
#define VGA_GFX_I_READMAP      0x04
#define VGA_GFX_I_MODE         0x05
#define VGA_GFX_I_MISC         0x06
#define VGA_GFX_I_CNOCARE      0x07
#define VGA_GFX_I_BITMASK      0x08

#define VGA_SEQ_I_RESET         0x00
#define VGA_SEQ_I_CLOCK         0x01
#define VGA_SEQ_I_MAPMASK      0x02
#define VGA_SEQ_I_CHARMAP      0x03
#define VGA_SEQ_I_MEMMODE      0x04

unsigned char readRegVGA(unsigned short reg, unsigned char idx)
{
	outb(reg, idx);
	return inb(reg + 0x01);
}

void writeRegVGA(unsigned short reg, unsigned char idx, unsigned char val)
{
	outb(reg, idx);
	outb(reg + 1, val);
}

void setFontVGA(const unsigned char * buffer, int h)
{
	unsigned char seq2, seq4, gfx6;
     	int i, j;
	unsigned char * mem;

	seq2 = readRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MAPMASK);
	writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MAPMASK, 0x04);


	seq4 = readRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MEMMODE);
     	writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MEMMODE, 0x06);

	writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_CHARMAP, 0x00);


	gfx6 = readRegVGA(VGA_GFX_REG, VGA_GFX_I_MISC);
   	writeRegVGA(VGA_SEQ_REG, VGA_GFX_I_MISC, 0x00);

   	mem = (unsigned char *) 0xB0000;

   	for (i = 0; i < 256; i++) {
      		for (j = 0; j < h; j++)
         		mem[j] = buffer[h * i + j];
      			mem += 32;
   	}

   writeRegVGA(VGA_GFX_REG, VGA_GFX_I_MISC, gfx6);
   writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MEMMODE, seq4);
   writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MAPMASK, seq2);

   writeRegVGA(VGA_GFX_REG, VGA_GFX_I_MODE, 0x10);
   writeRegVGA(VGA_GFX_REG, VGA_GFX_I_BITMASK, 0xFF);
}	

#endif
