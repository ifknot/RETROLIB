/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      26.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef HGA_ATTRIBUTES_H
#define HGA_ATTRIBUTES_H

namespace hga {

	enum text_attributes_t {
		BLACK_SPACE = 0,
		GREEN_ON_BLACK_UNDERSCORE,
		GREEN_ON_BLACK,
		BRIGHT_GREEN_ON_BLACK_UNDERSCORE = 9,
		BRIGHT_GREEN_ON_BLACK,
		BLACK_ON_GREEN = 112,
		DARK_GREEN_ON_GREEN,
		BLINK_BLACK_ON_GREEN = 240,
		BLINK_DARK_GREEN_ON_GREEN = 248,
	};

}

#endif