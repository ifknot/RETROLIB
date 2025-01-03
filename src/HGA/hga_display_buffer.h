#ifndef HGA_DISPLAY_BUFFER_H
#define HGA_DISPLAY_BUFFER_H

#include <stdint.h>

/**
* @brief select which of the 2 HGA adapter VRAM buffers ( 0 or 1 ) to be displayed on screen
* @note No sanity checking! Expects 0 or 1.
*/
void hga_select_display_buffer(uint8_t select);

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
* @details
* @note No sanity checking!
*/
void hga_write_vram_buffer_lookup(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t byte_pattern);

/**
* @brief Read an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) from the specified HGA VRAM buffer
* @details
* @note No sanity checking!
*/
uint8_t hga_read_vram_buffer_lookup(uint16_t vram_segment, uint16_t x, uint16_t y);

inline void hga_write_vram_buffer(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t byte_pattern) {
  hga_write_vram_buffer_lookup(vram_segment, x, y, byte_pattern);
}

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
* @brief clear the specified HGA VRAM buffer to black
*/
inline void hga_cls(uint16_t vram_segment) {
    hga_fill_vram_buffer(vram_segment, 0);
}

/** 
* @brief scroll graphics vram up number of lines and blank last line
*/
void hga_scroll_up(uint16_t vram_segment, uint16_t lines);

/**
* @breid repeat scroll up entire screen until whole screen blank
*/
void hga_screen_scroll_up(uint16_t vram_segment);

#endif
