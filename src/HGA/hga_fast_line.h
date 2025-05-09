/**
* C is an unconstrained language and most of its security is built around setting conventions.
* @note UNSAFE none of the function arguements are sanity checked, processed, coded, formatted, or analyzed for safety
* They are, however, fast.
* @brief the family of low level graphics adapter specific graphics functions do no checking on their arguements!
* It is expected that higher level calling function(s) will sanitize and bounds check any arguements to these low level functions.
*/
#ifndef HGA_LINE_H
#define HGA_LINE_H
#include <stdint.h>

/**
* @brief starting at screen coord (x0,y0) draws a horizontal line to point (x1, y1)
* ignoring y1 arguement
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
void hga_hline(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);

/**
* @brief starting at screen coord (x0,y0) draws a vertical line to point (x1,y1)
* ignoring x1 arguement
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
void hga_vline(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);

void hga_dash_hline(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour, uint8_t pattern);

void hga_dash_vline(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour, uint8_t pattern);

#endif
