#ifndef INCOLOR_VIDEO_MODE_H
#define INCOLOR_VIDEO_MODE_H

#include <stdint.h>

void incolor_video_mode(const uint8_t* presets, uint8_t flags_config, uint8_t flags_ctrl);

/**
*  @brief enter colour text mode interpret MDA attributes as "CGA like" colour attributes
*/
inline void incolor_text_mode() {
    incolor_video_mode(incolor_crtc_presets_text_mode, incolor_text_config_default, incolor_text_ctrl_default);
}

/**
*  @brief enter Hercules InColor 4 plane colour graphics mode with 2 frame buffers.
*/
inline void incolor_graphics_mode() {
    incolor_video_mode(incolor_crtc_presets_graphics_mode, incolor_graphics_config_default, incolor_graphics_ctrl_default);
}

#endif
