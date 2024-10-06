/**
 *
 *  @brief
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      5.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef TEXT2PIX_H
#define TEXT2PIX_H

#include <stdio.h>
#include <stdint.h>

#include "../DOS/dos_services_files.h"
#include "../DOS/dos_tools_files.h"

#include "../HGA/hga.h"

#include "../DBG/debug_macros.h"

#include "../HARDWARE/hw_constants_graphics.h"

int text2pix(int argc, char** argv) {
    char help_string[] = { "Usage:TEXT2PIX [path][filename]\nConverts a text file's characters to white pixels, punctuation to black pixels  and newlines to blank rows of pixels." };
    char error_string[] = { "ERROR: This version of TEXT2PIX requires a Hercules Graphics Adapter." };
    char file_path[255];
    uint8_t card_detected;
    dos_file_handle_t fhandle = 0;

    if (argc != 2) {
        printf("%s", help_string);
        return 0;
    }
    card_detected = hga_detect_adapter();
    if (!card_detected) {
        printf("%s", error_string);
        return 0;
    }
    printf("Card detected: %s\n", hw_video_adapter_names[card_detected]);
    sscanf(argv[1], "%s", file_path);
    fhandle = dos_open_file_using_handle(file_path, ACCESS_READ_ONLY);
    if (!fhandle) {
        return 0;
    }
    printf("%s file size=%lu bytes", file_path, dos_tools_file_size(fhandle));
    if (YESNO("Switch to Hercules Graphics Mode and display file as pixels?")) {
        hga_graphics_mode();
        hga_select_display_buffer((char)HGA_BUFFER_1);
        hga_cls(HGA_BUFFER_1);
        PRESSENTER("");
    }
    mda_text_mode();
    dos_close_file_using_handle(fhandle);
    return 0;
}

#endif
