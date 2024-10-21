/**
 *
 *  @brief    Visualize the patterns in text layout of a document by condensing characters into pixels. 
 *  @details  Code as a Creative Medium 001 
 *  Converts a text file's characters to white pixels, punctuation to black pixels and newlines to blank rows of pixels.
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
// 1. minimal user input checking
    if (argc != 2) {
        printf("%s", help_string);
        return 0;
    }
// 2. confirm appropriate graphics adapter present
    card_detected = hga_detect_adapter();
    if (!card_detected) {
        printf("%s", error_string);
        return 0;
    }
    printf("Card detected: %s\n", hw_video_adapter_names[card_detected]);
    sscanf(argv[1], "%s", file_path);
// 3. try open the file name given by user 
    fhandle = dos_open_file_using_handle(file_path, ACCESS_READ_ONLY);
    if (!fhandle) {
        return 0;
    }
    printf("%s file size=%lu bytes", file_path, dos_tools_file_size(fhandle));
// 4. allocate 360 bytes of memory space as an arena
// 5. switch to graphics mode
    if (YESNO("Switch to Hercules Graphics Mode and display file as pixels?")) {
        hga_graphics_mode();
        hga_select_display_buffer((char)HGA_BUFFER_1);
        hga_cls(HGA_BUFFER_1);
// 6. action loop
// 7. load (up to) 360 bytes ie 4 screen lines of file data
// 8. convert file data into word data 16 characters at a time per the given filter
    }
// 9. switch back to text mode
    mda_text_mode();
// 10. tidy up resources
    dos_close_file_using_handle(fhandle);
    return 0;
}

#endif
