/**
 *
 *  @file      bios_video_services.cpp
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      4.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#include "bios_video_services.h"

#include "bios_video_services_constants.h"

namespace bios {

	/**
	* @brief INT 10,0 - Set Video Mode
	* AH = 00
	* AL = video_mode_t
	* @see bios_video_services_constants.h enum type
	*/
	void set_video_mode(uint8_t mode) {
		__asm {
			.8086
			mov		al, mode
			mov		ah, SET_VIDEO_MODE
			int		BIOS_VIDEO_SERVICES
		}
	}

	/**
	* @brief INT 10,F - Get Video State
	* AH = 0F
	*
	* on return:
	* AH = number of screen columns
	* AL = mode currently set (see VIDEO MODES)
	* BH = current display page
	*
	* @note - video modes greater than 13h on EGA, MCGA and VGA indicate
	* INT 10,0 was called with the high bit of the mode (AL) set
	* to 1, meaning the display does not need cleared
	* @note - function returns byte value at 40:49;  On EGA, MCGA and
	* VGA bit 7 of register AL is determined by bit 7 of BIOS Data
	* Area byte 40:87.   This bit is usually set by INT 10,0
	* with bit 7 of the requested mode (in AL) set to 1
	*/
	void get_video_state(video_state_t* state) {
		__asm {
			.8086
			mov		ah, GET_CURRENT_VIDEO_STATE
			int		BIOS_VIDEO_SERVICES
			lds		di, state
			mov[di], ah; number of screen columns
			mov[di + 1], al; video mode
			mov[di + 2], bh; display page
		}
	}

	/**
	* @brief INT 10,12 - Video Subsystem Configuration (EGA/VGA)
	* -----------------------------------------------------------------------------------------------------
	* AH = 12h
	* BL = 10  return video configuration information
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
	*
	* Info: This obtains miscellaneous information about the EGA switch
	*       settings and the current values of the "feature bits" as read
	*       through those rarely-used RCA connectors on some EGA cards.
	*  @note If upon return from this call, BL>4, then must be running on a CGA or MDA (not an EGA or VGA).
	* -----------------------------------------------------------------------------------------------------
	* BL = 20  select alternate print screen routine
	*
	* Info: This redirects the INT 05H (print screen) vector to a handler within the EGA/VGA ROM-BIOS.
	*
	* On older PCs, XTs, and ATs, the default ROM-BIOS print-screen handler stops printing after 25 lines.
	* The EGA/VGA handler correctly prints the number of lines specified in the byte at 0040:0084 in the BIOS Data Area.
	*
	* @note Most EGAs and VGAs set this automatically.
	* However, it is good practice to use this fn whenever change the number of video lines that are displayed in text mode.
	* -----------------------------------------------------------------------------------------------------
	* BL = 30  select scan lines for alphanumeric modes
	*    AL = 0  200 scan lines
	*       = 1  350 scan lines
	*       = 2  400 scan lines
	*
	* on return: AL = 12 if a VGA is active
	*
	* Info: This updates bits 0-3 in the EgaMiscInfo2Rec (at 0040:0088) and bits 4 and 7 of the VgaFlagsRec (at 0040:0089).
	* Then, on the next INT 10H 00H call to switch to a text mode, the specified resolution will be in effect.
	* -----------------------------------------------------------------------------------------------------
	* @note Compatibility: VGA
	* BL = 31  select default palette loading
	*    AL = 0 enable default palette loading
	*       = 1 disable default palette loading
	*
	* on return: AL = 12 if a valid value was passed in AL
	*
	* Info: This changes how the BIOS handles the color palette on subsequent calls to INT 10H 00H (set video mode).
	* It updates bit 3 of the VgaFlagsRec (at 0040:0089).
	*
	* @note When automatic palette loading is enabled (the default setting), the BIOS reprograms the ATC
	* with the default palette values each time the video mode is reset.
	* User can modify the VgaSavePtr2Rec to setup own palette to be used the default.
	*
	* @note When disabled, the current palette setting are left in place on mode changes.
	* -----------------------------------------------------------------------------------------------------
	* @note Compatibility: VGA
	* BL = 32  CPU access to video RAM
	*    AL = 0  enable CPU access to video RAM and I/O ports
	*       = 1  disable CPU access to video RAM and I/O ports
	*
	* on return: AL = 12h if a valid value was passed in AL
	*
	* Info: You can use this to suppress CPU access to video RAM and I/O ports on VGA-compatible displays.  It updates bit 0 of the
	* VgaFlagsRec (at 0040:0089).
	* -----------------------------------------------------------------------------------------------------
	* * @note Compatibility: VGA
	* BL = 33  Gray scale summing
	*    AL = 0  enable gray scale summing
	*       = 2  disable gray scale summing
	*
	* on return: AL = 12 if a valid value was passed in AL
	*
	* Info: This updates bit 1 of the VgaFlagsRec (at 0040:0089).
	* On the next mode reset (via INT 10H 00H), if that bit is set, then the BIOS will automatically
	* reprogram the palette to display only shades of gray
	* -----------------------------------------------------------------------------------------------------
	* BL = 34  cursor emulation
	*    AL = 0  enable cursor emulation
	*       = 1  disable cursor emulation
	*
	* on return: AL = 12
	* -----------------------------------------------------------------------------------------------------
	* BL = 35  PS/2 video display switching
	*    AL = 0 initial adapter video off
	*       = 1 initial planar video on
	*       = 2 switch active video off
	*       = 3 switch inactive video on
	*    ES:DX pointer to 128 byte save area (when AL = 0, 2 or 3)
	*
	* on return: AL = 12  if a valid value was passed in AL
	*
	* Info: ON PS/2 systems, this provides a means to switch between the built-in VGA (on the motherboard)
	* and another, incompatible display system.(!)
	* -----------------------------------------------------------------------------------------------------
	* BL = 36  video refresh control
	*    AL = 0 enable refresh
	*       = 1 disable refresh
	*
	* on return: AL = 12 if a valid value was passed in AL
	* Info: This modifies bit 5 of the VGA's Sequencer Clocking Mode Register.
	* Can draw complex graphics faster while refresh is disabled (it avoids some enforced wait states).
	* @note Be sure to re-enable refresh when finished updating video memory.
	* -----------------------------------------------------------------------------------------------------
	*/
	uint8_t video_subsystem_configuration(uint8_t request, uint8_t setting, video_subsystem_config_t* config) {
		uint8_t error;
		switch (request) {
		case RETURN_VIDEO_CONFIGURATION_INFORMATION:
			if (config) {
				config->color_mode = config->ega_memory = config->feature_bits = config->switch_settings = 0;
				__asm {
					.8086
					push	bp
					pushf

					mov		ah, VIDEO_SUBSYSTEM_CONFIGURATION
					mov		bl, request
					int		BIOS_VIDEO_SERVICES
					mov		error, al
					lds		di, config
					mov[di], bh									; color_mode(0 = colour, 1 = mono)
					mov[di + 1], bl								; EGA memory BL(0 = 64k, 2 = 128k, 2 = 192k, 3 = 256k)
					mov[di + 2], ch								; feature_bits(values of those RCA connectors)
					mov[di + 3], cl								; switch_settings
					
			EXIT:	popf
					pop		bp
				}
			}
			break;
		case SELECT_ALTERNATE_PRINT_SCREEN_ROUTINE:
		case SELECT_SCAN_LINES_FOR_ALPHANUMERIC_MODES:
		case SELECT_DEFAULT_PALETTE_LOADING:
		case CPU_ACCESS_TO_VIDEO_RAM:
		case GRAY_SCALE_SUMMING:
		case CURSOR_EMULATION:
		case PS2_VIDEO_DISPLAY_SWITCHING:
		case VIDEO_REFRESH_CONTROL:
			__asm {
				.8086
				push	bp
				pushf

				mov		ah, VIDEO_SUBSYSTEM_CONFIGURATION
				mov		bl, request
				mov		al, setting
				int		BIOS_VIDEO_SERVICES
				mov		error, al
			
				popf
				pop		bp
			}
			break;
		default:
			break;
		}
		return error;
	}

}