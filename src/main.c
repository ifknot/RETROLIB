#include <stdio.h>

#include "DBG/debug_macros.h"
#include "BIOS/bios_memory_size.h"

int main(int argc, char** argv) {
    bios_memory_size_t mem_size = bios_memory_size_KiB();

    INFO("Hello World!");
    LOG(%i, mem_size);

	return 0;

}
