#ifndef HGA_FAST_LINE_H
#define HGA_FAST_LINE_H

/**
* starting at screen coord (x,y) draws a horizontal line to point (a, y) ignoring b arguement
*/
void hga_fast_hline(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t a, uint16_t b, uint8_t colour);

/**
* starting at screen coord (x,y) draws a vertical line to point (x,b) ignoring a arguement
*/
void hga_fast_vline(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t a, uint16_t b, uint8_t colour);

#endif
