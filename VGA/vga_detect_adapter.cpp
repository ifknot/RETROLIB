/**
 *
 *  @file      vga_detect_adapter.cpp
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      5.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#include "vga_detect_adapter.h"

#include "../gfx_constants.h"

#include "../../BIOS/bios_video_services.h"

namespace vga {

	/**
	* @brief detect VGA by attempting to use a VGA specific Video Subsystem Configuration (EGA/VGA) function
	* @details calling INT 10h,12h - Video Subsystem Configuration (EGA/VGA) with
	* BL = 32h  CPU access to video RAM
	* AL = 0  enable CPU access to video RAM and I/O ports
	* Is only compatible with VGA and will return AL = 12h if a valid value was passed in AL
	*
	* on return: AL = 12 if a valid value was passed in AL
	* 
	*/
	uint8_t detect_adapter() {
		return (bios::video_subsystem_configuration(bios::CPU_ACCESS_TO_VIDEO_RAM, 0) == 0x12) ? VIDEO_ADAPTER_VGA : VIDEO_ADAPTER_UKNOWN;
	}

}