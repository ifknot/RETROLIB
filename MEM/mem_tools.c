/**
 *
 *  @file      mem_tools.c
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      19.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "mem_tools.h"

#include <stdint.h>
#include <stdio.h>

#include "mem_constants.h"

void mem_tools_dump_mcb(char* mcb) {
    fprintf(stderr, "\nMCB - DOS Memory Control Block @%P\nOffset\tSize\tValue\n"
        "00\tbyte\t%c\n"
        "01\tword\t%04X\n"
        "03\tword\t%i\n" 
        "05\tbytes\treserved\n"
        "08\t8bytes\t%s\n"
        , mcb, *mcb, *(uint16_t*)(mcb + 1), *(uint16_t*)(mcb + 3), (char*)(mcb + 8));
}
