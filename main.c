#include <stdio.h>

#include "DOS/dos_services_files.h"
#include "DOS/dos_tools_files.h"

/**
* Load a text file and convert its characters to pixels
*/
int main(int argc, char** argv) {
    char help_string[] = {"Usage:TEXT2PIX [path][filename]\nConverts a text file's characters to white pixels, punctuation to a black pixels\nand newlines to blank row of pixels."};
    char file_path[255];
    dos_file_handle_t fhandle = 0;

    if (argc < 2) {
        printf("%s", help_string);
    }

    sscanf(argv[1], "%s", file_path);
    fhandle = dos_open_file_using_handle(file_path, ACCESS_READ_ONLY);

    printf("file size=%lu bytes", dos_tools_file_size(fhandle));

    dos_close_file_using_handle(fhandle);
    return 0;
}
