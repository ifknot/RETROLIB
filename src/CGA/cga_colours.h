#ifndef CGA_COLOURS_H
#define CGS_COLOURS_H

 /**
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

typedef enum {            // palette 0    1    2
	cga_black = 0x00,    //         x    x    x
	cga_green = 0x40,    //         x
	cga_cyan = 0x40,    //              x    x  
	cga_red = 0x80,    //         x
	cga_magenta = 0x80     //              x
	cga_red = 0x80,    //              x
	cga_yellow = 0xC0,    //         x
	cga_white = 0xC0     //              x    x
} cga_colour_palette_t;

#endif
