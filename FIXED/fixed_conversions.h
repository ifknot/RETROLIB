/*!
 *  @file fixed_conversions.h
 *  @brief Declares the fixed conversions.
 *  @details
 *  + Saturation arithmetic avoids the dangers of modular wrap around by clamping storage size overflows to maximum or minimum representation of the storage size i.e. a local +infinity and -infinity.
 *  @note Further additions to a maximum or subtractions from a minimum will not change the result.
 *  @author Jeremy Thornton
 *  @date 2024-07-27
 *  @copyright © Jeremy Thornton. All right reserved.
 *
 */
#ifndef FIXED_CONVERSIONS_H
#define FIXED_CONVERSIONS_H

#include <stdint.h>
#include <math.h>

#include "fixed_types.h"
#include "fixed_constants.h"

	fixed_Q10_6_t fixed_fix(float f);

	float fixed_unfix(fixed_Q10_6_t q);

	float fixed_unfix_fractional_part(fixed_Q10_6_t q);

	fixed_Q10_6_t  fixed_saturate(int32_t n);

	int16_t fixed_truncate(fixed_Q10_6_t n);

	int16_t fixed_fractional_mask(fixed_Q10_6_t n);

#endif
