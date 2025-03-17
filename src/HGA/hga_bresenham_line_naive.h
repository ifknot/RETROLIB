#ifndef HGA_BRESENHAM_LINE_NAIVE_H
#define HGA_BRESENHAM_LINE_NAIVE_H

#include <stdint.h>

void hga_bresenham_line_naive(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);

#endif
