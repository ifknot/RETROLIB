/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef FIXED_CONSTANTS_H
#define FIXED_CONSTANTS_H

#include <stdint.h>

#define Q10_6_INTEGRAL_BITS	10
#define Q10_6_FRACTIONAL_BITS	6
#define Q10_6_FRACTIONAL_MASK	0x3F		// 0000000000.111111
#define Q10_6_FDIVISOR			64.0
#define Q10_6_MAX				0x7FFF		// 0111111111.111111
#define Q10_6_MIN				-0x8000		// 1000000000.000000
#define Q10_6_MAXINT			511
#define Q10_6_MININT			-512
#define Q10_6_FRACTIONAL_MIN	0.015625	// 0000000000.000001


										// As 10:6 fixed point Q10_6_ values:
	const int16_t Q10_6_PI = 201;			//    3.141 
	const int32_t Q10_6_INFINITY = 32767;	//  511.984 
	const int32_t Q10_6_NINFINITY = -32768;	// -512.000
	const float	Q10_6_FLOAT_MAX = 511.984;
	const float	Q10_6_FLOAT_MIN = -512.0;

#endif
