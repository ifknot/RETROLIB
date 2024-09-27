/**
 *
 *  @brief     INT 10 - Video BIOS Services
 *  @note	   Warning: Some BIOS implementations have a bug that causes register
 *			   BP to be destroyed.   It is advisable to save BP before a call to
 *		       Video BIOS routines on these systems.
 *  @author    Jeremy Thornton
 *  @date      3.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef BIOS_VIDEO_SERVICES_H
#define	BIOS_VIDEO_SERVICES_H

#include "bios_video_services_constants.h"
#include "bios_video_services_types.h"

namespace bios {
	
	// INT 10,0 - Set video mode
	void set_video_mode(uint8_t mode);

	// INT 10,1 - Set cursor type
	// INT 10,2 - Set cursor position
	// INT 10,3 - Read cursor position
	// INT 10,4 - Read light pen
	// INT 10,5 - Select active display page
	// INT 10,6 - Scroll active page up
	// INT 10,7 - Scroll active page down
	// INT 10,8 - Read character and attribute at cursor
	// INT 10,9 - Write character and attribute at cursor
	// INT 10,A - Write character at current cursor
	// INT 10,B - Set color palette
	// INT 10,C - Write graphics pixel at coordinate
	// INT 10,D - Read graphics pixel at coordinate
	// INT 10,E - Write text in teletype mode
	
	// INT 10,F - Get current video state
	void get_video_state(video_state_t* state);

	// INT 10,10 - Set/get palette registers (EGA/VGA)
	// INT 10,11 - Character generator routine (EGA/VGA)
	
	// INT 10,12 - Video subsystem configuration (EGA/VGA)
	uint8_t video_subsystem_configuration(uint8_t request, uint8_t setting, video_subsystem_config_t* config = 0);
	
	// INT 10,13 - Write string (BIOS after 1/10/86)
	// INT 10,14 - Load LCD char font (convertible)
	// INT 10,15 - Return physical display parms (convertible)
	// INT 10,1A - Video Display Combination (VGA)
	// INT 10,1B - Video BIOS Functionality/State Information (MCGA/VGA)
	// INT 10,1C - Save/Restore Video State  (VGA only)
	// INT 10,FE - Get DESQView/TopView Virtual Screen Regen Buffer
	// INT 10,FF - Update DESQView/TopView Virtual Screen Regen Buffer

}

#endif