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

#include "../DOS/dos_services_files_types.h"

/**
* @brief select which of the 2 HGA adapter VRAM buffers ( 0 or 1 ) to be displayed on screen
*/
void hga_select_display_buffer(uint16_t vram_segment);

/**
* @brief Write an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) to the specified HGA VRAM buffer
* @details uses a lookup table to address the correct quad bank in the HGA VRAM buffer
* @see hga_table_lookup_y.h
*/
void hga_write_vram_buffer_lookup(uint16_t vram_segment, uint16_t byte_column, uint16_t byte_row, uint8_t byte_pattern);

/**
* @brief Read an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) from the specified HGA VRAM buffer
* @details uses a lookup table to address the correct quad bank in the HGA VRAM buffer
* @see hga_table_lookup_y.h
*/
uint8_t hga_read_vram_buffer_lookup(uint16_t vram_segment, uint16_t byte_column, uint16_t byte_row);

/**
* @brief Write an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) to the specified HGA VRAM buffer
* @details inline wrapper function for the policy chosen to address the correct quad bank in the HGA VRAM buffer i.e. calculate vs. look up table
*/
inline void hga_write_vram_buffer(uint16_t vram_segment, uint16_t byte_column, uint16_t byte_row, uint8_t byte_pattern) {
  hga_write_vram_buffer_lookup(vram_segment, byte_column, byte_row, byte_pattern);
}

/**
* @brief Read an 8 bit byte pattern at (byte, line) coordinates x(0..89), y(0..347) from the specified HGA VRAM buffer
* @details inline wrapper function for the policy chosen to address the correct quad bank in the HGA VRAM buffer i.e. calculate vs. look up table
*/
inline uint8_t hga_read_vram_buffer(uint16_t vram_segment, uint16_t byte_column, uint16_t byte_row) {
  return hga_read_vram_buffer_lookup(vram_segment, byte_column, byte_row);
}

/**
* @brief Fast fill the specified HGA VRAM buffer with an 8 bit byte pattern
* @details
*/
void hga_fill_vram_buffer(uint16_t vram_segment, uint8_t byte_pattern);

/**
* @brief fill the specified HGA VRAM buffer with two 8 bit byte patterns alternating byte patterns between rows
* @details
*/
void hga_knit_vram_buffer(uint16_t vram_segment, uint8_t byte_pattern_a, uint8_t byte_pattern_b);

/**
* @brief load entire 32K VRAM from file of *raw* VRAM data i.e. page bank layout
*/
dos_file_size_t hga_load_vram_buffer(uint16_t vram_segment, const char* file_path);

/**
* @brief saveentire 32K VRAM from file of *raw* VRAM data i.e. page bank layout
*/
dos_file_size_t hga_save_vram_buffer(uint16_t vram_segment, const char* file_path);

/**
* @brief wrapper function to clear the specified HGA VRAM buffer to black
*/
inline void hga_cls(uint16_t vram_segment) {
    hga_fill_vram_buffer(vram_segment, 0);
}

/**
* @brief scroll graphics vram up number of lines and blank last line
* @note TODO
*/
void hga_scroll_up(uint16_t vram_segment, uint16_t lines);

/**
* @breif repeat scroll up entire screen until whole screen blank
* @note TODO
*/
void hga_screen_scroll_up(uint16_t vram_segment);

#endif
