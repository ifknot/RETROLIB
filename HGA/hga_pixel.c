
#include "hga_pixel.h"

const uint16_t ROW_TABLE[384] = {
    0000, 2000, 4000, 6000,  
    005A, 205A, 405A, 605A,  00B4, 20B4, 40B4, 60B4,  010E, 210E, 410E, 610E,  0168, 2168, 4168, 6168,  01C2, 21C2, 41C2, 61C2,  021C, 221C, 421C, 621C,  0276, 2276, 4276, 6276,  02D0, 22D0, 42D0, 62D0,  
    032A, 232A, 432A, 632A,  0384, 2384, 4384, 6384,  03DE, 23DE, 43DE, 63DE,  0438, 2438, 4438, 6438,  0492, 2492, 4492, 6492,  04EC, 24EC, 44EC, 64EC,  0546, 2546, 4546, 6546,  05A0, 25A0, 45A0, 65A0,  
    05FA, 25FA, 45FA, 65FA,  0654, 2654, 4654, 6654,  06AE, 26AE, 46AE, 66AE,  0708, 2708, 4708, 6708,  0762, 2762, 4762, 6762,  07BC, 27BC, 47BC, 67BC,  0816, 2816, 4816, 6816,  0870, 2870, 4870, 6870,  
    08CA, 28CA, 48CA, 68CA,  0924, 2924, 4924, 6924,  097E, 297E, 497E, 697E,  09D8, 29D8, 49D8, 69D8,  0A32, 2A32, 4A32, 6A32,  0A8C, 2A8C, 4A8C, 6A8C,  0AE6, 2AE6, 4AE6, 6AE6,  0B40, 2B40, 4B40, 6B40,  
    0B9A, 2B9A, 4B9A, 6B9A,  0BF4, 2BF4, 4BF4, 6BF4,  0C4E, 2C4E, 4C4E, 6C4E,  0CA8, 2CA8, 4CA8, 6CA8,  0D02, 2D02, 4D02, 6D02,  0D5C, 2D5C, 4D5C, 6D5C,  0DB6, 2DB6, 4DB6, 6DB6,  0E10, 2E10, 4E10, 6E10,  
    0E6A, 2E6A, 4E6A, 6E6A,  0EC4, 2EC4, 4EC4, 6EC4,  0F1E, 2F1E, 4F1E, 6F1E,  0F78, 2F78, 4F78, 6F78,  0FD2, 2FD2, 4FD2, 6FD2,  102C, 302C, 502C, 702C,  1086, 3086, 5086, 7086,  10E0, 30E0, 50E0, 70E0,  
    113A, 313A, 513A, 713A,  1194, 3194, 5194, 7194,  11EE, 31EE, 51EE, 71EE,  1248, 3248, 5248, 7248,  12A2, 32A2, 52A2, 72A2,  12FC, 32FC, 52FC, 72FC,  1356, 3356, 5356, 7356,  13B0, 33B0, 53B0, 73B0,  
    140A, 340A, 540A, 740A,  1464, 3464, 5464, 7464,  14BE, 34BE, 54BE, 74BE,  1518, 3518, 5518, 7518,  1572, 3572, 5572, 7572,  15CC, 35CC, 55CC, 75CC,  1626, 3626, 5626, 7626,  1680, 3680, 5680, 7680,  
    16DA, 36DA, 56DA, 76DA,  1734, 3734, 5734, 7734,  178E, 378E, 578E, 778E,  17E8, 37E8, 57E8, 77E8,  1842, 3842, 5842, 7842,  189C, 389C, 589C, 789C,  18F6, 38F6, 58F6, 78F6,  1950, 3950, 5950, 7950,  
    19AA, 39AA, 59AA, 79AA,  1A04, 3A04, 5A04, 7A04,  1A5E, 3A5E, 5A5E, 7A5E,  1AB8, 3AB8, 5AB8, 7AB8,  1B12, 3B12, 5B12, 7B12,  1B6C, 3B6C, 5B6C, 7B6C,  1BC6, 3BC6, 5BC6, 7BC6,  1C20, 3C20, 5C20, 7C20,  
    1C7A, 3C7A, 5C7A, 7C7A,  1CD4, 3CD4, 5CD4, 7CD4,  1D2E, 3D2E, 5D2E, 7D2E,  1D88, 3D88, 5D88, 7D88,  1DE2, 3DE2, 5DE2, 7DE2,  1E3C, 3E3C, 5E3C, 7E3C 
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

/* build row table
for(int i= 0; i < 87; ++i) {
        printf("%04X, %X, %X, %X,\n",i * 90, 0x2000 + i * 90, 0x4000 + i * 90, 0x6000 + i * 90);
    }
*/





