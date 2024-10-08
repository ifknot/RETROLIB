
#include "math_prng.h"

#include <stdint.h>

/**
* @note the initial seeding value must not be zero
*/
static uint32_t _seed = 0xDECAFBAD;

/**
* @brief Xorshift-798 is an efficient 16-bit linear feedback shift registers (LFSR) pseudorandom number generator (PRNG).
* @details As shown by George Marsaglia and further analysed by Richard P. Brent:
* There are 60 shift triplets with the maximum period 216-1. 
* Four triplets pass a series of lightweight randomness tests, these are: 
*    6, 7, 13 
*    7, 9, 8
*    7, 9, 13
*    9, 7, 13
* @url https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Xorshift_LFSRs
*/
uint16_t math_prng_xorshift16() {
    __asm {
        .8086

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

    }
   
    return _seed;
}

uint16_t math_prng_range_xorshift16(uint16_t min, uint16_t max) {
    uint16_t rand = 0;
    __asm {
        .8086
    
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
        
    }
    return  rand;
}

void math_prng_set_seed(uint32_t s) {
    _seed = s;
}
