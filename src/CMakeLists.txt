cmake_minimum_required(VERSION 3.5)

project(
    retrolib
    VERSION 0.1.0
    LANGUAGES C
)

add_compile_options(
    -za99
)

file(GLOB SOURCES
    *.c
    DBG/*.c
    BIOS/*.c
    HW/*.c
    HGA/*.c
    CGA/*.c
    EGA/*.c
    VGA/*.c
    DOS/*.c
    MEM/*c
    DEMOS/*.c
)

add_executable(test ${SOURCES})

message(Source list="${SOURCES}")
