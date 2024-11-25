#ifndef HGA_VIDEO_MODE_PRESETS_H
#define HGA_VIDEO_MODE_PRESETS_H

#include <stdint.h>

/**
* use the config register 3BFh to enable text mode via the Hercules control register 3B8h
* bit 0 = 0 disable bit 1 of control register 03B8h
* bit 1 = 0 disable second 32k of RAM ("Full" mode)
*/
static const uint8_t hga_text_config_default = 0; // 0b00000000;

/**
* control register program text mode with screen off
* bit 1 = 0 set text mode
* bit 3 = 0 screen off
* bit 5 = 0 blink off
* bit 7 = 0 display page 1
*/
static const uint8_t hga_text_ctrl_default = 0; // 0b00000000;

/**
* use the config register 3BFh to enable graphics mode via the Hercules control register 3B8h
* bit 0 = 0 enable bit 1 of control register 03B8h
* bit 1 = 1 enable second 32k of RAM ("Full" mode)
*/
static const uint8_t hga_graphics_config_default = 3; //00000011b; // enable graphics "full" mode with 2 frame buffers

/**
* control register program graphics mode with screen off
* bit 1 = 1 set graphics mode
* bit 3 = 0 screen off
* bit 5 = 0 blink off
* bit 7 = 0 display page 1
*/
static const uint8_t hga_graphics_ctrl_default = 2; // 00000010b; // enter graphics mode display page 1

/**
* CRTC 6845 processor 16 preset reg:data value pairs for *text* mode
* @note values as per Hercules recommended
*/
static const uint8_t hga_crtc_presets_text_mode[32] = {
    // reg  value   meaning
        0,      97, // horizontal character seeded
        1,      80, // horizontal character displayed
        2,      82, // horizontal sync signal after character
        3,      15, // horizontal sync signal width
        4,      25, // vertical character seeded
        5,       6, // vertical character justified
        6,      25, // vertical character displayed
        7,      25, // vertical sync signal after character
        8,       2, // interlace mode
        9,      13, // scan-lines per line
        10,     11, // starting line of blinking cursor
        11,     12, // ending line of blinking cursor
        12,      0, // hi byte of screen page start address
        13,      0, // lo byte of screen page start address
        14,      0, // hi byte of blinking cursor character address
        15,      0  // lo byte of blinking cursor character address
    //  16      reserved
    //  17      reserved
};

/**
* CRTC 6845 processor 16 preset reg:data value pairs for *graphics* mode
* @note values as per Hercules recommended
*/
static const uint8_t hga_crtc_presets_graphics_mode[32] = {
    // reg  value   meaning
        0,      53, // horizontal character total 54 (0..53)
        1,      45, // horizontal character displayed
        2,      46, // horizontal sync signal after character
        3,       7, // horizontal sync signal width
        4,      91, // vertical character total 92 (0..91)
        5,       2, // vertical character justified 2 scan lines
        6,      87, // vertical character displayed rows (348 lines)
        7,      87, // vertical sync signal after character
        8,       2, // interlace mode
        9,       3, // scan-lines per char 4 (0..3)
        10,      0, // starting line of blinking cursor
        11,      0, // ending line of blinking cursor
        12,      0, // hi byte of screen page start address
        13,      0, // lo byte of screen page start address
        14,      0, // hi byte of blinking cursor character address
        15,      0  // lo byte of blinking cursor character address
    //  16      reserved
    //  17      reserved
};

#endif
