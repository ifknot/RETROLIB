/**
 *  @brief    INT 1Ah: Timer I/O (and MRCI POST Hook)
 *  @url http://www.techhelpmanual.com/27-dos__bios___extensions_service_index.html
 */
#ifndef BIOS_TIMER_IO_SERVICES_H
#define	BIOS_TIMER_IO_SERVICES_H

#include "bios_timer_io_types.h"

// INT 1a,00 - Read System Clock (Ticks) [PC] [XT] [AT]
void bios_read_system_clock(bios_ticks_since_midnight_t* ticks);

// INT 1a,01 - Set System Clock (Ticks) [PC] [XT] [AT]
void bios_set_system_clock(bios_ticks_since_midnight_t ticks);

// INT 1a,02 - Read Time from Real-Time Clock [AT]
// INT 1a,03 - Set Time on Real-Time Clock [AT]
// INT 1a,04 - Read Date from Real-Time Clock [AT]
// INT 1a,05 - Set Date on Real-Time Clock [AT]
// INT 1a,06 - Set Real-Time Clock Alarm [AT]
// INT 1a,07 - Cancel Real-Time Clock Alarm [AT]

#endif
