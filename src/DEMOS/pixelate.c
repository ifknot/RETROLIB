#include "pixelate.h" 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//#include "../DBG/debug_macros.h"
#include "../BIOS/bios_timer_io_services.h"
#include "../BIOS/bios_tools_timer.h"
#include "../HGA/hga_detect_adapter.h"
#include "../HGA/hga_video_mode.h"
#include "../HGA/hga_display_buffer.h"
#include "../DOS/dos_services_files.h"
#include "../DOS/dos_tools_files.h"
#include "../MEM/mem_arena.h"

// string constants
#define USAGE_INFO         "Converts a text file's characters to white pixels, punctuation to black pixels and newlines to a new pixel row."
#define USAGE_FMT          "%s [inputfile]"
#define ERR_GRAPHICS       "ERROR: This version of %s requires a Hercules Graphics Adapter."
#define ERR_MEMORY         "ERROR: Memory allocation failure."
#define ERR_FOPEN_INPUT    "ERROR: Unable to open input file %s"
#define METRICS_INFO       "%s file %lu characters as pixels. Duration = %f secs"
// value constants
#define FILE_BLOCK_SIZE 720


int pixelate(int argc, char** argv) {
    char file_path[255];
    char* text_buffer;
    dos_file_handle_t fhandle = 0;
    mem_arena_t* arena;
    uint8_t pixel_byte, pixel_bitmask;
    uint16_t file_bytes_read, byte_count, bit_count, k;
    uint32_t char_count = 0;
    bios_ticks_since_midnight_t t1, t2;

// 1. confirm appropriate graphics adapter present
    if (!hga_detect_adapter()) {
        fprintf(stderr, ERR_GRAPHICS, argv[0]);
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
    fhandle = dos_open_file_using_handle(file_path, ACCESS_READ_ONLY);
    if (!fhandle) {
        fprintf(stderr, ERR_FOPEN_INPUT, file_path);
        return EXIT_FAILURE;
    }
// 3.2 total characters to process
    char_count = dos_tools_file_size(fhandle));
// 4.0 create screen size block of memory space as an arena
    arena = mem_arena_new(MEM_ARENA_POLICY_DOS, FILE_BLOCK_SIZE);
    if (!arena) {
        fprintf(stderr, ERR_MEMORY);
        return EXIT_FAILURE;
    }
// 4.1 allocate all of the arena as a char buffer
    text_buffer = (char*)mem_arena_alloc(arena, FILE_BLOCK_SIZE);
// 5. switch to graphics mode
    hga_graphics_mode();
    hga_select_display_buffer((char)HGA_BUFFER_1);
    hga_cls(HGA_BUFFER_1);
// 6.0 reset the bios system clock to zero and take an initial reading
    bios_set_system_clock(0);
    bios_read_system_clock(&t1);


// 6.7 measure duration of conversion loop and display info
    bios_read_system_clock(&t2);
    fprintf(stderr, METRICS_INFO, file_path, (unsigned long)char_count, bios_tools_timer_ticks_to_seconds(t2 - t1));      
// 7. wait for ENTER key and switch back to text mode
    getchar();
    hga_text_mode();
// 8. tidy up resources
    dos_close_file_using_handle(fhandle);
    mem_arena_delete(arena);
    return EXIT_SUCCESS;
}
