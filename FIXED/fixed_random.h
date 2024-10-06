/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      6.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef FIXED_RANDOM_H
#define FIXED_RANDOM_H

#include <stdint.h> 

#include "fixed_types.h"

/**
* Xorshift is a fast pseudorandom generator algorithm originally developed by George Marsaglia. 
* John Metcalf found a 16-bit version of the algorithm that is fast on 8/16-bit platforms with 
* only single bit shifts available. 
* It has a period of 65535 and passes reasonable tests for randomness. 
*
* 16-bit xorshift PRNG:
* 
*   unsigned x = 1;
*   unsigned xorshift() {
*    x ^= x << 7;
*    x ^= x >> 9;
*    x ^= x << 8;
*    return x;
*	}
*/

static uint16_t seed = 112;         // seed must not be 0

/**
* There are some performance gains, 8086 at least, by replacing shifts with rotates and byte manipulations
* 8086 reg,CL shifts use 8+4n cycles as opposed to 2 cycles for reg,1 shifts 
*/
uint16_t fixed_prng_xorshift() {
    __asm {
        .8086
        mov     bx, seed 

        mov     al, bh
        ror     al, 1               ; hi byte LSB into CF
        mov     al, bl
        ror     al, 1               ; lo byte rotate right with CF 
        xor     al, bh              ; xor hi byte seed
        mov     bh, al              ; save hi byte

        mov     al, bl          
        ror     al, 1
        mov     al, bh
        ror     al, 1
        xor     al, bl
        mov     bl, al
        xor     al, bh
        mov     bh, al

        mov     seed, bx
    }
    return seed;
}

void fixed_prng_seed(uint16_t s) {
    seed = s;
}

#endif
