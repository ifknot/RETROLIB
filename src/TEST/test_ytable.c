#include "test_ytable.h"

#include "../DBG/debug_macros.h"
#include "../HGA/hga.h"

int test_ytable() {
    INFO("test ytable");
    if (YESNO("Switch to Hercules Graphics Mode and display file as pixels?")) {
        hga_graphics_mode();
        hga_select_display_buffer((char)HGA_BUFFER_1);
        hga_cls(HGA_BUFFER_1);

        for(int i = 0; i < HGA_SCREEN_Y_MAX; ++i) {
            hga_write_vram_buffer(HGA_BUFFER_1, HGA_BYTES_PER_LINE / 2, i, 0xFF);
        }

        PRESSENTER("To retrun to text mode");
        hga_text_mode();
    }
    return 0;
};
