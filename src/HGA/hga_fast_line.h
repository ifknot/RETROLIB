#ifndef HGA_FAST_LINE_H
#define HGA_FAST_LINE_H
#include <stdint.h>

/**
* starting at screen coord (x1,y1) draws a horizontal line to point (x2, y2) ignoring y2 arguement
*/
void hga_fast_hline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour);

/**
* starting at screen coord (x1,y1) draws a vertical line to point (x2,y2) ignoring x2 arguement
*/
void hga_fast_vline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour);

#endif
