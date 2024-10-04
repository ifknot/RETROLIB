/**
 *
 *  @file      cga_mode4_constants.h
 *  @brief
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      2.04.2022
 *  @copyright © Jeremy Thornton, 2022. All right reserved.
 *
 */
#ifndef CGA_MODE4_CONSTANTS_H
#define CGA_MODE4_CONSTANTS_H

#include <stdint.h>

#include "cga_constants.h"

 /**
  * Mode 4 macro definitions for assembly
  * 
  * Graphic Mode: 320x200, 2-bit Color - this was the most popular CGA mode used for graphics. 
  * The display resolution is 320x200 pixels with a pixel ratio of 1:1.2 (visually 320x240). 
  * Only four colors could be displayed on the screen at a time, and they were mostly fixed, which is why so many CGA games look the same.
  *
  * Everything is hard coded to shave a few clock cycles off the execution time
  * Using #define enables immediate addressing mode - in which the data operand is a part of the instruction itself
  * This matters on an 8088 where reg,imm is only 4 clock cycles as opposed to 12+EA for reg,mem
  * This advantage dwindles on the 286 (2:5) and 386 (2:4), vanishing on the 486(1:1) and beyond
  */
#define CGA_MODE4_SCREEN_X_MIN 0
#define CGA_MODE4_SCREEN_X_MAX 320
#define CGA_MODE4_SCREEN_Y_MIN 0
#define CGA_MODE4_SCREEN_Y_MAX 200
#define CGA_MODE4_SCREEN_Y_SCALE 0.625
#define CGA_MODE4_PIXELS_PER_BYTE		4

/**
 *  @namespace cga::mode4::colour
 *  @brief  Three (0, 1, 2) colour palettes are available in mode 4  
 *  @details
 *		Palette 0				Palette 0 high intensity
 *	#	palette colour		  RGB		palette colour		  RGB 
 *	0			default							default
 *	1		2	green		#00AA00		10		light green	#55FF55
 *	2		4	red			#AA0000		12		light red	#FF5555
 *	3		6	brown		#AA5500		14		yellow		#FFFF55
 * 
 *		Palette 1				Palette 1 high intensity
 *	#	palette colour		  RGB		palette colour		  RGB 
 *	0			default							default
 *	1		3	cyan		#00AAAA		11		light cyan	#55FFFF
 *	2		5	magenta		#AA00AA		13		light mag	#FF55FF
 *	3		7	light gray	#AAAAAA		15		white		#FFFFFF
 * 
 *  @note A third 320×200 4-color palette is achieved by disabling the composite color burst bit while in graphics mode. 
 *  This is what IBM BIOS Mode 5 does
 * 
 *		Palette 2				Palette 2 high intensity
 *	#	palette colour		  RGB		palette colour		  RGB 
 *	0			default							default
 *	1		3	cyan		#00AAAA		11		light cyan	#55FFFF
 *	2		4	red			#AA0000		13		light red	#FF5555
 *	3		7	light gray	#AAAAAA		15		white		#FFFFFF
 *  
 *  @note In 320×200 graphics mode, the background & border colour can be changed from black (default) to any of the other 15 colors of the CGA palette. 
 *  This allows for some variation, as well as flashing effects, as the background color can be changed without having to redraw the screen (i.e. without changing the contents of the video RAM.)
 */

typedef enum {
  black	= 0x00,
  green	= 0x40,
  red		= 0x80,
  yellow	= 0xC0
} cga_colour_palette_0;

typedef enum {
  black = 0x00,
  cyan = 0x40,
  magenta = 0x80,
  white = 0xC0
} cga_colour_palette_1;

typedef enum {
  black	= 0x00,
  cyan	= 0x40,
  red		= 0x80,
  white	= 0xC0
} cga_colour_palette_2;

#endif
