#include "bios_tools_timer.h"
#include "bios_timer_io_constants.h"

float bios_tools_timer_ticks_to_seconds(bios_ticks_since_midnight_t ticks) {
	return (float)ticks / TICKS_PER_SECOND;
}
