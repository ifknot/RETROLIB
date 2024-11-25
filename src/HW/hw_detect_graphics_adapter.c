/**
 *
 *  @file      hw_detect_graphics_adapter.c
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "hw_detect_graphics_adapter.h"

#include "../HGA/hga_detect_adapter.h"
#include "../CGA/cga_detect_adapter.h"
#include "../EGA/ega_detect_adapter.h"
#include "../VGA/vga_detect_adapter.h"

uint8_t hw_detect_monochrome_adapter() {
	return hga_detect_adapter();
}

uint8_t hw_detect_colour_adapter() {
	uint8_t colour_card = 0;
	colour_card = vga_detect_adapter();
	if (colour_card) return colour_card;
	colour_card = ega_detect_adapter();
	if (colour_card) return colour_card;
	colour_card = cga_detect_adapter();
	return colour_card;
}
