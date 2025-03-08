#include "demo_bresenham.h"

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
#include "../HGA/hga_colours.h"
#include "../HGA/hga_line.h"
#include "../HGA/hga_rectangle.h"
#include "../HGA/hga_pixel.h"


#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define INFO_ADAPTER        "INFO: Adapter Type = %s\n"
#define PRESS_ENTER         "Press <Enter>"

int bresenham(int argc, char** argv) {
    uint16_t x0 = HGA_SCREEN_X_MAX / 2;
    uint16_t y0 = HGA_SCREEN_Y_MAX / 2;
    uint16_t r = 100;
    int samples = 50;
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
    // white lines
    hga_cls(HGA_BUFFER_1);
    getchar();
    float angle;
    uint16_t x1, y1;
    /*
    // performance loop horizontal and vertical
    bios_read_system_clock(&t1);
    for(int i = 0; i < samples; ++i) {
        // horizontal x++
        hga_line(HGA_BUFFER_1, x0, y0, x0 + x0, y0, HGA_WHITE);
        // vertical y++
        hga_line(HGA_BUFFER_1, x0, y0, x0, y0 + y0, HGA_WHITE);
        // horizontal x--
        hga_line(HGA_BUFFER_1, x0, y0, x0 - x0, y0, HGA_WHITE);
        // vertical y--
        hga_line(HGA_BUFFER_1, x0, y0, x0, y0 - y0, HGA_WHITE);
    }
    bios_read_system_clock(&t2);
    printf("draw %i horiz & vert lines ticks=%li\n",samples * 4, t2 - t1);
    // performance loop diagonal
    bios_read_system_clock(&t1);
    for(int i = 0; i < 50; ++i) {
        // diagonal x++ y++
        hga_line(HGA_BUFFER_1, x0, y0, x0 + x0, y0 + y0, HGA_WHITE);
        // diagonal x-- y++
        hga_line(HGA_BUFFER_1, x0, y0, x0 - x0, y0 + y0, HGA_WHITE);
        // diagonal x++ y--
        hga_line(HGA_BUFFER_1, x0, y0, x0 + x0, y0 - y0, HGA_WHITE);
        // diagonal x-- y--
        hga_line(HGA_BUFFER_1, x0, y0, x0 - x0, y0 - y0, HGA_WHITE);
    }
    bios_read_system_clock(&t2);
    printf("draw %i diagonal lines ticks=%li\n",samples * 4, t2 - t1);
    //getchar();
    // 360 circle
    for (int a = 0; a < 360; a +=1) {
        angle = (a * 3.14159) / 180.0;
        x1 = x0 + r * cos(angle);
        y1 = y0 + r * sin(angle);
        //hga_plot_pixel(HGA_BUFFER_1, x1, y1, HGA_WHITE);
        hga_line(HGA_BUFFER_1, x0, y0, x1, y1, HGA_WHITE);
    }
    /*
    // black lines
    hga_fill_vram_buffer(HGA_BUFFER_1, 0xFF);
    getchar();

    for(int i = 0; i < 0; ++i) {
        // horizontal x++
        hga_line(HGA_BUFFER_1, x0, y0, x0 + x0, y0, HGA_BLACK);
        // vertical y++
        hga_line(HGA_BUFFER_1, x0, y0, x0, y0 + y0, HGA_BLACK);
        // horizontal x--
        hga_line(HGA_BUFFER_1, x0, y0, x0 - x0, y0, HGA_BLACK);
        // vertical y--
        hga_line(HGA_BUFFER_1, x0, y0, x0, y0 - y0, HGA_BLACK);
    }

    for(int i = 0; i < 0; ++i) {
        // diagonal x++ y++
        hga_line(HGA_BUFFER_1, x0, y0, x0 + x0, y0 + y0, HGA_BLACK);
        // diagonal x-- y++
        hga_line(HGA_BUFFER_1, x0, y0, x0 - x0, y0 + y0, HGA_BLACK);
        // diagonal x++ y--
        hga_line(HGA_BUFFER_1, x0, y0, x0 + x0, y0 - y0, HGA_BLACK);
        // diagonal x-- y--
        hga_line(HGA_BUFFER_1, x0, y0, x0 - x0, y0 - y0, HGA_BLACK);
    }
    */
    //getchar();
    // 360 circle
    uint16_t xx[360];
    uint16_t yy[360];
    for (int a = 0; a < 360; a +=1) {
        angle = (a * 3.14159) / 180.0;
        xx[a] = x0 + r * cos(angle);
        yy[a] = y0 + r * sin(angle);
    }

    for (int i = 0; i < 90; ++i) {
        hga_plot_pixel(HGA_BUFFER_1, xx[i], yy[i], HGA_WHITE);
        hga_bline0(HGA_BUFFER_1, x0, y0, xx[i], yy[i], HGA_BLACK);
    }

    //  wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();

    return 0;
}
