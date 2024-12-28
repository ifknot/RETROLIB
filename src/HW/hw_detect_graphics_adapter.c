/**
 *
 *  @file      hw_detect_graphics_adapter.c
 *  @brief
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.09.2024
 *  @copyright Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "hw_detect_graphics_adapter.h"

#include "../HGA/hga_detect_adapter.h"
#include "../CGA/cga_detect_adapter.h"
#include "../EGA/ega_detect_adapter.h"
#include "../VGA/vga_detect_adapter.h"
#include "hw_constants_graphics.h"

uint8_t hw_detect_monochrome_adapter() {
	return hga_detect_adapter();   // NB detects MDA adapter too
}

uint8_t hw_detect_colour_adapter() {
	if (vga_detect_adapter()) return HW_VIDEO_ADAPTER_VGA;
	if (ega_detect_adapter()) return HW_VIDEO_ADAPTER_EGA;
	if (cga_detect_adapter()) return HW_VIDEO_ADAPTER_CGA;
	return HW_VIDEO_ADAPTER_UKNOWN;;
}
