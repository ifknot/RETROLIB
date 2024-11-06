/*****************************************************************
 * @file   bios_tools_timer.h
 * @brief  
 * 
 * @author Jeremy Thornton
 * @date   October 2024
 *********************************************************************/

#ifndef TIME_CONVERSION_H
#define TIME_CONVERSION_H

#include <stdint.h>

#include "../BIOS/bios_timer_io_types.h"

float bios_tools_timer_ticks_to_seconds(bios_ticks_since_midnight_t ticks);


#endif 
