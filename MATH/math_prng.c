
#include "math_prng.h"

#include <stdint.h>

/**
* @note the initial seeding value must not be zero
*/
static uint32_t _seed = 0xDECAFBAD;

uint16_t math_prng_xorshift16() {
    __asm {
        .8086
        push    ds 

        les     ax, _seed

        mov     bx, ax 
        mov     cl, 7
        shl     bx, cl 
        xor     ax, bx 

        mov     bx, ax
        mov     cl, 9
        shr     bx, cl
        xor     ax, bx

        mov     bx, ax
        mov     cl, 8
        shl     bx, cl
        xor     ax, bx

        lea     bx, _seed
        mov     [bx], ax

        pop     ds
    }
   
    return _seed;
}

uint16_t math_prng_range_xorshift16(uint16_t min, uint16_t max) {
    /*
    Another note is that there is more easy way to get the number in the needed interval :

    mov     ax, dx
    xor     dx, dx
    mov     bx, max - min
    div     bx              ; DX remainder(modulus)
    return 0;
    */
}

void math_prng_set_seed(uint32_t s) {
    _seed = s;
}
