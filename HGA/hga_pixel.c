
#include "hga_pixel.h"

const uint16_t ROW_TABLE[384] = {
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
};

void hga_plot_pixel_lookup_table(uint16_t vram_segment, uint16t x, uint16_t y, uint8_t colour, uint16_t* row_table) {}

void hga_plot_pixel_calculate(uint16_t vram_segment, uint16_t x, uint16_t y, colour_t c) {
    __asm {
        .8086
        pushf                                           ; preserve flags on entry
    
        // 1. set up registers
        mov     bx, x
        mov     cl, bl                                  ; copy of x low order byte
        mov     ax, y
        // 2. build offset into VRAM using rotations and CF to select VRAM bank
        // The carry flag becomes 8000h which gets shifted down each rotation
        shr     ax, 1                                   ; (y div 2) NB carry flag CF = bit shifted off
        rcr     bx, 1                                   ; BX = (x div 2) + 8000h * (y & 1) NB (y & 1) could be 0 hence selecting bank
        shr     ax, 1                                   ; AX = (y div 4) CF = select bank 2 ? NB y is now contained wholly in AL
        rcr     bx, 1                                   ; BX = (x div 4) + 4000h * (y & 3) via CF
        shr     bx, 1                                   ; BX = (x div 8) + 2000h * (y & 3) via CF
        mov     ah, HGA_BYTES_PER_LINE
        mul     ah                                              ; AL = (y div 4) * 90
        add     bx, ax                                  ; 2000h * (y & 3) + (x div 8) + ((y div 4) * 90)
        // 3. setup ES:[BX] to point to the VRAM byte containing pixel location
        mov     ax, vram_segment
        mov     es, ax
        // 4. setup AL = pixel bit mask, AH = pixel 'colour'
        and     cx, 7                                   ; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
        xor     cl, 7                                   ; CL = number of bits to shift left (thanks bit flip XOR)
        mov     ah, c                                   ; load ah with a single pixel at lsb (e.g. white 00000001)
        shl     ah, cl                                  ; shift single bit along by x mod 8
        // 5. display pixel
        or      es:[bx], ah                             ; plot point
    
        popf                                            ; restore flags on exit
    }
}

void hga_plot_pixel_mul_branch(uint16_t vram_segment, uint16t x, uint16_t y, colour_t c)(uint16_t buffer, uint16t x, uint16_t y, colour_t c) {}

uint8_t hga_read_buffer_byte_lookup_table(uint16_t vram_segment, uint16t x, uint16_t y) { return 0; }

uint8_t hga_read_pixel_lookup_table(uint16_t vram_segment, uint16t x, uint16_t y, uint16_t* row_table)  { return 0; }







