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

uint8_t hw_detect_monochrome_adapter() {
	return hga_detect_adapter();
}
