#include "demo_pixels.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../HW/hw_constants_graphics.h"
#include "../BIOS/bios_tools_timer.h"
#include "../BIOS/bios_timer_io_services.h"

#include "../HGA/hga_constants.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"
#include "../HGA/hga_pixel.h"

#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define INFO_ADAPTER        "INFO: Adapter Type = %s\n"
#define PRESS_ENTER         "Press <Enter>"

int pixels(int argc, char** argv) {
    // 0. set up variables
    uint8_t adapter_type = hga_detect_adapter();
    bios_ticks_since_midnight_t t1, t2;
    // 1. confirm appropriate graphics adapter presenddt
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
    // time pixels
    hga_cls(HGA_BUFFER_1);
    getchar();

    bios_read_system_clock(&t1);
    for(uint16_t y = 0; y < HGA_SCREEN_Y_MAX; ++y) {
        for(uint16_t x = 0; x < HGA_SCREEN_X_MAX; ++x) {
            hga_plot_pixel_lookup_table(HGA_BUFFER_1, x, y, HGA_WHITE);
        }
    }
    bios_read_system_clock(&t2);
    printf("hga_plot_pixel_lookup_table 250560 white pixels = %f\n", bios_tools_timer_ticks_to_seconds(t2 - t1));

    bios_read_system_clock(&t1);
    for(uint16_t y = 0; y < HGA_SCREEN_Y_MAX; ++y) {
        for(uint16_t x = 0; x < HGA_SCREEN_X_MAX; ++x) {
            hga_plot_pixel_lookup_table(HGA_BUFFER_1, x, y, HGA_BLACK);
        }
    }
    bios_read_system_clock(&t2);
    printf("hga_plot_pixel_lookup_table 250560 black pixels = %f\n", bios_tools_timer_ticks_to_seconds(t2 - t1));

    bios_read_system_clock(&t1);
    for(uint16_t y = 0; y < HGA_SCREEN_Y_MAX; ++y) {
        for(uint16_t x = 0; x < HGA_SCREEN_X_MAX; ++x) {
            hga_plot_pixel_calculate(HGA_BUFFER_1, x, y, HGA_WHITE);
        }
    }
    bios_read_system_clock(&t2);
    printf("hga_plot_pixel_calculate 250560 white pixels = %f\n", bios_tools_timer_ticks_to_seconds(t2 - t1));

    bios_read_system_clock(&t1);
    for(uint16_t y = 0; y < HGA_SCREEN_Y_MAX; ++y) {
        for(uint16_t x = 0; x < HGA_SCREEN_X_MAX; ++x) {
            hga_plot_pixel_calculate(HGA_BUFFER_1, x, y, HGA_BLACK);
        }
    }
    bios_read_system_clock(&t2);
    printf("hga_plot_pixel_calculate 250560 white pixels = %f\n", bios_tools_timer_ticks_to_seconds(t2 - t1));

    //  wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();

    return 0;
}
