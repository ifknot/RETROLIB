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

int pixelate(int argc, char** argv) {
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
}
  
  return EXIT_SUCCESS;
}
