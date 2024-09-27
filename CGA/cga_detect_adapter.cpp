/**
 *
 *  @file      cga_detect_adapter.cpp
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      5.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#include "cga_detect_adapter.h"

#include "../gfx_constants.h"
#include "../gfx_detect_crtc.h"

namespace cga {

	uint8_t detect_adapter() {
		uint8_t adapter = VIDEO_ADAPTER_UKNOWN;
		if (gfx::detect_CRTC_at_port(gfx::crtc_port_CGA)) {
			adapter = VIDEO_ADAPTER_CGA;
		}
		return adapter;
	}

}