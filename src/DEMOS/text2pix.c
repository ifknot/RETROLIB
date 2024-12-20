#include "text2pix.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../DBG/debug_macros.h"
#include "../BIOS/bios_timer_io_services.h"
#include "../BIOS/bios_tools_timer.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"
#include "../DOS/dos_services_files.h"
#include "../DOS/dos_tools_files.h"
#include "../MEM/mem_arena.h"

int demo_text2pix(int argc, char** argv) {
    char help_string[] = { "Usage:TEXT2PIX [path][filename]\nConverts a text file's characters to white pixels, punctuation to black pixels  and newlines to blank rows of pixels." };
    char error_string[] = { "ERROR: This version of TEXT2PIX requires a Hercules Graphics Adapter." };
    char file_path[255];
    char* text_buffer;

    dos_file_handle_t fhandle = 0;
    mem_arena_t* arena;
    uint8_t card_detected, pixel_byte, pixel_bitmask;
    const uint16_t FILE_BLOCK_SIZE = 720;
    uint16_t file_bytes_read, byte_count, bit_count, k;
    uint32_t char_count = 0;
    bios_ticks_since_midnight_t t1, t2;

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
        INFO("fail to find");
        return EXIT_FAILURE;
    }
    fprintf(stderr, "%s file size=%lu bytes", file_path, (unsigned long)dos_tools_file_size(fhandle));
    dos_tools_file_dump(stdout, fhandle);
// 4.0 create 360 bytes of memory space as an arena
    arena = mem_arena_new(MEM_ARENA_POLICY_DOS, FILE_BLOCK_SIZE);
    if (!arena) {
        return EXIT_FAILURE;
    }
// 4.1 allocate all of the arena as a char buffer
    text_buffer = (char*)mem_arena_alloc(arena, FILE_BLOCK_SIZE);
// 5. switch to graphics mode
    if (YESNO("Switch to Hercules Graphics Mode and display file as pixels?")) {
        hga_graphics_mode();
        hga_select_display_buffer((char)HGA_BUFFER_1);
        hga_cls(HGA_BUFFER_1);
// 6.0 reset the bios system clock to zero and take an initial reading
        bios_set_system_clock(0);
        bios_read_system_clock(&t1);
        do {
// 6.1 load (up to) 360 bytes ie 4 screen lines of file data
            file_bytes_read = dos_read_file_using_handle(fhandle, text_buffer, FILE_BLOCK_SIZE);
            k = 0;
            //LOG(file_bytes_read, % i);
// 6.2 convert file data into screen byte data 8 characters at a time per the given filter
            byte_count = file_bytes_read >> 3;                          // div 8
            //LOG(byte_count, % i);
// 6.3 convert the text 8 characters at time into a screen data byte
            for (int i = 0; i < byte_count;++i) {
                pixel_byte = 0;
                pixel_bitmask = 0x80;
                for (int j = 0; j < 8; ++j) {
                    //printf("%c",text_buffer[k]);
                    // if CR then buffer byte so far, reset byte and bitmask and graphics new line
                    // ?asm XLATB w a lookup table more selective and variable approach
                    if (text_buffer[k] > 32 && text_buffer[k] < 127) {  // alphanumeric characters
                        pixel_byte |= pixel_bitmask;                    // set matching bit to 1
                    }

                    pixel_bitmask >>= 1;                                // next bit
                    k++;                                                // next character
                    char_count++;                                       // running total
                }
                // buffer byte
                printf("[%i]", pixel_byte);
            }

// 6.4 process any remaining characters mod 8 into a final byte
            bit_count = file_bytes_read & 7;                            // mod 8
            pixel_byte = 0;
            pixel_bitmask = 0x80;
            //LOG(bit_count, % i);
            for (int j = 0; j < bit_count; ++j) {
                //printf("%c", text_buffer[k]);
                 // if CR then buffer byte so far, reset byte and bitmask and graphics new line
                //? XLATB function
                if (text_buffer[k] > 32 && text_buffer[k] < 127) {      // alphanumeric characters
                    pixel_byte |= pixel_bitmask;
                }
                pixel_bitmask >>= 1;
                k++;
                char_count++;
            }
            // buffer byte
            //printf("[%i]",pixel_byte);
// 6.5 write to HGA screen buffer
// 6.6 more file data to process?
        } while (file_bytes_read);
    }
// 6.7 measure duration of conversion loop and display info
    bios_read_system_clock(&t2);
    fprintf(stderr, "%s file %lu characters as pixels. Duration = %f secs", file_path, (unsigned long)char_count, bios_tools_timer_ticks_to_seconds(t2 - t1));
// 7. switch back to text mode
    hga_text_mode();
// 8. tidy up resources
    dos_close_file_using_handle(fhandle);
    mem_arena_delete(arena);

    return EXIT_SUCCESS;
}
