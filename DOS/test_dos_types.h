/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      9.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef TEST_DOS_TYPES_H
#define TEST_DOS_TYPES_H

#include "../TEST/debug_macros.h"
#include "../MEM/mem_address.h"

#include "dos.h"

#include <stdint.h>

namespace test_dos_types {

	void run() {
		INFO("* testing DOS services types...");
		char str[] = "hello";
		mem::address_t addr, bddr;
		addr.memloc = (uint32_t)str;
		std::cout << str << '\n'
			<< addr.segoff << '\n'
			<< addr << '\n'
			<< std::hex << (uint32_t)str << '\n';
		bddr.segoff.segment = addr.segoff.segment;
		bddr.segoff.offset = addr.segoff.offset;
		LOG(bddr);
		LOG(bddr.memloc);
		std::cout << ((char*)bddr.memloc) << '\n';
	}

}

#endif