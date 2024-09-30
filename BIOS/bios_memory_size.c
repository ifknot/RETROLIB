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

uint16_t bios_memory_size_KiB() {
	uint16_t size_kib;
	__asm {
		.8086
		pushf                                ; preserve what int BIOS functions may not
        	push    ds                           ; due to unreliable behaviour
	
		int		BIOS_REPORT_MEMORY_SIZE
		mov		size_kib, ax

		pop 	ds 
		popf
	}
	return size_kib;
}
