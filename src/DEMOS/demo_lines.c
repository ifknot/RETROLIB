#include "demo_lines.h"

#include <stdio.h>
#include <stdlib.h>

#include "../HW/hw_constants_graphics.h"

#include "../HGA/hga_constants.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"
#include "../HGA/hga_fast_line.h"
#include "../HGA/hga_colours.h"

#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define INFO_ADAPTER        "INFO: Adapter Type = %s\n"
#define PRESS_ENTER         "Press <Enter>"

int demo(int argc, char** argv) {
    // 1. confirm appropriate graphics adapter present
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
    getchar();
    // 3. draw white lines
    /*
    int j = 1;
    for(int i = 10; i < 100; i+=1) {
        hga_fast_vline(HGA_BUFFER_1, i, j, i, j + 174, HGA_WHITE);
        j+=1;
    }
    getchar();
    // 4. draw black lines
    j = 1;
    for(int i = 10; i < 100; i+=2) {
        hga_fast_vline(HGA_BUFFER_1, i, j + 1, i, j + 173, HGA_BLACK);
        j+=2;
    }
    */

    hga_fast_hline(HGA_BUFFER_1, 0, 0, 0, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 1, 1, 1, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 2, 2, 2, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 3, 3, 3, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 4, 4, 4, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 5, 5, 5, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 6, 6, 6, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 7, 7, 7, 0, HGA_WHITE);

    hga_fast_hline(HGA_BUFFER_1, 0, 8, 0, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 1, 8, 1, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 2, 8, 2, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 3, 8, 3, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 4, 8, 4, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 5, 8, 5, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 6, 8, 6, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 7, 8, 7, 0, HGA_WHITE);

    hga_fast_hline(HGA_BUFFER_1, 0, 10, 7, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 1, 11, 7, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 2, 12, 7, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 3, 13, 7, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 4, 14, 7, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 5, 15, 7, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 6, 16, 7, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 7, 17, 7, 0, HGA_WHITE);

    hga_fast_hline(HGA_BUFFER_1, 0, 18, 7, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 0, 19, 6, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 0, 20, 5, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 0, 21, 4, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 0, 22, 3, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 0, 23, 2, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 0, 24, 1, 0, HGA_WHITE);
    hga_fast_hline(HGA_BUFFER_1, 0, 25, 0, 0, HGA_WHITE);

    //  wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();

    return EXIT_SUCCESS;
}
