/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef FIXED_TEST_H
#define FIXED_TEST_H

#include <assert.h>
#include <stdio.h>

#include "../DBG/debug_macros.h"

#include "fixed_types.h"

void fixed_test() {

	fixed_Q10_6_t x, y;
	fixed_point_t p;
	fixed_pair_t o;

	x = y = 1;
	p.x = 10;
	p.y = 20;
	o.part.x = 30;
	o.part.y = 40;

	INFO("fixed_test");

	assert(x == y == 1);
	assert(p.x == 10 && p.y == 20);
	assert(o.part.x == 0x1E && o.part.y == 0x28);
	assert(o.both == 0x28001E);

	INFO("pass");

}

#endif
