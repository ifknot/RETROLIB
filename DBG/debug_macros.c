/**
 *
 *  @file      debug_macros.c
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      19.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "debug_macros.h"

void __fprintf_as_binary(size_t const size, void const* const ptr) {
    unsigned char* b = (unsigned char*)ptr;
    unsigned char byte;
    int i, j;
    fprintf(stderr, "\n");
    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            fprintf(stderr, "%u", byte);
        }
    }
}