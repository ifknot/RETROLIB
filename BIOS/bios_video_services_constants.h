/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      4.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef	BIOS_VIDEO_SERVICES_CONSTANTS_H
#define BIOS_VIDEO_SERVICES_CONSTANTS_H

#define BIOS_VIDEO_SERVICES                 10h

/**
* BIOS VIDEO SERVICES AH FOR INT 10 - Video BIOS Services
*/

#define SET_VIDEO_MODE                      0

//SET_CURSOR_TYPE
//SET_CURSOR_POSITION
//READ_CURSOR_POSITION
//READ_LIGHT_PEN
//SELECT_ACTIVE_DISPLAY_PAGE
//SCROLL_ACTIVE_PAGE_UP
//SCROLL_ACTIVE_PAGE_DOWN
//READ_CHARACTER_AND_ATTRIBUTE_AT_CURSOR
//WRITE_CHARACTER_AND_ATTRIBUTE_AT_CURSOR
//WRITE_CHARACTER_AT_CURRENT_CURSOR
//SET_COLOR_PALETTE
//WRITE_GRAPHICS_PIXEL_AT_COORDINATE
//READ_GRAPHICS_PIXEL_AT_COORDINATE
//WRITE_TEXT_IN_TELETYPE_MODE

#define GET_CURRENT_VIDEO_STATE             0Fh

//SET/GET_PALETTE_REGISTERS					// (EGA/VGA)
//CHARACTER_GENERATOR_ROUTINE				// (EGA/VGA)

#define VIDEO_SUBSYSTEM_CONFIGURATION		12h	    // (EGA/VGA)

//WRITE_STRING								// (BIOS_AFTER_1/10/86)
//LOAD_LCD_CHAR_FONT						// (CONVERTIBLE)
//RETURN_PHYSICAL_DISPLAY_PARMS				// (CONVERTIBLE)
//VIDEO_DISPLAY_COMBINATION					// (VGA)
//VIDEO_BIOS_STATE_INFORMATION				// (MCGA/VGA)
//SAVE/RESTORE_VIDEO_STATE					// (VGA_ONLY)

//GET_DESQVIEW_VIRTUAL_SCREEN_REGEN_BUFFER
//UPDATE_DESQVIEW_VIRTUAL_SCREEN_REGEN_BUFFER

namespace bios {

    /**
    * BIOS Video Subsytem Configuration Sub-systems:
    */
    enum video_subsytem_t {
        RETURN_VIDEO_CONFIGURATION_INFORMATION = 0x10,
        SELECT_ALTERNATE_PRINT_SCREEN_ROUTINE = 0x20,
        SELECT_SCAN_LINES_FOR_ALPHANUMERIC_MODES = 0x30,
        SELECT_DEFAULT_PALETTE_LOADING,
        CPU_ACCESS_TO_VIDEO_RAM,
        GRAY_SCALE_SUMMING,
        CURSOR_EMULATION,
        PS2_VIDEO_DISPLAY_SWITCHING,
        VIDEO_REFRESH_CONTROL
    };

	enum video_mode_t {
        CGA_TEXT_MONOCHROME_40X25 = 0,              // 00  40x25 B/W text (CGA,EGA,MCGA,VGA)
        CGA_TEXT_16_COLOUR_40X25,                   // 01  40x25 16 color text (CGA,EGA,MCGA,VGA)
        CGA_TEXT_16_GRAY_80X25,                     // 02  80x25 16 shades of gray text (CGA,EGA,MCGA,VGA)
        CGA_TEXT_16_COLOUR_80x25,                   // 03  80x25 16 color text (CGA,EGA,MCGA,VGA)
        CGA_GRAPHICS_4_COLOUR_320x200,              // 04  320x200 4 color graphics (CGA,EGA,MCGA,VGA)
        CGA_GRAPHICS_4_GRAY_320x200,                // 05  320x200 4 gray graphics (CGA,EGA,MCGA,VGA)
        CGA_GRAPHICS_MONOCHROME_640X200,            // 06  640x200 B/W graphics (CGA,EGA,MCGA,VGA)
        MDA_TEXT_MONOCHROME_80X25,                  // 07  80x25 Monochrome text (MDA,HERC,EGA,VGA)
        PCJR_GRAPHICS_16_COLOUR_160X200,            // 08  160x200 16 color graphics (PCjr)
        PCJR_GRAPHICS_16_COLOUR_320X200,            // 09  320x200 16 color graphics (PCjr)
        PCJR_GRAPHICS_4_COLOUR_640X200,             // 0A  640x200 4 color graphics (PCjr)
        EGA_RESERVED1,                              // 0B  Reserved (EGA BIOS function 11)
        EGA_RESERVED2,                              // 0C  Reserved (EGA BIOS function 11)
        EGA_GRAPHICS_16_COLOUR_320X200,             // 0D  320x200 16 color graphics (EGA,VGA)
        EGA_GRAPHICS_16_COLOUR_640X200,             // 0E  640x200 16 color graphics (EGA,VGA)
        EGA_GRAPHICS_MONOCHROME_640X350,            // 0F  640x350 Monochrome graphics (EGA,VGA)
        EGA_GRAPHICS_16_COLOUR_640X350,             // 10  640x350 16 color graphics (EGA or VGA with 128K)
        MCGA_GRAPHICS_MONOCHROME_640X480,           // 11  640x480 B/W graphics (MCGA,VGA)
        VGA_GRAPHICS_16_COLOUR_640X480,             // 12  640x480 16 color graphics (VGA)
        MCGA_GRAPHICS_256_COLOUR_320X200            // 13  320x200 256 color graphics (MCGA,VGA)
        /*
        8x  EGA, MCGA or VGA ignore bit 7, see below
        9x  EGA, MCGA or VGA ignore bit 7, see below

        if AL bit 7=1, prevents EGA,MCGA & VGA from clearing display
        N.B. function updates byte at 40:49;  bit 7 of byte 40:87
        */
	};

    const char video_mode_names[20][64] = {
        "00  40x25 B/W text (CGA)",
        "01  40x25 16 color text (CGA)",
        "02  80x25 16 shades of gray text (CGA)",
        "03  80x25 16 color text (CGA)",
        "04  320x200 4 color graphics (CGA)",
        "05  320x200 4 color graphics (CGA)",
        "06  640x200 B/W graphics (CGA)",
        "07  80x25 Monochrome text (MDA,HERC)",
        "08  160x200 16 color graphics (PCjr)",
        "09  320x200 16 color graphics (PCjr)",
        "0A  640x200 4 color graphics (PCjr)",
        "0B  Reserved (EGA BIOS function 11)",
        "0C  Reserved (EGA BIOS function 11)",
        "0D  320x200 16 color graphics (EGA)",
        "0E  640x200 16 color graphics (EGA)",
        "0F  640x350 Monochrome graphics (EGA)",
        "10  640x350 16 color graphics (EGA)",
        "11  640x480 B/W graphics (MCGA)",
        "12  640x480 16 color graphics (VGA)",
        "13  320x200 256 color graphics (MCGA, VGA)"
    };

}

#endif