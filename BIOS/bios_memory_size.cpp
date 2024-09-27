/**
 *
 *  @file      bios_memory_size.cpp
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      22.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#include "bios_memory_size.h"

namespace bios {

	uint16_t memory_size_KiB() {
		uint16_t size_kib;
		__asm {
			.8086
			int		BIOS_REPORT_MEMORY_SIZE
			mov		size_kib, ax
		}
		return size_kib;
	}

}