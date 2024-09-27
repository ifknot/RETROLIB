/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef TEST_DOS_MEMORY_H
#define TEST_DOS_MEMORY_H

#include <iostream>

#include "../TEST/debug_macros.h"

#include "../MEM/mem.h"

#include "../BIOS/bios_memory_size.h"

#include "dos.h"

namespace test_dos_memory {

	void run() {
		INFO("* testing DOS memory services...");
		LOG(bios::memory_size_KiB());
		uint32_t start_bytes = mem::available_low_memory_bytes();
		LOG(mem::available_low_memory_bytes());
		uint16_t seg_mem_1K = 0;
		uint16_t seg_mem_2K = 0;

		if (YESNO("* allocate 1K ? ")) {
			seg_mem_1K = dos::allocate_memory_blocks(64); // 1024K bytes == 64 x paragraphs (16 bytes per paragraph)
			INFO("seg_mem_1K = dos::allocate_memory_blocks(64)");
			LOG_AS(seg_mem_1K, std::hex);
			ASSERT(seg_mem_1K, != , 0, "fail allocate mem");
			LOG(mem::available_low_memory_bytes());
		}
		if (YESNO("* examine MCB ?")) {
			mem::address_t mcb_1k; // set it up to point to the paragraph preceding the one returned
			mcb_1k.segoff.segment = seg_mem_1K - 1;
			mcb_1k.segoff.offset = 0;
			ASSERT(*(char*)mcb_1k.memloc, == , 'M', "MCB not an M");
			INFO("* MCB dump");
			mem::dump(mcb_1k, 16);
		}

		if (YESNO("* allocate 2K ?")) {
			seg_mem_2K = dos::allocate_memory_blocks(128); // 2K
			INFO("seg_mem_2K = dos::allocate_memory_blocks(128)");
			LOG_AS(seg_mem_2K, std::hex);
			ASSERT(seg_mem_2K, != , 0, "fail allocate mem");
			LOG(mem::available_low_memory_bytes());
		}
		if (YESNO("* examine MCB ?")) {
			mem::address_t mcb_2k;
			mcb_2k.segoff.segment = seg_mem_2K - 1;
			mcb_2k.segoff.offset = 0;
			ASSERT(*(char*)mcb_2k.memloc, == , 'M', "MCB not an M");
			INFO("* MCB dump");
			mem::dump(mcb_2k, 16);
		}

		if (YESNO("* free 2K")) {
			ASSERT(dos::free_allocated_memory_blocks(seg_mem_2K), == , 0, "fail free");
			LOG(mem::available_low_memory_bytes());
		}
		if (YESNO("* free 1K")) {
			ASSERT(dos::free_allocated_memory_blocks(seg_mem_1K), == , 0, "fail free");
			LOG(mem::available_low_memory_bytes());
		}

		if (YESNO("* allocate 2K at old 1K address")) {
			seg_mem_2K = dos::allocate_memory_blocks(128); // 2K
			INFO("seg_mem_2K = dos::allocate_memory_blocks(128)");
			LOG_AS(seg_mem_2K, std::hex);
			ASSERT(seg_mem_2K, == , seg_mem_1K, "not reused");
			LOG(mem::available_low_memory_bytes());
			ASSERT(dos::free_allocated_memory_blocks(seg_mem_2K), == , 0, "fail free");
			LOG(mem::available_low_memory_bytes());
		}

		if (YESNO("* test error conditions ?")) {
			INFO("* try and free same 1K again");
			LOG(dos::free_allocated_memory_blocks(seg_mem_1K)); // should error out re-free freed

			INFO("* try free invalid MCB");
			LOG(dos::free_allocated_memory_blocks(seg_mem_1K + 1)); // should error out invalid MCB

			INFO("* try allocate too much");
			LOG(dos::allocate_memory_blocks(40960)); // All 640K - should error out
		}

		ASSERT(start_bytes, == , mem::available_low_memory_bytes(), "memory leak!");
	}

}

#endif