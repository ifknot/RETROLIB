#ifndef HGA_BRESENHAM_LINE_H
#define HGA_BRESENHAM_LINE_H

#include <stdint.h>

void hga_bline0(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);

/**
* @url https://zingl.github.io/bresenham.html
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
void hga_bresenham_line(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);

#endif
