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

#include "../HARDWARE/hw_constants_graphics.h"
#include "../HARDWARE/hw_detect_crtc.h"

uint8_t cga_detect_adapter() {
	uint8_t adapter = HW_VIDEO_ADAPTER_UKNOWN;
	if (hw_detect_CRTC_at_port(crtc_port_CGA)) {
		adapter = HW_VIDEO_ADAPTER_CGA;
	}
	return adapter;
}
