#include "demo_display_buffer.h"

#include <stdio.h>
#include <stdlib.h>

#include "../HW/hw_constants_graphics.h"

#include "../HGA/hga_constants.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"

#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define INFO_ADAPTER        "INFO: Adapter Type = %s\n"
#define PRESS_ENTER         "Press <Enter>"

int display_buffer(int argc, char** argv) {
    uint16_t x, y, w, h;
    w = 8;
    h = w * 8;
    x = (HGA_BYTES_PER_LINE / 2) - (w / 2);
    y = (HGA_SCREEN_Y_MAX / 2) - (h / 2);
    // confirm appropriate graphics adapter present
    uint8_t adapter_type = hga_detect_adapter();
    if (adapter_type < HW_VIDEO_ADAPTER_HGA) {
        fprintf(stderr, ERR_GRAPHICS);
        fprintf(stderr, ERR_GRAPHICS_INFO, argv[0]);
        return EXIT_FAILURE;
    }
    printf(INFO_ADAPTER, hw_video_adapter_names[adapter_type]);
    // wait for ENTER key and switch to HGA graphics mode
    printf(PRESS_ENTER);
    getchar();
    hga_graphics_mode();
    hga_select_display_buffer(HGA_BUFFER_1); //0);
    hga_cls(HGA_BUFFER_1);
    /*
    getchar();
    hga_cls(HGA_BUFFER_0);
    getchar();
    hga_fill_vram_buffer(HGA_BUFFER_0, 254);
    hga_fill_vram_buffer(HGA_BUFFER_0, 255);
    getchar();
    hga_select_display_buffer(HGA_BUFFER_1);
    getchar();
    hga_knit_vram_buffer(HGA_BUFFER_1, 0xAA, 0x55);
    getchar();
    hga_knit_vram_buffer(HGA_BUFFER_0, 0, 255);
    printf("bytes written = %li", hga_save_vram_buffer(HGA_BUFFER_0, "vram.raw"));
    hga_cls(HGA_BUFFER_1);
    getchar();
    printf("bytes read = %li", hga_load_vram_buffer(HGA_BUFFER_1, "vram.raw"));
    getchar();
    hga_cls(HGA_BUFFER_0);
    hga_knit_vram_buffer(HGA_BUFFER_0, 0, 255);
    hga_cls(HGA_BUFFER_1);
    for(uint16_t row = 0; row < HGA_SCREEN_Y_MAX; ++row) {
        for(uint16_t col = 0; col < HGA_BYTES_PER_LINE; col +=2) {
            hga_write_vram_buffer(HGA_BUFFER_1, col, row, 255);
        }
    }
    getchar();
    for(uint16_t row = 0; row < HGA_SCREEN_Y_MAX; ++row) {
        for(uint16_t col = 0; col < HGA_BYTES_PER_LINE; col +=2) {
            hga_write_vram_buffer(HGA_BUFFER_1, col + 1, row, hga_read_vram_buffer(HGA_BUFFER_0, col, row));
        }
    }
    printf("bytes written = %li", hga_save_vram_buffer(HGA_BUFFER_1, "vram.raw"));

    getchar();
    hga_knit_vram_buffer(HGA_BUFFER_1, 0, 255);
    for(uint16_t row = 0; row < h; ++row) {
        for(uint16_t col = 0; col < w; ++col) {
            hga_write_vram_buffer(HGA_BUFFER_1, x + col, y + row, 255);
        }
    }
    getchar();
    hga_pixel_scroll_up(HGA_BUFFER_1, 0);
    getchar();
   	for(uint16_t i = HGA_SCREEN_Y_MAX; i > 0; --i) {
		hga_pixel_scroll_up(HGA_BUFFER_1, 0);
	}
    */
    getchar();
    printf("bytes read = %li", hga_load_vram_buffer(HGA_BUFFER_1, "vram.raw"));
    getchar();
    hga_pixel_scroll_down(HGA_BUFFER_1, 0);
    getchar();
    for(uint16_t i = HGA_SCREEN_Y_MAX; i > 0; --i) {
		hga_pixel_scroll_down(HGA_BUFFER_1, 0);
	}
    getchar();
    hga_text_mode();

    return EXIT_SUCCESS;
}
