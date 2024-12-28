#ifndef DEMO_HGA_H
#define DEMO_HGA_H

#include <assert.h>

#include "../DBG/debug_macros.h"

#include "../HGA/hga.h"
#include "../HW/hw_detect_graphics_adapter.h"
#include "../BIOS/bios_video_services.h"
#include "../BIOS/bios_video_services_types.h"
#include "../BIOS/bios_timer_io_services.h"
//#include "../MATH/math_prng.h"

void demo_hga() {
        uint16_t colour, mono, buffer = HGA_BUFFER_0;
        bios_video_mode_t mode;
        mono = hw_detect_monochrome_adapter();
        colour = hw_detect_colour_adapter();
        if (colour) {
                fprintf(stderr, "\ncolour display = %s\n", hw_video_adapter_names[colour]);
        }
        if (mono) {
                fprintf(stderr, "monochrome display = %s\n", hw_video_adapter_names[mono]);
        }
        INFO("***** DEMO HERCULES GRAPHICS FUNCTIONS *****");
        if (!hga_detect_adapter()) {
                INFO("ERROR! No Hercules Family Graphics Adapter Installed.");
                return;
        }
        if (YESNO("* switch to HGA Graphics Mode buffer 0 ? ")) {
                hga_graphics_mode();
                INFO("HGA Graphics Mode.");
                if (YESNO("* switch to HGA buffer 1 ? ")) {
                        hga_select_display_buffer(1);
                        buffer = HGA_BUFFER_1;
                }
                if (YESNO("* clear screen HGA Graphics Mode? ")) {
                        hga_cls(buffer);
                }
                if (YESNO("* fill display with AAh pattern ? ")) {
                        hga_fill_vram_buffer(buffer, 0xAA);
                }
                if (YESNO("* fill display with FFh pattern ? ")) {
                        hga_fill_vram_buffer(buffer, 0xFF);
                }
                if (YESNO("* plot pixels ? ")) {
                        math_prng_set_seed(bios_read_system_clock());
                        for (int i = 0; i < 10000; ++i) {
                            hga_plot_pixel_calculate(buffer, math_prng_xorshift16() % 720, math_prng_xorshift16() % 348, hga_black);
                        }
                }
                YESNO("* switch back to MDA Text Mode... ");
                hga_text_mode();
                INFO("MDA Text Mode.");
        }
}

#endif
