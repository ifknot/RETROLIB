/**
 *
 *  @file      fixed_conversions.cpp
 *  @brief
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      8.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */

#include "fixed_conversions.h"

#include "../DBG/debug_macros.h"

    fixed_Q10_6_t fixed_fix(float f) {
        fixed_Q10_6_t q;
        f = ceil(f * 1000.0) / 1000.0;                   
        q = fixed_saturate((int32_t)(f));          
        f -= q;                                          
        q <<= Q10_6_FRACTIONAL_BITS;                     
        q += (fixed_Q10_6_t)(round(f * Q10_6_FDIVISOR)); 
        return q;
    }

    float fixed_unfix(fixed_Q10_6_t q) {
        float f = q & Q10_6_FRACTIONAL_MASK;
        f /= Q10_6_FDIVISOR;
        q >>= Q10_6_FRACTIONAL_BITS;
        f += q;
        return ceil(f * 1000.0) / 1000.0;
    }

    float fixed_unfix_fractional_part(fixed_Q10_6_t q) {
        return fixed_fractional_mask(q) / Q10_6_FDIVISOR;
    }

    fixed_Q10_6_t fixed_saturate(int32_t n) {
        if (n > Q10_6_INFINITY) return (fixed_Q10_6_t)Q10_6_MAX;
        if (n < Q10_6_NINFINITY) return (fixed_Q10_6_t)Q10_6_MIN;
        return (fixed_Q10_6_t)n;
    }

    int16_t fixed_truncate(fixed_Q10_6_t n) {
        return (int16_t)(n >> Q10_6_FRACTIONAL_BITS);
    }

    int16_t fixed_fractional_mask(fixed_Q10_6_t n) {
        return (n & Q10_6_FRACTIONAL_MASK);
    }
