/**
 *
 *  @brief    Visualize the patterns in text layout of a document by condensing characters into pixels. 
 *  @details  Code as a Creative Medium 001 
 *  Converts a text file's characters to white pixels, punctuation to black pixels and newlines to blank rows of pixels.
 *  HGA is 720 x 348 pixels, so read upto 720 bytes from the text file at a time and then convert to pixels
 *  
 *  Demonstrates using retrolib libraries and the coding idioms for them
 *  @author    Jeremy Thornton
 *  @date      5.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef TEXT2PIX_H
#define TEXT2PIX_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../DBG/debug_macros.h"
#include "../DOS/dos_services_files.h"
#include "../DOS/dos_tools_files.h"
#include "../HGA/hga.h"
#include "../MEM/mem_arena.h"

int demo_text2pix(int argc, char** argv) {
    char help_string[] = { "Usage:TEXT2PIX [path][filename]\nConverts a text file's characters to white pixels, punctuation to black pixels  and newlines to blank rows of pixels." };
    char error_string[] = { "ERROR: This version of TEXT2PIX requires a Hercules Graphics Adapter." };
    char file_path[255];
    uint8_t card_detected;
    dos_file_handle_t fhandle = 0;
    mem_arena_t* arena; 
    const uint16_t FILE_BLOCK_SIZE = 720;    
    uint16_t file_bytes_read;
    char* text_buffer;
    uint16_t pixel_word;
    const uint16_t PIXEL_BITMASK = 0x8000;
// 1. minimal user input checking
    if (argc != 2) {
        printf("%s", help_string);
        return EXIT_FAILURE;
    }
// 2. confirm appropriate graphics adapter present
    card_detected = hga_detect_adapter();
    if (!card_detected) {
        printf("%s", error_string);
        return EXIT_FAILURE;
    }
    printf("Card detected: %s\n", hw_video_adapter_names[card_detected]);
// 3.0 extract the file name given by user from the arguement list
    sscanf(argv[1], "%s", file_path);
// 3.1 try open the file
    fhandle = dos_open_file_using_handle(file_path, ACCESS_READ_ONLY);
    if (!fhandle) {
        return 0;
    }
    fprintf(stderr, "%s file size=%lu bytes", file_path, dos_tools_file_size(fhandle));
// 4.0 create 360 bytes of memory space as an arena
    arena = mem_arena_new(MEM_ARENA_POLICY_DOS, FILE_BLOCK_SIZE);
    if (!arena) {
        return EXIT_FAILURE;
    }
    mem_arena_dump(arena);
// 4.1 allocate all of the arena as a char buffer
    text_buffer = (char*)mem_arena_alloc(arena, FILE_BLOCK_SIZE);
    mem_arena_dump(arena);
// 5. switch to graphics mode
    if (YESNO("Switch to Hercules Graphics Mode and display file as pixels?")) {
        hga_graphics_mode();
        hga_select_display_buffer((char)HGA_BUFFER_1);
        hga_cls(HGA_BUFFER_1);
// 6.0 action loop
        do {
// 6.1 load (up to) 360 bytes ie 4 screen lines of file data  
            file_bytes_read = dos_read_file_using_handle(fhandle, text_buffer, FILE_BLOCK_SIZE);
            LOG(file_bytes_read, % i);      
// 6.2 convert file data into word data 16 characters at a time per the given filter
            pixel_word = 0;

// 6.7 more file data to process?
        } while (file_bytes_read);
    }
// 7. switch back to text mode
    mda_text_mode();
// 8. tidy up resources
    dos_close_file_using_handle(fhandle);
    mem_arena_delete(arena);
    return EXIT_SUCCESS;
}

#endif
