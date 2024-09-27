/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef DEMO_HW_H
#define DEMO_HW_H

#include "..\DBG\debug_macros.h"

#include "hw_constants_graphics.h"
#include "hw_detect_crtc.h"
#include "hw_detect_graphics_adapter.h"

void demo_hw() {

	LOG(hw_video_adapter_names[hw_detect_colour_adapter()], %s);
}

#endif
