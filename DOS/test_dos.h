#ifndef TEST_DOS_H
#define TEST_DOS_H

#include "../DBG/debug_macros.h"

#include "test_dos_types.h"
#include "test_dos_interrupt_vector.h"
#include "test_dos_memory.h"
#include "test_dos_files.h"
#include "test_dos_3x.h"

void  test_dos() {	
	INFO("* testing DOS services...");
	if (YESNO("* 110\ttest types ?")) test_dos_types();
	if (YESNO("* 120\ttest interrupt get/set ?")) test_dos_interrupt_vector();
	if (YESNO("* 130\ttest memory alloc/free ?")) test_dos_memory();
	if (YESNO("* 140\ttest file services ?")) test_dos_files();

	if (YESNO("* 190\ttest DOS 3.x+ ?")) test_dos_3x();
}

#endif
