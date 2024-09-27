/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      18.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef TEST_BIOS_H
#define TEST_BIOS_H

#include "../TEST/debug_macros.h"

#include "test_bios_video.h"

namespace test_bios {

	void run() {
		INFO("* testing BIOS services...");
		if (YESNO("* 310\ttest BIOS video services ?")) test_bios_video::run();

	}

}

#endif