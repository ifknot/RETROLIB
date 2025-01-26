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

#include "../HGA/hga_line.h"


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

    hga_rectangle(HGA_BUFFER_1, 200, 100, 100, 100, HGA_WHITE);
    //hga_hline(HGA_BUFFER_1, 0, j + 1, 0, 0, HGA_WHITE);


    // wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();

    return EXIT_SUCCESS;
}
