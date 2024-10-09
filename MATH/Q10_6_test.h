/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef Q10_6_TEST_H
#define Q10_6_TEST_H

#include <assert.h>
#include <stdio.h>

#include "../DBG/debug_macros.h"

#include "Q10_6.h"

void Q10_6_test() {

	Q10_6_t x, y;
	Q10_6_t q = -100;
	Q10_6_t pi = fixed_fix(3.14159265359);
	Q10_6_t sin1 = fixed_fix(0.01745240643);
	Q10_6_t sin2 = fixed_fix(0.0348994967);
	Q10_6_t sin3 = fixed_fix(0.05233595624);

	Q10_6_point_t p;
	Q10_6_pair_t o;

	x = y = 1;
	p.x = 10;
	p.y = Q10_6_PI;
	o.part.x = 30;
	o.part.y = 40;

	INFO("fixed tests...");
	
	// types
	assert(x == y == 1);
	assert(p.x == 10 && p.y == 201);
	assert(o.part.x == 0x1E && o.part.y == 0x28);
	assert(o.both == 0x28001E);

	// conversions
	assert(fixed_fix(3.14159) == 201);
	assert(fixed_unfix(Q10_6_PI) == 3.141f);
	assert(fixed_unfix(sin1) ==  0.016f);
	assert(fixed_unfix(sin2) ==  0.032f);
	assert(fixed_unfix(sin3) ==  0.047f);
	assert(fixed_fix(3.14159) == 201);
	assert(fixed_unfix(Q10_6_PI) == fixed_unfix(pi));
	assert(fixed_truncate(Q10_6_PI) ==  3);
	assert(fixed_unfix_fractional_part(Q10_6_PI) == 0.140625f);
	assert(fixed_saturate(-2), == , -2);
	assert(fixed_saturate(q), == , -100);
	assert(fixed_saturate(Q10_6_INFINITY + 1) == Q10_6_MAX);
	assert(fixed_saturate(Q10_6_NINFINITY - 1) == Q10_6_MIN);
	assert(fixed_saturate(40000) == Q10_6_MAX);
	assert(fixed_saturate(-40000) == Q10_6_MIN);
	
	INFO("pass");

}

#endif
