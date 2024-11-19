/**
 *
 *  @brief
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      4.11.2023
 *  @copyright Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef	BIOS_VIDEO_SERVICES_TYPES
#define BIOS_VIDEO_SERVICES_TYPES

#include <stdint.h>

#include "bios_video_services_constants.h"


/**
* Video BIOS get video mode return structure.
*/
typedef struct {
	uint8_t			columns;
	uint8_t			mode;
	uint8_t			page;
} bios_video_state_t;

/**
* INT 10,12 - Video Subsystem Configuration (EGA/VGA)
* AH = 12h
* BL = 10  return video configuration informationreturn video configuration information
*
* on return:
* BH = 0 if color mode in effect
*    = 1 if mono mode in effect
* BL = 0 if 64k EGA memory
*    = 1 if 128k EGA memory
*    = 2 if 192k EGA memory
*    = 3 if 256k EGA memory
* CH = feature bits
* CL = switch settings
*/
typedef struct {
	uint8_t		color_mode;
	uint8_t		ega_memory;
	uint8_t		feature_bits;
	uint8_t		switch_settings;
} bios_video_subsystem_config_t;

#endif
