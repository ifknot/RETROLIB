/**
 * @brief
 * @details
 *
 * clock updates at 1193180/65536 (about 18.2) ticks per second.
 * counts per second ? 18
 * counts per minute ? 1092
 * counts per hour   ? 65543
 * counts per day    ? 1573040
 */
#ifndef BIOS_TIMER_IO_CONSTANTS_H
#define BIOS_TIMER_IO_CONSTANTS_H

#define BIOS_CLOCK_SERVICES			1Ah
#define BIOS_READ_SYSTEM_CLOCK_COUNTER	0
#define BIOS_SET_SYSTEM_CLOCK_COUNTER	1

#define TICKS_PER_SECOND			18.206
#define TICKS_PER_24HR				1800B0h

#endif
