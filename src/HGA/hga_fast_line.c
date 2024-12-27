#include "hga_fast_line.h"

void hga_fast_hline(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t a, uint16_t b, uint8_t colour) {

}

void hga_fast_vline(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t a, uint16_t b, uint8_t colour) {
    // 1. shift the pixel in DL right into the correct x mod 8 position 
    mov		ax, x			; load x 
    mov		cx, ax			; copy of x 
    and		cx, 7h			; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2) 
    mov		dl, 80h			; load DL with a single pixel at msb 10000000 
    shr		dl, cl			; shift single bit along by x mod 8
    // 2. x div 8 
    shr		ax, 1			; calculate column byte x / 8 
    shr		ax, 1			; poor old 8086 only has opcodes shifts by an implicit 1 or CL
    shr		ax, 1
    // 3. lookup y 
}
