#include "demo_lines.h"

#include <stdio.h>
#include <stdlib.h>

#include "../HGA/hga_detect_adapter.h"

#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "INFO: This version of %s requires an Hercules Graphics Adapter.\n"

int demo(int argc, char** argv) {
    // 1. confirm appropriate graphics adapter present
    if (hga_detect_adapter() < HW_VIDEO_ADAPTER_HGA) {
        fprintf(stderr, ERR_GRAPHICS);
        fprintf(stderr, ERR_GRAPHICS_INFO, argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
