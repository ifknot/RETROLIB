/**
 *
 *  @brief
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef HGA_DETECT_ADAPTER_H
#define HGA_DETECT_ADAPTER_H

#include <stdint.h>

#include "../HW/hw_constants_graphics.h"
#include "../HW/hw_detect_crtc.h"

#include "hga_constants.h"

/**
* If an MDA card is detected, however, you can also distinguish between an MDA card and the Hercules cards.
* To do this, the vertical sync bit is sampled and then for a certain time interval, is continually and frequently re-sampled.
* + if no change is observed, the video card is assumed to be an MDA card.
* + if the vertical sync bit changes, the video card is one of the Hercules graphics cards.
* The specific card is identified by examining bits 4-6 of the status port:
* + a pattern of 001 indicates a Hercules Graphics Card Plus
* + a pattern of 101 indicates a Hercules In-Color Card
* + any other pattern is a Hercules Graphics Card.
*
* Reading from port 03BAh returns vertical sync in bit 7, and a card ID in bits 6-4:
* 000: Hercules
* 001: Hercules Plus
* 101: Hercules InColor
* 111: Unknown clone
*
* @note Only trust this ID once you've checked that there's a vertical sync present; if bit 7 is the same for 32768 reads in succession, then there isn't.
* @note Some Hercules cards support a light pen. Bit 1 of port 3BAh returns the lightpen sensor status; any write to port 3BBh resets the light pen status.
*
* return:
* 0 Unknown
* 2 Hercules
* 3 Hercules Plus
* 4 Hercules InColor
* 5 Hercules Clone
*/
uint8_t hga_detect_adapter();

#endif
