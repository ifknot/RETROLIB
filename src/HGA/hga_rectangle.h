
#ifndef HGA_RECTANGLE_H
#define HGA_RECTANGLE_H

#include <stdint.h>

/**
* @brief
*/
void hga_rectangle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t colour);

void hga_dash_rectangle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t colour, uint8_t pattern);

#endif
