/**
 *  @brief
 *  @details   8254 channel 0 runs at 1.19318 mhz or ~ 838.0965 nsecs / cycle
 *  System timer interrupts every 65536 cycles ~= 54.9255 ms or ~ 18.20648 ticks per second
 *  1 ms   = 1193.18 cycles
 *  1 hour ~= 65543 ticks ~= 3599.9816 secs
 *  24 hour ~= 1573040 (hex 1800B0) ticks ~= 86399.998 secs
 */
#include <stdint.h>
#include <stdio.h>

#include "bios_timer_io_services.h"
#include "bios_timer_io_constants.h"

#include "../DBG/debug_macros.h"

/**
* @brief  INT 1A,0 - Read System Clock Counter - it is incremented about once every 55 ms by INT 08H.
* @details Reads the BIOS Data Area address 40:6C Timer Counter DWord as updated by the INT 08 routine.
* INT 08 Timer (IRQ 0)  This interrupt is issued 18.2 times per second and is responsible for updating the
* system timer at 40:6C BDA, resetting diskette drive motors, and issuing INT 1C.
*
* @note The reality is that vector 08h wasn't "unused".
* In fact it was listed as reserved all the way back to Intel's design specification documents.
* Intel on the 8086/8088 reserved the first 32 interrurpt vectors.
* The problem is that IBM ignored Intel and actually used reserved vectors for the 8259.
* This caused the problems when the 80286 and 80386 came out (Coprocessor vector clashed and the 8259A clashed).
* Had IBM not designed a flawed system from the start and conformed to Intel this clash wouldn't have happened – as per the iAPX 86 and 88 Intel manual from 1981, page 4-17
* "...the first five interrupt vectors are associated with the software-initiated interrupts and the hardware non-maskable interrupt (NMI).
* The next 27 interrupt vectors are reserved by Intel and should not be used if compatibility with future Intel products is to be maintained..."
*
* AH = 00
* on return:
* AL = midnight flag, 1 if 24 hours passed since reset
* CX = high order word of tick count
* DX = low order word of tick count
* incremented approximately 18.206 times per second
* at midnight CX:DX is zero
*/
void bios_read_system_clock(bios_ticks_since_midnight_t* ticks) {
	__asm{
		.8086

		mov		ah, BIOS_READ_SYSTEM_CLOCK_COUNTER
		int		BIOS_CLOCK_SERVICES
		lds		bx, ticks
		mov		[bx], dx							; Intel is little endian
		mov		[bx + 2],cx

	}
}

/**
* @brief Sets the system clock to the specified tick-count.
* @details ticks - should be set to the number of seconds past midnight multiplied by approximately 18.206
* @note Also clears the timer overflow flag at 0040:0070.
*
* AH = 01H
* CX:DX desired tick-count setting
* on return:
* AH    0
* CF    (0) if no error		(1) on invalid setting
*/
void bios_set_system_clock(bios_ticks_since_midnight_t ticks) {
	uint8_t error = 0;
	__asm {
		.8086

		les		dx, ticks
		mov		cx, es
		mov     ah, BIOS_SET_SYSTEM_CLOCK_COUNTER
		int     BIOS_CLOCK_SERVICES
		jnc		OK
		mov		error, 0FFh;
OK:

	}
#ifndef NDEBUG
	if (error) {
		fprintf(stderr, "ERROR set system clock - invalid setting tick count = %s\n", ticks);
	}
#endif

}
