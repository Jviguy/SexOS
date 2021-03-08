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

#define VGA_SEQ_INDEX_PORT 0x3C4
#define VGA_SEQ_DATA_PORT 0x3C5

#define VGA_GC_INDEX_PORT 0x3CE
#define VGA_GC_DATA_PORT 0x3CF

#define VGA_CRTC_INDEX_PORT 0x3D4
#define VGA_CRTC_DATA_PORT 0x3D5

#define VGA_SEQ_MAP_MASK_REG 0x02
#define VGA_SEQ_CHARSET_REG 0x03
#define VGA_SEQ_MEMORY_MODE_REG 0x04

#define VGA_GC_READ_MAP_SELECT_REG 0x04
#define VGA_GC_GRAPHICS_MODE_REG 0x05
#define VGA_GC_MISC_REG 0x06

#define BYTES_PER_GLYPTH 16
#define BYTES_SKIP 16
#define CHARSET_LENGTH 256
#define FONT_SIZE CHARSET_LENGTH * BYTES_PER_GLYPTH

/* @iport - index port, @iport + 1 - data port */
static inline void vga_write_reg(uint16_t iport, uint8_t reg, uint8_t val){
	outb(iport, reg); /* Select register */
	outb(iport + 1, val); /* Do writing through data port */
}

static inline uint8_t vga_read_reg(uint16_t iport, uint8_t reg){
	outb(iport, reg); /* Select register */
	return inb(iport + 1); /* Do reading through data port */
}

static void set_font(unsigned char font[FONT_SIZE]){
   int i, j;
   unsigned char *p = (unsigned char *)0xB8000;
   uint8_t mem_mode, graphics_mode;
   
   /* Panel 2 write enable */
   vga_write_reg(VGA_SEQ_INDEX_PORT, VGA_SEQ_MAP_MASK_REG, 0x04);
   
   /* To be shure, that the first font in the plane 2 is selected */
   vga_write_reg(VGA_SEQ_INDEX_PORT, VGA_SEQ_CHARSET_REG, 0x00);
   
   mem_mode = vga_read_reg(VGA_SEQ_INDEX_PORT, VGA_SEQ_MEMORY_MODE_REG);
   vga_write_reg(VGA_SEQ_INDEX_PORT, VGA_SEQ_MEMORY_MODE_REG, 0x06);
   
        /* I think this line is unnecessary */
   vga_write_reg(VGA_GC_INDEX_PORT, VGA_GC_READ_MAP_SELECT_REG, 0x02);
   
   graphics_mode = vga_read_reg(VGA_GC_INDEX_PORT, VGA_GC_GRAPHICS_MODE_REG);
   vga_write_reg(VGA_GC_INDEX_PORT, VGA_GC_GRAPHICS_MODE_REG, 0x00);
   
        /* And this line is unnecessary too, since I now, that address is 0xB8000 */
   vga_write_reg(VGA_GC_INDEX_PORT, VGA_GC_MISC_REG, 0x0C);
   
   /* Write charmap */
   for(i = 0; i < CHARSET_LENGTH; i++){
      for(j = 0; j < BYTES_PER_GLYPTH; j++){
         *p = *font;
         ++p;
         ++font;
      }
      p += BYTES_SKIP;
   }
   
   /* Restore VGA to normal operation */
   
   /* Panels 0 and 1 write enable */
   vga_write_reg(VGA_SEQ_INDEX_PORT, VGA_SEQ_MAP_MASK_REG, 0x03);
   vga_write_reg(VGA_SEQ_INDEX_PORT, VGA_SEQ_MEMORY_MODE_REG, mem_mode);
   
   vga_write_reg(VGA_GC_INDEX_PORT, VGA_GC_READ_MAP_SELECT_REG, 0x00);
   vga_write_reg(VGA_GC_INDEX_PORT, VGA_GC_GRAPHICS_MODE_REG, graphics_mode);
   vga_write_reg(VGA_GC_INDEX_PORT, VGA_GC_MISC_REG, 0x0C);
}

#endif









