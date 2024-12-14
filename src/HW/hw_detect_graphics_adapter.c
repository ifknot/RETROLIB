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
    uint8_t mono_card = HW_VIDEO_ADAPTER_UKNOWN;

    mono_card = hga_detect_adapter();
	if(mono_card != HW_VIDEO_ADAPTER_HGA_INCOLOR) return mono_card;

	return HW_VIDEO_ADAPTER_UKNOWN;
}

uint8_t hw_detect_colour_adapter() {
	uint8_t colour_card = HW_VIDEO_ADAPTER_UKNOWN;

	colour_card = vga_detect_adapter();
	if (colour_card) return colour_card;

	colour_card = ega_detect_adapter();
	if (colour_card) return colour_card;

	colour_card = cga_detect_adapter();
	if (colour_card) return colour_card;

	colour_card = hga_detect_adapter();
	if(colour_card == HW_VIDEO_ADAPTER_HGA_INCOLOR) return colour_card;

	return HW_VIDEO_ADAPTER_UKNOWN;
}
