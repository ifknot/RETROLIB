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
    fprintf(stderr, "\n");
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            fprintf(stderr, "%u", byte);
        }
    }
}

int __yesno(char* msg) {
    char key;
    printf("\n%s Y/N ", msg);
    return scanf(" %c", &key) == 1 && key == 'Y' || key == 'y';
}

void __pressenter(char* msg) {
    printf("\n%s Press <ENTER> ", msg);
    getchar();
	while (getchar() != '\n');
}

char __anykey(char* msg) {
    printf("\n%s ", msg);
	return getchar();
}
