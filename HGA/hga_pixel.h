/*
graphics mode row table macro

n = 0
rept 87				; 384 / 4

dw n * 90			; 90 bytes per row 
dw 2000h + n * 90
dw 4000h + n * 90
dw 6000h + n * 90

n++
loop

HGA does not require await vertical retrace 
*/

// use clipping routines to massage *before* calling graphics functions

#include <stdint.h>

void hga_plot_pixel_lookup_table(uint16_t vram_segment, uint16t x, uint16_t y, uint8_t colour, uint16_t* row_table);

void hga_plot_pixel_shift_select(uint16_t vram_segment, uint16_t x, uint16_t y, colour_t c);

void hga_plot_pixel_mul_branch(uint16_t vram_segment, uint16t x, uint16_t y, colour_t c);

uint8_t hga_read_buffer_byte_lookup_table(uint16_t vram_segment, uint16t x, uint16_t y);

uint8_t hga_read_pixel_lookup_table(uint16_t vram_segment, uint16t x, uint16_t y, uint16_t* row_table);

inline void hga_plot_pixel(uint16t x, uint16_t y) {
  hga_plot_pixel_lookup_table(HGA_BUFFER_0, x, y, HGA_WHITE);
  // plot_pixel_calculate(HGA_BUFFER_0, x,y,HGA_WHITE);
  // plot_pixel_mul_jump(HGA_BUFFER_0, x,y,HGA_WHITE);
}

inline void hga_unplot_pixel(uint16t x, uint16_t y) {
  hga_plot_pixel_lookup_table(HGA_BUFFER_0, x, y, HGA_BLACK, &ROW_TABLE);
  // plot_pixel_calculate(HGA_BUFFER_0, x,y,HGA_BLACK);
  // plot_pixel_mul_jump(HGA_BUFFER_0, x,y,HGA_BLACK;
}

inline uint8_t hga_read_pixel(uint16t x, uint16_t y) {
  return hga_read_pixel_lookup_table(HGA_BUFFER_0, x, y, &ROW_TABLE); // uses hga_read_buffer_byte(buffer, x, y) mechanics
}
