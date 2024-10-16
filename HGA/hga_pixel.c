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

static const uint16_t ROW_TABLE[348] = {
    0x0000, 0x2000, 0x4000, 0x6000, 0x005A, 0x205A, 0x405A, 0x605A, 0x00B4, 0x20B4, 0x40B4, 0x60B4, 0x010E, 0x210E, 0x410E, 0x610E, 0x0168, 0x2168, 0x4168, 0x6168, 0x01C2, 0x21C2, 0x41C2, 0x61C2, 0x021C, 0x221C, 0x421C, 0x621C, 0x0276, 0x2276, 0x4276, 0x6276, 0x02D0,
    0x22D0, 0x42D0, 0x62D0, 0x032A, 0x232A, 0x432A, 0x632A, 0x0384, 0x2384, 0x4384, 0x6384, 0x03DE, 0x23DE, 0x43DE, 0x63DE, 0x0438, 0x2438, 0x4438, 0x6438, 0x0492, 0x2492, 0x4492, 0x6492, 0x04EC, 0x24EC, 0x44EC, 0x64EC, 0x0546, 0x2546, 0x4546, 0x6546, 0x05A0, 0x25A0, 
    0x45A0, 0x65A0, 0x05FA, 0x25FA, 0x45FA, 0x65FA, 0x0654, 0x2654, 0x4654, 0x6654, 0x06AE, 0x26AE, 0x46AE, 0x66AE, 0x0708, 0x2708, 0x4708, 0x6708, 0x0762, 0x2762, 0x4762, 0x6762, 0x07BC, 0x27BC, 0x47BC, 0x67BC, 0x0816, 0x2816, 0x4816, 0x6816, 0x0870, 0x2870, 0x4870, 
    0x6870, 0x08CA, 0x28CA, 0x48CA, 0x68CA, 0x0924, 0x2924, 0x4924, 0x6924, 0x097E, 0x297E, 0x497E, 0x697E, 0x09D8, 0x29D8, 0x49D8, 0x69D8, 0x0A32, 0x2A32, 0x4A32, 0x6A32, 0x0A8C, 0x2A8C, 0x4A8C, 0x6A8C, 0x0AE6, 0x2AE6, 0x4AE6, 0x6AE6, 0x0B40, 0x2B40, 0x4B40, 0x6B40, 
    0x0B9A, 0x2B9A, 0x4B9A, 0x6B9A, 0x0BF4, 0x2BF4, 0x4BF4, 0x6BF4, 0x0C4E, 0x2C4E, 0x4C4E, 0x6C4E, 0x0CA8, 0x2CA8, 0x4CA8, 0x6CA8, 0x0D02, 0x2D02, 0x4D02, 0x6D02, 0x0D5C, 0x2D5C, 0x4D5C, 0x6D5C, 0x0DB6, 0x2DB6, 0x4DB6, 0x6DB6, 0x0E10, 0x2E10, 0x4E10, 0x6E10, 0x0E6A, 
    0x2E6A, 0x4E6A, 0x6E6A, 0x0EC4, 0x2EC4, 0x4EC4, 0x6EC4, 0x0F1E, 0x2F1E, 0x4F1E, 0x6F1E, 0x0F78, 0x2F78, 0x4F78, 0x6F78, 0x0FD2, 0x2FD2, 0x4FD2, 0x6FD2, 0x102C, 0x302C, 0x502C, 0x702C, 0x1086, 0x3086, 0x5086, 0x7086, 0x10E0, 0x30E0, 0x50E0, 0x70E0, 0x113A, 0x313A, 
    0x513A, 0x713A, 0x1194, 0x3194, 0x5194, 0x7194, 0x11EE, 0x31EE, 0x51EE, 0x71EE, 0x1248, 0x3248, 0x5248, 0x7248, 0x12A2, 0x32A2, 0x52A2, 0x72A2, 0x12FC, 0x32FC, 0x52FC, 0x72FC, 0x1356, 0x3356, 0x5356, 0x7356, 0x13B0, 0x33B0, 0x53B0, 0x73B0, 0x140A, 0x340A, 0x540A, 
    0x740A, 0x1464, 0x3464, 0x5464, 0x7464, 0x14BE, 0x34BE, 0x54BE, 0x74BE, 0x1518, 0x3518, 0x5518, 0x7518, 0x1572, 0x3572, 0x5572, 0x7572, 0x15CC, 0x35CC, 0x55CC, 0x75CC, 0x1626, 0x3626, 0x5626, 0x7626, 0x1680, 0x3680, 0x5680, 0x7680, 0x16DA, 0x36DA, 0x56DA, 0x76DA, 
    0x1734, 0x3734, 0x5734, 0x7734, 0x178E, 0x378E, 0x578E, 0x778E, 0x17E8, 0x37E8, 0x57E8, 0x77E8, 0x1842, 0x3842, 0x5842, 0x7842, 0x189C, 0x389C, 0x589C, 0x789C, 0x18F6, 0x38F6, 0x58F6, 0x78F6, 0x1950, 0x3950, 0x5950, 0x7950, 0x19AA, 0x39AA, 0x59AA, 0x79AA, 0x1A04, 
    0x3A04, 0x5A04, 0x7A04, 0x1A5E, 0x3A5E, 0x5A5E, 0x7A5E, 0x1AB8, 0x3AB8, 0x5AB8, 0x7AB8, 0x1B12, 0x3B12, 0x5B12, 0x7B12, 0x1B6C, 0x3B6C, 0x5B6C, 0x7B6C, 0x1BC6, 0x3BC6, 0x5BC6, 0x7BC6, 0x1C20, 0x3C20, 0x5C20, 0x7C20, 0x1C7A, 0x3C7A, 0x5C7A, 0x7C7A, 0x1CD4, 0x3CD4, 
    0x5CD4, 0x7CD4, 0x1D2E, 0x3D2E, 0x5D2E, 0x7D2E, 0x1D88, 0x3D88, 0x5D88, 0x7D88, 0x1DE2, 0x3DE2, 0x5DE2, 0x7DE2, 0x1E3C, 0x3E3C, 0x5E3C, 0x7E3C 
};

void hga_plot_pixel_lookup_table(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t c) {
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
        and     es:[bx], al                             ; mask out the pixel bits
        or      es:[bx], ah                             ; plot point
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





