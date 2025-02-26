#include "demo_bresenham.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../HW/hw_constants_graphics.h"

#include "../HGA/hga_constants.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"
#include "../HGA/hga_colours.h"
#include "../HGA/hga_bresenham_line.h"
#include "../HGA/hga_rectangle.h"

#include "../BIOS/bios_timer_io_services.h"

#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define INFO_ADAPTER        "INFO: Adapter Type = %s\n"
#define PRESS_ENTER         "Press <Enter>"

int bresenham(int argc, char** argv) {
    bios_ticks_since_midnight_t t1, t2;
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
    // white lines
    hga_cls(HGA_BUFFER_1);
    getchar();
    bios_read_system_clock(&t1);
    for(int j = 0; j < 20; ++j) {
        int y = 0;
        for(int i = 0; i < 50; ++i) {
            hga_bresenham_line(HGA_BUFFER_1, 100, 100, 200, 100 + y, HGA_WHITE);
            y +=2;
        }
    }
    bios_read_system_clock(&t2);
    printf("white ticks = %lu", t2 - t1);
    hga_rectangle(HGA_BUFFER_1, 100, 100, 100, 100, HGA_WHITE);
    getchar();
    // black lines
    hga_fill_vram_buffer(HGA_BUFFER_1, 0xFF);
    getchar();
    bios_read_system_clock(&t1);
    for(int j = 0; j < 20; ++j) {
        int y = 0;
        for(int i = 0; i < 50; ++i) {
            hga_bresenham_line(HGA_BUFFER_1, 100, 100, 200, 100 + y, HGA_BLACK);
            y +=2;
        }
    }
    bios_read_system_clock(&t2);
    printf("black ticks = %lu", t2 - t1);
    hga_rectangle(HGA_BUFFER_1, 100, 100, 100, 100, HGA_BLACK);
    //  wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();

    return 0;
}
