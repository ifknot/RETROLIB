/*****************************************************************
 * @file   time_conversions.c
 * @brief  
 * 
 * @author Jeremy Thornton
 * @date   October 2024
 *********************************************************************/
#include "time_conversions.h"

#include "../BIOS/bios_timer_io_constants.h"
float time_convert_ticks_to_seconds(bios_ticks_since_midnight_t ticks) {
	return (float)ticks / TICKS_PER_SECOND;
}
