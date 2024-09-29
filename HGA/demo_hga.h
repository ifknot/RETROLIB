/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      29.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef DEMO_HGA_H
#define DEMO_HGA_H

#include <assert.h>

#include "..\DBG\debug_macros.h"

#include "hga.h"

void demo_hga() {

	INFO("***** DEMO HERCULES GRAPHICS FUNCTIONS *****");
	if (!hga_detect_adapter()) {
		INFO("ERROR! No Hercules Family Graphics Adapter Installed.");
		return;
	}
	if (YESNO("* switch to HGA Graphics Mode? ")) {
		INFO("HGA Graphics Mode.");
		PRESSENTER("* switch back to MDA Text Mode... ");
		INFO("MDA Text Mode.");
	}
}

#endif
