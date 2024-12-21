#ifndef INCOLOR_VIDEO_MODE_PRESETS_H
#define INCOLOR_VIDEO_MODE_PRESETS_H

#include <stdint.h>

#include "../HGA/hga_video_mode_presets.h"  // borrow the Hercules monochrome card presets

/**
* Hercules graphics cards do not include any form of BIOS extension,
* and the standard IBM BIOS treats them as an MDA.
* Consequently, to switch between text and graphics modes (or 80- and 90- column modes),
* the CRT controller has to be reprogrammed manually.
* These values should be set at the same time as the text/graphics bit in the Mode Control
* register, or the 80/90 column bit in the xMode register.
*
* Register	                 80col   90col   Graphics
* 00 Horizontal total	      61      6D       35
* 01 Horizontal displayed	  50	  5A	   2D
* 02 Horizontal blanking	  52	  5C	   2E
* 03 Sync width	              0F	  0F	   07
* 04 Vertical total	          19      19       5B
* 05 Vertical adjust	      06	  06	   02
* 06 Vertical displayed	      19	  19   	   57
* 07 Vertical sync position	  19	  19	   57
* 08 Interlace and skew	      02	  02	   02
* 09 Max raster	              0D	  0D	   03
* 0A Cursor start	          0B	  0B	   00
* 0B Cursor end	              0C	  0C	   00
*/


#endif
