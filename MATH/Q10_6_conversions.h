/*!
 *  @brief Declares the fixed conversions.
 *  @details
 *  + Saturation arithmetic avoids the dangers of modular wrap around by clamping storage size overflows to maximum or minimum representation of the storage size i.e. a local +infinity and -infinity.
 *  @note Further additions to a maximum or subtractions from a minimum will not change the result.
 *  @author Jeremy Thornton
 *  @date 2024-07-27
 *  @copyright © Jeremy Thornton. All right reserved.
 *
 */
#ifndef Q10_6_CONVERSIONS_H
#define Q10_6_CONVERSIONS_H

#include <stdint.h>

#include "Q10_6_types.h"
#include "Q10_6__constants.h"

Q10_6_t Q10_6__fix(float f);

float Q10_6__unfix(Q10_6_t q);

float Q10_6__unfix_fractional_part(Q10_6_t q);

Q10_6_t Q10_6__saturate(int32_t n);

int16_t Q10_6__truncate(Q10_6_t q);

int16_t Q10_6__fractional_mask(Q10_6_t q);

#endif
