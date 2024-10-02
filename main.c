#include <stdio.h>

#include "DOS/dos_services_files.h"

/**
* Load a text file and convert its characters to pixels
*/
int main(int argc, char** argv) {
    int i;
    char help_string[] = {"Usage:TEXT2PIX [path][filename]\nConverts a text file's characters to white pixels, punctuation to a black pixels\nand newlines to blank row of pixels."};
    char file_path[255];
    if (argc < 2) {
        printf("%s", help_string);
    }
    sscanf(argv[1], % s, file_path);
    printf("%s", file_path);
    return 0;
}
