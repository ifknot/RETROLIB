/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      25.05.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef MDA_TOOLBOX_H
#define MDA_TOOLBOX_H

#include "mda_constants.h"

namespace mda {

	void fill_vram_buffer(char ascii, attribute_t attrib);

	void fill_vram_attributes(attribute_t attrib);

	void fill_vram_characters(char ascii);

	inline void cls() {
		fill_vram_buffer(' ', attribute::normal);
	}

}

#endif