#include "pixelate.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../BIOS/bios_timer_io_services.h"
#include "../TIME/time_tools.h"
#include "../HGA/hga_constants.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"
#include "../DOS/dos_services_files.h"
#include "../FILE/file_tools.h"
#include "../MEM/mem_arena.h"

// string constants
#define USAGE_INFO          "INFO: Converts a text file's characters to white pixels, punctuation to black pixels and newlines to a new pixel row.\n"
#define USAGE_FMT           "\nUSAGE: %s [inputfile]\n"
#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_INFO_GRAPHICS   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define ERR_MEMORY          "ERROR: Memory allocation failure!\n"
#define ERR_FOPEN_INPUT     "ERROR: Unable to open input file %s!\n"
#define INFO_FINPUT         "INFO: %s file size %lu bytes.\n"
#define INFO_METRICS        "INFO: %s file %lu characters as pixels. Duration = %f secs\n"

token_t tokenize_character(char c) {
    if(c < 33) return TOK_NO_PIXEL;
    if(c == 10) return TOK_LF;
    if(c == 13) return TOK_UNDEFINED;   // i.e. consume carriage return
    return TOK_PIXEL;
}

int pixelate(int argc, char** argv) {
    char file_path[255];
    char* text_buffer;
    dos_file_handle_t fhandle = 0;
    mem_arena_t* arena;
    uint8_t pixel_byte, pixel_bitmask;
    uint16_t file_bytes_read, byte_count, bit_count, tpos, x, y;
    uint32_t file_size, char_count = 0;
    bios_ticks_since_midnight_t t1, t2;

// 1. confirm appropriate graphics adapter present
    if (hga_detect_adapter() < HW_VIDEO_ADAPTER_HGA) {
        fprintf(stderr, ERR_GRAPHICS);
        fprintf(stderr, ERR_INFO_GRAPHICS, argv[0]);
        return EXIT_FAILURE;
    }
// 2. minimal user input checking
    if (argc != 2) {
        fprintf(stderr, USAGE_INFO);
        fprintf(stderr, USAGE_FMT, argv[0]);
        return EXIT_FAILURE;
    }
// 3.0 extract the file name given by user from the arguement list
    sscanf(argv[1], "%s", file_path);
// 3.1 try open the file
    fhandle = dos_open_file(file_path, ACCESS_READ_ONLY);
    if (!fhandle) {
        fprintf(stderr, ERR_FOPEN_INPUT, file_path);
        return EXIT_FAILURE;
    }
// 3.2 total characters to process
    file_size = file_get_size(fhandle);
    printf(INFO_FINPUT, argv[0], (unsigned long)file_size);
// 4.0 create screen size block of memory space as an arena
    arena = mem_arena_new(MEM_ARENA_POLICY_DOS, HGA_BYTES_PER_SCREEN);
    if (!arena) {
        fprintf(stderr, ERR_MEMORY);
        return EXIT_FAILURE;
    }
// 4.1 allocate all of the arena as a char buffer
    text_buffer = (char*)mem_arena_alloc(arena, HGA_BYTES_PER_SCREEN);
// 5. switch to graphics mode
    //hga_graphics_mode();
    //hga_select_display_buffer((char)HGA_BUFFER_1);
    //hga_cls(HGA_BUFFER_1);
// 6.0 reset the bios system clock to zero and take an initial reading
    bios_set_system_clock(0);
    bios_read_system_clock(&t1);
// 6.1 process all the characters from the input file a screen full at a time
    do {
        file_bytes_read = dos_read_file(fhandle, text_buffer, HGA_BYTES_PER_SCREEN);
        x = y = tpos = 0;                                           // reset to top left of screen and start of text buffer
// 6.2 convert file data into screen byte data 8 characters at a time per the given filter
        byte_count = file_bytes_read >> 3;                          // div 8
// 6.3 convert the text 8 characters at time into a screen data byte
        for (int i = 0; i < byte_count;++i) {
            pixel_byte = 0;
            pixel_bitmask = 0x80;
            for (int j = 0; j < 8; ++j) {
                switch (tokenize_character(text_buffer[tpos])) {
                    case TOK_LF:
                        // if j < 7 screen buffer current pixel byte & reset pixel byte 0
                        // y++
                    break;
                    case TOK_PIXEL:
                        pixel_byte |= pixel_bitmask;
                    break;
                    case TOK_NO_PIXEL:
                    default:
                    break;
                }
                pixel_bitmask >>= 1;                                // next bit
                tpos++;                                             // next character
                char_count++;                                       // running total
            }
            //hga_write_vram_buffer(HGA_BUFFER_0, x++, y);
        }
// 6.4 process any remaining characters mod 8 into a final byte
        bit_count = file_bytes_read & 7;                            // mod 8
        pixel_byte = 0;
        pixel_bitmask = 0x80;
        for (int j = 0; j < bit_count; ++j) {
            switch (tokenize_character(text_buffer[tpos])) {
               case TOK_LF:
                    // if j < 7 screen buffer current pixel byte & reset pixel byte 0
                    // y++
                break;
                case TOK_PIXEL:
                    pixel_byte |= pixel_bitmask;
                break;
                case TOK_NO_PIXEL:
                default:
                break;
            }
            pixel_bitmask >>= 1;                                // next bit
            tpos++;                                             // next character
            char_count++;                                       // running total
        }
        //hga_write_vram_buffer(HGA_BUFFER_0, x, y);
// 6.5 scroll screen up if char_count < files_size
// hga_scroll_up(348);
// 6.6 more file data to process?
    } while (file_bytes_read);
// 6.7 measure duration of conversion loop
    bios_read_system_clock(&t2);
// 7. wait for ENTER key and switch back to text mode
    //getchar();
    //hga_text_mode();
// 8. tidy up resources
    dos_close_file(fhandle);
    mem_arena_delete(arena);
// 9. display peformance metrics
    printf(INFO_METRICS, file_path, (unsigned long)char_count, bios_tools_timer_ticks_to_seconds(t2 - t1));
    return EXIT_SUCCESS;
}
