
#include "math_prng.h"

#include <stdint.h>

/**
* @note the seeding value must not be zero
*/
static uint16_t __prng_seed_16_bit = 1;

/**
* There are some performance gains, 8086 at least, to be had by replacing shifts with rotates and byte manipulations
* 8086 reg,CL shifts use 8+4n cycles as opposed to 2 cycles for reg,1 shifts 
*/
uint16_t math_prng_xorshift16() {
    __asm {
        .8086
        mov     bx, __prng_seed_16_bit

        mov     al, bh
        ror     al, 1               ; hi byte LSB into CF
        mov     al, bl
        ror     al, 1               ; lo byte rotate right with CF 
        xor     al, bh              ; xor hi byte seed
        mov     bh, al              ; save hi byte

        mov     al, bl              ; rotate bl and bh
        ror     al, 1
        mov     al, bh
        ror     al, 1
        xor     al, bl              ; xor rotated results
        mov     bl, al
        xor     al, bh
        mov     bh, al

        mov     __prng_seed_16_bit, bx  
    }
    return __prng_seed_16_bit;
}

void math_prng_set_seed16(uint16_t seed) {
    __prng_seed_16_bit = seed;
}
