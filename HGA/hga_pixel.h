/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      6.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef HGA_PIXEL_H
#define HGA_PIXEL_H

// use clipping routines to massage *before* calling graphics functions

#include <stdint.h>

#include "hga_constants.h"
#include "hga_colours.h"

void hga_plot_pixel_lookup_table(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t c, uint16_t* row_table);

void hga_plot_pixel_calculate(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t c);

uint8_t hga_read_buffer_byte_lookup_table(uint16_t vram_segment, uint16_t x, uint16_t y);

uint8_t hga_read_pixel_lookup_table(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t* row_table);

#endif
