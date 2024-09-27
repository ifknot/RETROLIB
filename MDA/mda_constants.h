/**
 *
 *  @brief     MDA constants
 *  @details   The original MDA is a full-length 8-bit ISA card containing the display controller and a printer port.
 *  When present in a computer, it requires the following system resources:
 *  + 4k of RAM at address 0B0000h for its display buffer - this address is not completely decoded; the entire 32k from 0B0000h to 0B7FFFh is filled with repeats of this 4k area.
 *  + I/O addresses 03B0h-03BFh
 *  + The printer port uses IRQ7
 *  @url http://www.seasip.info/VintagePC/mda.html
 *  @author    Jeremy Thornton
 *  @date      20.07.2022
 *  @copyright © Jeremy Thornton, 2022. All right reserved.
 *
 */
#ifndef MDA_CONSTANTS_H
#define MDA_CONSTANTS_H

#include <stdint.h>

#define MDA_VIDEO_RAM_SEGMENT	0B000h
#define BYTES_PER_LINE 160 
#define CHARS_PER_SCREEN 2000
#define BYTES_PER_SCREEN 4000

namespace mda {

	typedef uint8_t attribute_t;

	namespace attribute {

		/**
		* The attribute bytes mostly behave like a bitmap:
		*
		* Bits 0-2: 1 => underline, other values => no underline.
		* Bit 3: High intensity.
		* Bit 7: Blink
		*
		* but there are eight exceptions:
		*
		* Attributes 00h, 08h, 80h and 88h display as black space.
		* Attribute 70h displays as black on green.
		* Attribute 78h displays as dark green on green. In fact, depending on timing and on the design of the monitor, it may have a bright green 'halo' where the dark green and bright green bits meet.
		* Attribute F0h displays as a blinking version of 70h (if blinking is enabled); as black on bright green otherwise.
		* Attribute F8h displays as a blinking version of 78h (if blinking is enabled); as dark green on bright green otherwise.
		*/
		enum attributes {
			invisible,
			underline,
			normal,
			bright = 0x0A,
			reverse = 0x70,
			blink = 0x82,
			bright_blink = 0x8A,
			reverse_bright = 0x7A,
			reverse_blink = 0xF0,
			reverse_bright_blink = 0xF8
		};

	}

}

#endif