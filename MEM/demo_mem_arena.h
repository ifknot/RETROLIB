/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      17.10.2024
 *  @copyright � Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef DEMO_MEM_ARENA_H
#define DEMO_MEM_ARENA_H

#include <assert.h>

#include "..\DBG\debug_macros.h"

#include "mem_constants.h"
#include "mem_types.h"
#include "mem_arena.h"

void demo_mem_arena() {

	mem_size_t size = 0;
	mem_diff_t diff = 0;
	mem_segoff_t m;
	mem_address_t addr;

	assert(PARAGRAPH_SIZE == 16);
	assert(MAX_DOS_MEM_ALLOCATE == 1048560);
	assert(size == diff);

	m.segment = 0xDEAD;
	m.offset = 0xF00D;

	addr.segoff = m;

	assert(addr.ptr == (void*)0xDEADF00D);
	assert(addr.segoff.offset == m.offset);
	assert(addr.segoff.segment == addr.words[1]);
	assert(addr.bytes[3] = 0xDE);

	INFO("***** DEMO MEMORY ARENA *****");
	if (YESNO("* create arena 1K? ")) {
		
	}
}

#endif
