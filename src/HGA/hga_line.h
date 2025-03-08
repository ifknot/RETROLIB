#ifndef HGA_LINE_H
#define HGA_LINE_H

#include "hga_bresenham_line.h"

inline void hga_line(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour) {
    hga_bresenham_line(vram_segment, x0, y0, x1, y1, colour);
}

#endif
