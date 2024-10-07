



#ifndef MATH_PRNG_H
#define MATH_PRNG_H

#include <stdint.h> 

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
uint16_t math_prng_xorshift16();

/**
* Ranged version
*/
uint16_t math_prng_range_xorshift16(uint16_t min, uint16_t max);

void math_prng_set_seed16(uint16_t seed);

#endif
