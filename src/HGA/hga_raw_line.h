/**
* C is an unconstrained language and most of its security is built around setting conventions.
* @note UNSAFE none of the function arguements are sanity checked, processed, coded, formatted, or analyzed for safety
* They are, however, fast.
* @brief the "raw" family of low level graphics adapter specific graphics functions do no checking on their arguements!
* It is expected that higher level calling function(s) will sanitize and bounds check any arguements to these low level functions.
*/
#ifndef HGA_RAW_LINE_H
#define HGA_RAW_LINE_H
#include <stdint.h>

/**
* @breif starting at screen coord (x1,y1) draws a horizontal line to point (x2, y2)
* ignoring y2 arguement
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
void hga_raw_hline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour);

/**
* @brief starting at screen coord (x1,y1) draws a vertical line to point (x2,y2)
* ignoring x2 arguement
* @note ABSOLUTELY NO BOUNDS CHECKING!
*/
void hga_raw_vline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour);

#endif
