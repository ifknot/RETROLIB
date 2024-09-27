/*!
 *  @file fixed_types.h
 *  @brief Fixed point mathematics designed for DOS gaming upto and including XGA
 *  @details   Screen dimenisions likely in DOS games:
 *  + CGA4		320 x 200
 *  + CGA6		600 x 200
 *  + HGA		720 x 348
 *  + VGA		640 x 480
 *  + SVGA		800 x 600
 *	+ XGA		1024 x 768
 *  For comparison "High Definition" HD is defined as:
 *  + HD720		1280 x 720
 *  + HD		1920 x 1080
 *  Choosing fixed point layout of 10:6 i.e. 10 bit integral and 6 bit fractional parts would give:
 *  fixed (-512 < n < 511.984)
 *  A fractional part of of 6 bits gives 1/64 resolution:
 *  + x range -512..511.984 or a pixel range of 0..1022.984 which would round to 0..1023 ie XGA - this library's maximum target DOS resolution
 *  + 0.015625 of a pixel or, to an effective 3dp, the smallest step in Q10.6 is 0.016
 *  + float value of PI		= 3.14159265359		Q10.6 PI	 = 3.141 (99.9% accurate)
 *  Accuracy issues arise at the 3rd decimal place using only 6 bits for the fractional part:
 *  + float sin of 1 degree = 0.01745240643		Q10.6 sin(1) = 	0.016 (91.7% accurate)
 *  + float sin(2)			= 0.0348994967		Q10.6 sin(2) = 	0.031 (88.8% accurate)
 *  + float sin(3)			= 0.05233595624		Q10.6 sin(3) = 	0.047 (89.8% accurate)
 *  + float 0.023415		= 0.023415			Q10.6		 =	0.156 (66.7% accurate)
 *  So the worst case scenario is the the 3dp is only 67% accurate
 *  @note Q10.6 fixed point math is only accurate to about 2.5 decimal places or about 6% uncertainty at 3dp
 *
 *  @author Jeremy Thornton
 *  @date 2024-07-27
 *  @copyright © Jeremy Thornton. All right reserved.
 */
#ifndef FIXED_TYPES_H
#define FIXED_TYPES_H

#include <stdint.h>



	/*!
	 * The Q notation consists of the letter Q followed by a pair of numbers m.n
	 * where m is the number of bits used for the integer part of the value, and n is the number of fraction bits.
	 * By default, the notation describes signed binary fixed point format, with the unscaled integer being stored
	 * in two's complement format, used in most binary processors.
	 * The first bit always gives the sign of the value(1 = negative, 0 = non-negative), and it is not counted in
	 * the m parameter. Thus, the total number w of bits used is 1 + m + n.
	 */
	
	typedef int16_t fixed_Q10_6_t;

	typedef struct {
		fixed_Q10_6_t x;
		fixed_Q10_6_t y;
	} fixed_point_t;

	/**
	* A 1D tensor rank two Q10_6_t components <part[0], part[1]> occupying 32 bits of memory
	*/
	typedef union {

		int32_t			both;
		fixed_point_t	part;

	} fixed_pair_t ;



#endif
