#include "test_video_info.h"

#include <stdio.h>

#include "../HW/hw_constants_graphics.h"
#include "../HW/hw_detect_graphics_adapter.h"

int test_video_info() {
    uint16_t mono, colour;
    mono = colour = HW_VIDEO_ADAPTER_UKNOWN;
    mono = hw_detect_monochrome_adapter();
    colour = hw_detect_colour_adapter();
    if (colour) {
            printf("\ncolour display = %s\n", hw_video_adapter_names[colour]);
    }
    if (mono) {
            printf("\nmonochrome display = %s\n", hw_video_adapter_names[mono]);
    }
    if(!(colour | mono)) {
        printf("\n tested for...");
        for(int i = HW_VIDEO_ADAPTER_MDA; i <= HW_VIDEO_ADAPTER_SVGA; ++i) {
            printf("\n? %s", hw_video_adapter_names[i]);
        }
        printf("\nERROR unknown video adapter\n");
    }

}
