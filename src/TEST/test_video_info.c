#include "test_video_info.h"

#include <stdio.h>

#include "../HW/hw_constants_graphics.h"
#include "../HW/hw_detect_graphics_adapter.h"

int test_video_info() {
    uint16_t card1, card2, display = 1;
    card1 = card2 = HW_VIDEO_ADAPTER_UKNOWN;
    card1 = hw_detect_monochrome_adapter();
    card2 = hw_detect_colour_adapter();
    if (card1) {
            printf("\ndisplay adapter %i = %s\n", display++, hw_video_adapter_names[card1]);
    }
    if (card2) {
            printf("\ndisplay adapter %i = %s\n", display, hw_video_adapter_names[card2]);
    }
    if(!(card1 | card2)) {
        printf("\n tested for...");
        for(int i = HW_VIDEO_ADAPTER_MDA; i <= HW_VIDEO_ADAPTER_SVGA; ++i) {
            printf("\n? %s", hw_video_adapter_names[i]);
        }
        printf("\nERROR unknown video adapter\n");
    }

}
