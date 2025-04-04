/**
* C is an unconstrained language and most of its security is built around setting conventions.
* @note UNSAFE none of the function arguements are sanity checked, processed, coded, formatted, or analyzed for safety
* They are, however, fast.
* @brief the family of low level graphics adapter specific graphics functions do no checking on their arguements!
* It is expected that higher level calling function(s) will sanitize and bounds check any arguements to these low level functions.
*/
#ifndef HGA_DISPLAY_BUFFER_H
#define HGA_DISPLAY_BUFFER_H

#include <stdint.h>

/**
* @brief select which of the 2 HGA adapter VRAM buffers ( 0 or 1 ) to be displayed on screen
*/
void hga_select_display_buffer(uint16_t vram_segment);

/**
* @brief Write an 8 bit byte pattern at byte count location (0 .. 31,320) to the specified HGA VRAM buffer
* @note treats the VRAM as an indexable contiguous byte space (the quad bank HGA implementation is resolved in function)
* @details
* @note No sanity checking!
*/
void hga_write_vram_buffer_byte_location(uint16_t vram_segment, uint16_t byte_location, uint8_t byte_pattern);

/**
* @brief Read an 8 bit byte pattern at byte count location (0 .. 31,320) to the specified HGA VRAM buffer
* @note treats the VRAM as an indexable contiguous byte space (the quad bank HGA implementation is resolved in function)
* @details
* @note No sanity checking!
*/
uint8_t hga_read_vram_buffer_byte_location(uint16_t vram_segment, uint16_t byte_location);

/**
* @brief Write an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) to the specified HGA VRAM buffer
* @details uses a lookup table to address the correct quad bank in the HGA VRAM buffer
* @see hga_table_lookup_y.h
* @note No sanity checking!
*/
void hga_write_vram_buffer_lookup(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t byte_pattern);

/**
* @brief Read an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) from the specified HGA VRAM buffer
* @details uses a lookup table to address the correct quad bank in the HGA VRAM buffer
* @see hga_table_lookup_y.h
* @note No sanity checking!
*/
uint8_t hga_read_vram_buffer_lookup(uint16_t vram_segment, uint16_t x, uint16_t y);

/**
* @brief Write an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) to the specified HGA VRAM buffer
* @details inline wrapper function for the policy chosen to address the correct quad bank in the HGA VRAM buffer i.e. calculate vs. look up table
*/
inline void hga_write_vram_buffer(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t byte_pattern) {
  hga_write_vram_buffer_lookup(vram_segment, x, y, byte_pattern);
}

/**
* @brief Read an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) from the specified HGA VRAM buffer
* @details inline wrapper function for the policy chosen to address the correct quad bank in the HGA VRAM buffer i.e. calculate vs. look up table
*/
inline uint8_t hga_read_vram_buffer(uint16_t vram_segment, uint16_t x, uint16_t y) {
  return hga_read_vram_buffer_lookup(vram_segment, x, y);
}

/**
* @brief Fast fill the specified HGA VRAM buffer with an 8 bit byte pattern
* @details
* @note No sanity checking! Expects vram_segment to be valid.
*/
void hga_fill_vram_buffer(uint16_t vram_segment, uint8_t byte_pattern);

/**
* @brief fill the specified HGA VRAM buffer with two 8 bit byte patterns alternating byte patterns between rows
* @details
* @note No sanity checking! Expects vram_segment to be valid.
*/
void hga_knit_vram_buffer(uint16_t vram_segment, uint8_t byte_pattern_a, uint8_t byte_pattern_b);

int16_t hga_load_vram_buffer(uint16_t vram_segment, char* file_path);

int16_t hga_save_vram_buffer(uint16_t vram_segment, char* file_path);

/**
* @brief clear the specified HGA VRAM buffer to black
*/
inline void hga_cls(uint16_t vram_segment) {
    hga_fill_vram_buffer(vram_segment, 0);
}

/**
* @brief scroll graphics vram up number of lines and blank last line
* @note No sanity checking!
*/
void hga_scroll_up(uint16_t vram_segment, uint16_t lines);

/**
* @breif repeat scroll up entire screen until whole screen blank
* @note No sanity checking!
*/
void hga_screen_scroll_up(uint16_t vram_segment);

#endif
