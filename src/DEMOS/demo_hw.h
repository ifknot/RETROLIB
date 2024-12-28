#ifndef DEMO_HW_H
#define DEMO_HW_H

#include "../DBG/debug_macros.h"

#include "../HW/hw_constants_graphics.h"
#include "../HW/hw_detect_crtc.h"
#include "../HW/hw_detect_graphics_adapter.h"

void demo_hw() {

	INFO("***** DEMO HARDWARE DETECT FUNCTIONS *****");
	LOG(%s, hw_video_adapter_names[hw_detect_monochrome_adapter()]);
	LOG(%s, hw_video_adapter_names[hw_detect_colour_adapter()]);

}

#endif
