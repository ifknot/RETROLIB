#include "demo_lines.h"

#include <stdio.h>
#include <stdlib.h>

#include "../HW/hw_constants_graphics.h"

#include "../HGA/hga_constants.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"
#include "../HGA/hga_colours.h"
#include "../HGA/hga_fast_line.h"

#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define INFO_ADAPTER        "INFO: Adapter Type = %s\n"
#define PRESS_ENTER         "Press <Enter>"

int lines(int argc, char** argv) {
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
    /*
    // 3. draw white vlines
    int j = 1;
    for(int i = 10; i < 100; i+=1) {
        hga_fast_vline(HGA_BUFFER_1, i, j, i, j + 174, HGA_WHITE);
        j+=1;
    }
    getchar();
    // 4. draw black vlines
    j = 1;
    for(int i = 10; i < 100; i+=2) {
        hga_fast_vline(HGA_BUFFER_1, i, j + 1, i, j + 173, HGA_BLACK);
        j+=2;
    }
    */
    /*
    // 5. draw white hlines 0..7
    int k = 0;
    int y = 8;
    for(int i = 0; i <90; ++i) {
        for(int j = 0; j < 8; ++j) {
            hga_fast_hline(HGA_BUFFER_1, k + j, j, k + j, 0, HGA_WHITE);
            hga_fast_hline(HGA_BUFFER_1, k + j, y, k + j, 0, HGA_WHITE);
            hga_fast_hline(HGA_BUFFER_1, k + j, 110 + j, k + 7, 0, HGA_WHITE);
            hga_fast_hline(HGA_BUFFER_1, k + 0, 218 + j, k + 7 - j, 0, HGA_WHITE);
        }
        k+=8;
    }
    getchar();
    // 6. draw black hlines 0..7hga_fast_hline(HGA_BUFhga_fast_hline(HGA_BUFFER_1, k + j, j, k + j, 0, HGA_WHITE);FER_1, k + j, j, k + j, 0, HGA_WHITE);
    hga_fill_vram_buffer(HGA_BUFFER_1, 0xFF);
    getchar();
    k = 0;
    y = 8;
    for(int i = 0; i <90; ++i) {
        for(int j = 0; j < 8; ++j) {
            hga_fast_hline(HGA_BUFFER_1, k + j, j, k + j, 0, HGA_BLACK);
            hga_fast_hline(HGA_BUFFER_1, k + j, y, k + j, 0, HGA_BLACK);
            hga_fast_hline(HGA_BUFFER_1, k + j, 110 + j, k + 7, 0, HGA_BLACK);
            hga_fast_hline(HGA_BUFFER_1, k + 0, 218 + j, k + 7 - j, 0, HGA_BLACK);
        }
        k+=8;
    }
    */
    /*
    // 7. draw white shared word hlines
    int k = 0;
    for(int j = 0; j < 45; j++) {
        int a = 7;
        int b = 8;
        for(int i =0; i < 8;++i) {
            hga_fast_hline(HGA_BUFFER_1, k + a, i, k + b, 0, HGA_WHITE);
            hga_fast_hline(HGA_BUFFER_1, k + a, i + 8, k + b, 0, HGA_WHITE);
            hga_fast_hline(HGA_BUFFER_1, k + a, i + 110, k + b, 0, HGA_WHITE);
            hga_fast_hline(HGA_BUFFER_1, k + a, i + 218, k + b, 0, HGA_WHITE);
            a--;
            b++;
        }
        k+=16;
    }
    getchar();
    // 8. draw black shared word hlines
    hga_fill_vram_buffer(HGA_BUFFER_1, 0xFF);
    getchar();
    k = 0;
    for(int j = 0; j < 45; j++) {
        int a = 7;
        int b = 8;
        for(int i =0; i < 8;++i) {
            hga_fast_hline(HGA_BUFFER_1, k + a, i, k + b, 0, HGA_BLACK);
            hga_fast_hline(HGA_BUFFER_1, k + a, i + 8, k + b, 0, HGA_BLACK);
            hga_fast_hline(HGA_BUFFER_1, k + a, i + 110, k + b, 0, HGA_BLACK);
            hga_fast_hline(HGA_BUFFER_1, k + a, i + 218, k + b, 0, HGA_BLACK);
            a--;
            b++;
        }
        k+=16;
    }
    */
    /*
    // 9. draw white lines length over mulitple bytes
    int k = 8;
    for(int i = 0; i < 45; ++i) {
        hga_hline(HGA_BUFFER_1, 0, i + 10, k + 15, 0, HGA_WHITE);
        k+=8;
    }
    getchar();
    // 10. draw black lines length over mulitple bytes
    hga_fill_vram_buffer(HGA_BUFFER_1, 0xFF);
    getchar();
    k = 8;
    for(int i = 0; i < 45; ++i) {
        hga_hline(HGA_BUFFER_1, 0, i + 10, k + 15, 0, HGA_BLACK);
        k+=8;
    }
    */
    // 11. draw white lines length over mulitple bytes
    int x = HGA_SCREEN_X_MAX / 2;
    int y = 10;
    int w = 1;
    for(int i = 0; i < 150; ++i) {
        hga_hline(HGA_BUFFER_1, x - (w / 2), y, x + w, y, HGA_WHITE);
        w += 2;
        y += 2;
    }
    // 12. draw black lines length over mulitple bytes
    getchar();
    // 12. draw black lines length over mulitple bytes
    y = 10;
    w = 1;
    hga_fill_vram_buffer(HGA_BUFFER_1, 0xFF);
    getchar();
    for(int i = 0; i < 150; ++i) {
        hga_hline(HGA_BUFFER_1, x - (w / 2), y, x + w, y, HGA_BLACK);
        w += 2;
        y += 2;
    }
    //  wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();

    return EXIT_SUCCESS;
}
