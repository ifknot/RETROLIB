#include "bios_memory_size.h"

bios_memory_size_t bios_memory_size_KiB() {
	bios_memory_size_t size_kib = 0;
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
