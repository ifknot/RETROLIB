#include "mem_tools.h"

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "../DOS/dos_services_files.h"

#include "mem_constants.h"

uint16_t mem_available_low_paragraphs() {
    uint16_t paragraphs, err_code;
    paragraphs = err_code = 0;
    __asm {
        .8086

        mov     bx, 0FFFFh              ; deliberate over sized request
        mov     ah, 48h                 ; allocate memory
        int     21h                     ; INT 21h, 48h allocate memory service
        mov     err_code, ax            ; CF set, and AX = 08 (Not Enough Mem)
        mov     paragraphs, bx          ; size in paras of the largest block of memory available

    }
    assert(err_code == 8);
    return paragraphs;
}

void mem_dump_mcb(char* mcb) {
    fprintf(stderr, "\nMCB - DOS Memory Control Block @%P\nOffset\tSize\tValue\n"
        "00\tbyte\t%c\n"
        "01\tword\t%04X\n"
        "03\tword\t%i\n"
        "05\tbytes\treserved\n"
        "08\t8bytes\t%s\n"
        , mcb, *mcb, *(uint16_t*)(mcb + 1), *(uint16_t*)(mcb + 3), (char*)(mcb + 8));
}

dos_file_size_t mem_load_from_file(const char* path_name, char* start, uint16_t nbytes) {
    dos_file_handle_t fhandle = dos_open_file(path_name, ACCESS_READ_ONLY);
    dos_file_size_t bytes_loaded = 0;
    if (fhandle) {
        bytes_loaded = dos_read_file(fhandle, start, nbytes);
        dos_close_file(fhandle);
    }
    return bytes_loaded;
}

dos_file_size_t mem_save_to_file(const char* path_name, char* start, uint16_t nbytes){
    dos_file_size_t bytes_saved = 0;
    dos_file_handle_t fhandle = dos_open_file(path_name, ACCESS_WRITE_ONLY);
    if (fhandle) {
        bytes_saved = dos_write_file(fhandle, start, nbytes);
        dos_close_file(fhandle);
    }
    return bytes_saved;
}
