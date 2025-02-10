#include "demo_rectangles.h"

#include <stdio.h>
#include <stdlib.h>

#include "../HW/hw_constants_graphics.h"

#include "../HGA/hga_constants.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"
#include "../HGA/hga_colours.h"
#include "../HGA/hga_rectangle.h"
#include "../HGA/hga_filled_rectangle.h"
#include "../HGA/hga_fast_line.h"


#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define INFO_ADAPTER        "INFO: Adapter Type = %s\n"
#define PRESS_ENTER         "Press <Enter>"

int rectangles(int argc, char** argv) {
    // 1. confirm appropriate graphics adapter presenddt
    uint8_t adapter_type = hga_detect_adapter();
    if (adapter_type < HW_VIDEO_ADAPTER_HGA) {
        fprintf(stderr, ERR_GRAPHICS);
        fprintf(stderr, ERR_GRAPHICS_INFO, argv[0]);
        return EXIT_FAILURE;
    }
    printf(INFO_ADAPTER, hw_video_adapter_names[adapter_type]);
    // 2. wait for ENTER key and switch to HGA graphics mode
    printf(PRESS_ENTER);
    getchar();
    hga_graphics_mode();
    hga_select_display_buffer(HGA_BUFFER_1);
    hga_cls(HGA_BUFFER_1);
    //
    getchar();

    /*hga_hline(HGA_BUFFER_1, 200, 98, 200 + 100, 98 + 100, HGA_WHITE);
    hga_rectangle(HGA_BUFFER_1, 200, 100, 100, 100, HGA_WHITE);
    hga_hline(HGA_BUFFER_1, 210, 108, 210 + 100, 108 + 100, HGA_WHITE);
    hga_rectangle(HGA_BUFFER_1, 210, 110, 100, 100, HGA_WHITE);
    hga_hline(HGA_BUFFER_1, 220, 118, 220 + 100, 118 + 100, HGA_WHITE);
    hga_rectangle(HGA_BUFFER_1, 220, 120, 100, 100, HGA_WHITE);
    hga_hline(HGA_BUFFER_1, 230, 128, 230 + 100, 128 + 100, HGA_WHITE);
    hga_rectangle(HGA_BUFFER_1, 230, 130, 100, 100, HGA_WHITE);
    hga_hline(HGA_BUFFER_1, 240, 138, 240 + 100, 138 + 100, HGA_WHITE);
    hga_rectangle(HGA_BUFFER_1, 240, 140, 100, 100, HGA_WHITE);
    hga_hline(HGA_BUFFER_1, 250, 148, 250 + 100, 148 + 100, HGA_WHITE);
    hga_rectangle(HGA_BUFFER_1, 250, 150, 100, 100, HGA_WHITE);
    hga_hline(HGA_BUFFER_1, 260, 158, 260 + 100, 158 + 100, HGA_WHITE);
    hga_rectangle(HGA_BUFFER_1, 260, 160, 100, 100, HGA_WHITE);
    */
    hga_hline(HGA_BUFFER_1, 270, 168, 270 + 100, 168 + 100, HGA_WHITE);
    hga_vline(HGA_BUFFER_1, 268, 170, 268 + 100, 170 + 100, HGA_WHITE);
    hga_filled_rectangle(HGA_BUFFER_1, 270, 170, 100, 100, HGA_WHITE);

    //hga_rectangle(HGA_BUFFER_1, 110, 50, 14, 14, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 120, 51, 14, 14, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 130, 52, 14, 14, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 140, 53, 14, 14, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 150, 54, 14, 14, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 160, 55, 14, 14, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 170, 56, 14, 14, HGA_WHITE);
    hga_filled_rectangle(HGA_BUFFER_1, 1, 10, 14, 14, HGA_WHITE);

    //hga_rectangle(HGA_BUFFER_1, 51, 26, 4, 4, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 52, 31, 4, 4, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 53, 36, 4, 4, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 54, 41, 4, 4, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 55, 46, 4, 4, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 56, 51, 4, 4, HGA_WHITE);
    //hga_rectangle(HGA_BUFFER_1, 57, 56, 4, 4, HGA_WHITE);
    hga_filled_rectangle(HGA_BUFFER_1, 58, 61, 4, 4, HGA_WHITE);
    // 12. draw black rectangles
    getchar();
    hga_fill_vram_buffer(HGA_BUFFER_1, 0xAA);
    getchar();
    hga_hline(HGA_BUFFER_1, 270, 168, 270 + 100, 168 + 100, HGA_BLACK);
    hga_vline(HGA_BUFFER_1, 268, 170, 268 + 100, 170 + 100, HGA_BLACK);
    hga_filled_rectangle(HGA_BUFFER_1, 270, 170, 100, 100, HGA_BLACK);
    hga_filled_rectangle(HGA_BUFFER_1, 1, 10, 14, 14, HGA_BLACK);
    hga_filled_rectangle(HGA_BUFFER_1, 58, 61, 4, 4, HGA_BLACK);
    // wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();

    return EXIT_SUCCESS;
}
