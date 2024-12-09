#include "text2bit.h"

#include <stdio.h>
#include <stdlib.h>

#include "../DBG/debug_macros.h"

int demo_text2bit(int argc, char** argv) {
    FILE* fin;
    int c;
    switch(argc) {
        case 1:
        fin = stdin;
        break;
        case 2:
        default:
        printf("Error! Argument count = %i", argc);
    }

    do {
        c = fgetc(fin);
        fputc(32, stdout);
        fputc(c, stdout);
    } while(c != EOF && c != '\n' && c != '\r');



    return EXIT_SUCCESS;
}
