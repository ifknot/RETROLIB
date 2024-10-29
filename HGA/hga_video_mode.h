/**
 *
 *  @brief
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      29.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef HGA_VIDEO_MODE_H
#define HGA_VIDEO_MODE_H

#include <stdint.h>

#include "hga_constants.h"
#include "hga_video_mode_presets.h"
//#include "hga_table_video_modes.h"

/**
*  @brief  program CRTC for video mode as described by the CRTC presets data table.
*  @details Since the Hercules card(s) use the same CRTC 6845 processor as the IBM card(s), the internal
*  registers of the 6845 and their meaning are identical to the IBM MDA.
*  The index register and data register are also located at the same address and the preset values
*  must be assigned to the various registers in the text and graphics modes respectively.
* */
void hga_video_mode(const uint8_t* presets, uint8_t flags_config, uint8_t flags_ctrl);

/**
*  @brief  enter text "half" mode single display frame (mimic MDA).
*/
void mda_text_mode() {
    hga_video_mode(hga_crtc_presets_text_mode, hga_text_config_default, hga_text_ctrl_default); // HGA_TABLE_TEXT_MODE_DEFAULTS
}

/**
*  @brief  enter Hercules graphics "full" mode with 2 frame buffers.
*/
void hga_graphics_mode() {
    hga_video_mode(hga_crtc_presets_graphics_mode, hga_graphics_config_default, hga_graphics_ctrl_default); // HGA_TABLE_GRAPHICS_MODE_DEFAULTS
}

#endif
