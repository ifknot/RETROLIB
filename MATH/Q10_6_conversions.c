#include <math.h>

#include "Q10_6_conversions.h"

Q10_6_t Q10_6__fix(float f) {
    Q10_6_t q;
    f = ceil(f * 1000.0) / 1000.0;                   
    q = fixed_saturate((int32_t)(f));          
    f -= q;                                          
    q <<= Q10_6_FRACTIONAL_BITS;                     
    q += (Q10_6_t)(round(f * Q10_6_FDIVISOR)); 
    return q;
}

float Q10_6__unfix(Q10_6_t q) {
    float f = q & Q10_6_FRACTIONAL_MASK;
    f /= Q10_6_FDIVISOR;
    q >>= Q10_6_FRACTIONAL_BITS;
    f += q;
    return ceil(f * 1000.0) / 1000.0;
}

float Q10_6__unfix_fractional_part(Q10_6_t q) {
    return fixed_fractional_mask(q) / Q10_6_FDIVISOR;
}

Q10_6_t Q10_6__saturate(int32_t n) {
    if (n > Q10_6_INFINITY) return (Q10_6_t)Q10_6_MAX;
    if (n < Q10_6_NINFINITY) return (Q10_6_t)Q10_6_MIN;
    return (Q10_6_t)n;
}

int16_t Q10_6__truncate(Q10_6_t q) {
    return (int16_t)(q >> Q10_6_FRACTIONAL_BITS);
}

int16_t Q10_6__fractional_mask(Q10_6_t q) {
    return (q & Q10_6_FRACTIONAL_MASK);
}
