#include "demo_lines.h"

#include <stdio.h>
#include <stdlib.h>

#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_fast_lines.h
#include "../HGA/hga_colours.h

#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "INFO: This version of %s requires an Hercules Graphics Adapter.\n"

int demo(int argc, char** argv) {
    // 1. confirm appropriate graphics adapter present
    if (hga_detect_adapter() < HW_VIDEO_ADAPTER_HGA) {
        fprintf(stderr, ERR_GRAPHICS);
        fprintf(stderr, ERR_GRAPHICS_INFO, argv[0]);
        return EXIT_FAILURE;
    }
    // 2. switch to HGA graphics mode 
    hga_graphics_mode();
    hga_select_display_buffer((char)HGA_BUFFER_1);
    hga_cls(HGA_BUFFER_1);

    hga_fast_vline(HGA_BUFFER_1, 10, 10, 10, 100, HGA_WHITE) {
    
    //7. wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();
    
    return EXIT_SUCCESS;
}
