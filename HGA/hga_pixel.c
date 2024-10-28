/**
 *
 *  @file               hga_pixel.c
 *  @brief              Plot or read a pixel in Hercules Graphics Mode
 *  @details    HGA Hi-res pixel-addressable graphics mode
 *  Resolution  720?×?348 graphics mode (pixel-addressable graphics)
 *  Colours             2-color (off black and on 'white')
 *  Palette     none
 *  VRAM                32KB (nearly) from a total of 64K (HGA is page switchable)
 *      Pixel Ratio 1.46:1 on a standard 4:3 display.
 *  Segment     B0000-BFFFF
 *      I/O ports   3B0-3BF.
 *  Layout      4 banks of interleaved scan lines, packed-pixel.
 *
 *          Each scan line is 90-bytes long and there are 256 scan lines
 *          Each byte contains 8 pixels
 *          ?7?6?5?4?3?2?1?0?
 *          ? ? ? ? ? ? ? ? ?
 *          ?????????????????  bits mask
 *           ? ? ? ? ? ? ? ???? 0:  01H  eighth pixel in byte
 *           ? ? ? ? ? ? ?????? 1:  02H  seventh pixel in byte
 *           ? ? ? ? ? ???????? 2:  04H  sixth pixel in byte
 *           ? ? ? ? ?????????? 3:  08H  fifth pixel in byte
 *           ? ? ? ???????????? 4:  10H  fourth pixel in byte
 *           ? ? ?????????????? 5:  20H  third pixel in byte
 *           ? ???????????????? 6:  40H  second pixel in byte
 *           ?????????????????? 7:  80H  first pixel in byte
 *                                       0=black; 1=white*
 *
 *          Each 1-bit field selects black (0) or 'white'(1).
 *  @author    Jeremy Thornton
 *  @date      21.06.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "hga_pixel.h"
#include "hga_table_y_lookup.h"

void hga_plot_pixel_lookup_table(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t c, const uint8_t* y_lookup) {
    __asm {
        // 1. set up registers
        mov     bx, x
        mov     cl, bl                                  ; copy of x low order byte
        mov     di, y
        // 2. look up row offset
    // lea     di, ROW_TABLE
    // add     y
        mov     di, [ROW_TABLE + di]
        // 3. setup ES:[DI] to point to the VRAM byte containing pixel location
        mov     ax, vram_segment
        mov     es, ax
        shr     bx, 1                                    ; x / 8
        shr     bx, 1 
        shr     bx, 1
        add     di, bx
        // 4. setup AL = pixel bit mask, AH = pixel 'colour'
        and     cx, 7                                   ; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
        xor     cl, 7                                   ; CL = number of bits to shift left (thanks bit flip XOR)
        mov     al, 11111110b                           ; AL = pixel mask
        rol     al, cl                                  ; roll mask around by x mod 8
        mov     ah, c                                   ; load ah with a single pixel at lsb (e.g. white 00000001)
        shl     ah, cl                                  ; shift single bit along by x mod 8
        // 5. display pixel
        and     es:[di], al                             ; mask out the pixel bits
        or      es:[di], ah                             ; plot point
    }
}

void hga_plot_pixel_calculate(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t c) {
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
        mov     al, 11111110b                           ; AL = pixel mask
        rol     al, cl                                  ; roll mask around by x mod 8
        mov     ah, c                                   ; load ah with a single pixel at lsb (e.g. white 00000001)
        shl     ah, cl                                  ; shift single bit along by x mod 8
        // 5. display pixel
        and     es:[bx], al                             ; mask out the pixel bits
        or      es:[bx], ah                             ; plot point
    
        popf                                            ; restore flags on exit
    }
}

/* build row table

for(int i= 0; i < 87; ++i) {
    printf("0x%04X, 0x%X, 0x%X, 0x%X,\n",i * 90, 0x2000 + i * 90, 0x4000 + i * 90, 0x6000 + i * 90);
}

*/





