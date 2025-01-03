#include "test_ytable.h"

#include "../DBG/debug_macros.h"
#include "../HGA/hga.h"

int test_ytable() {
    INFO("test ytable");
    if (YESNO("Switch to Hercules Graphics Mode and display file as pixels?")) {
        hga_graphics_mode();
        hga_select_display_buffer((char)HGA_BUFFER_1);
        hga_cls(HGA_BUFFER_1);

        for(int i = 0; i < 90; ++i) {
            hga_write_vram_buffer(HGA_BUFFER_1, i, i, 0xFF);
        }
        LOG(%i, hga_read_vram_buffer(HGA_BUFFER_1, 0, 0));
        LOG(%i, hga_read_vram_buffer(HGA_BUFFER_1, 45, 45));
        LOG(%i, hga_read_vram_buffer(HGA_BUFFER_1, 46, 45));
        LOG(%i, hga_read_vram_buffer(HGA_BUFFER_1, 45, 44));
        PRESSENTER("To retrun to text mode");
        hga_text_mode();

    }
    return 0;
};
