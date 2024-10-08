
#include "math_prng.h"

#include <stdint.h>

/**
* @note the initial seeding value must not be zero
*/
 x ^= x << 7;
*    x ^= x >> 9;
*    x ^= x << 8;
static uint32_t _seed = 0xDECAFBAD;

uint16_t math_prng_xorshift16() {
    __asm {
        .8086
        push    ds 

        les     ax, _seed          ; ES:AX = 32 bit seed 

        mov     bx, ax             ; use 16 bit seed
        mov     cl, 7
        shl     bx, cl             ; x << 7
        xor     ax, bx             ; x ^= x << 7

        mov     bx, ax
        mov     cl, 9
        shr     bx, cl            ; x >> 9
        xor     ax, bx            ; x ^= x >> 9

        mov     bx, ax
        mov     cl, 8
        shl     bx, cl            ; x << 8
        xor     ax, bx            ; x ^= x << 8

        lea     di, _seed
        mov     [di], ax          ; update seed value

        pop     ds
    }
   
    return _seed;
}

uint16_t math_prng_range_xorshift16(uint16_t min, uint16_t max) {
    uint16_t rand = 0;
    __asm {
        .8086
        push    ds
    
        les     ax, _seed          ; ES:AX = 32 bit seed 

        mov     bx, ax             ; use 16 bit seed
        mov     cl, 7
        shl     bx, cl             ; x << 7
        xor     ax, bx             ; x ^= x << 7

        mov     bx, ax
        mov     cl, 9
        shr     bx, cl            ; x >> 9
        xor     ax, bx            ; x ^= x >> 9

        mov     bx, ax
        mov     cl, 8
        shl     bx, cl            ; x << 8
        xor     ax, bx            ; x ^= x << 8

        lea     di, _seed
        mov     [di], ax          ; update seed value

        mov     bx, max           ; calculate range 
        sub     bx, min           ; BX = range

        xor     dx, dx            ; prepare DX use in division
        div     bx                ; DX:AX / BX 
        add     dx, min           ; DX is the modulus + start of the range

        lea     di, rand
        mov     [di], dx          
        
        pop     ds
    }
    return  rand;
}

void math_prng_set_seed(uint32_t s) {
    _seed = s;
}
