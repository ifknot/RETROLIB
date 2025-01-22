/**
* C is an unconstrained language and most of its security is built around setting conventions.
* @note UNSAFE none of the function arguements are sanity checked, processed, coded, formatted, or analyzed for safety
* They are, however, fast.
* @brief the "raw" family of low level graphics adapter specific graphics functions do no checking on their arguements!
* It is expected that higher level calling function(s) will sanitize and bounds check any arguements to these low level functions.
*/
#ifndef HGA_PIXEL_H
#define HGA_PIXEL_H

#include <stdint.h>

#include "hga_constants.h"
#include "hga_colours.h"

/**
* @brief at screen coord (x1,y1) plots a single pixel 'point' using look table for y vram row offsets
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
void hga_raw_plot_pixel_lookup_table(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t c);

/**
* @brief at screen coord (x1,y1) plots a single pixel 'point' calculating the y vram row offsets
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
void hga_raw_plot_pixel_calculate(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t c);

/**
* @brief at screen coord (x1,y1) reads a single pixel 'point' colour value (1 bit black or white in this case)
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
uint8_t hga_raw_read_pixel(uint16_t vram_segment, uint16_t x, uint16_t y);

#endif
