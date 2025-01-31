#ifndef HGA_BRESENHAM_LINE_H
#define HGA_BRESENHAM_LINE_H

/**
* @url https://zingl.github.io/bresenham.html
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
void hga_bresenham_line(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour);

#endif
