#include <stdio.h>

#include "DEMOS/TEXT2PIX.h"
//#include "HGA/demo_hga.h"
//#include "MEM/demo_mem_arena.h"


// TODO:
// [x] switch to Linux on T480
// [x] use git gui
// [x] compile open watcom 2
// [x] use Zed
// [x] wcl HELLO.EXE test use DOSBOX
// [ ] convert to source and build directorys within RETROLIB base directory
// [ ] use CMake
// cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS -D CMAKE_SYSTEM_PROCESSOR=I86
//
// [ ] use NASA's IKOS https://github.com/NASA-SW-VnV/ikos'
//
// [ ] get bbd-for-c to compile
// [ ] use bdd-for-c
//
// [ ] draw some 1 bit playing cards for a nice change
//
// [x] mem_arena
// [x] TIME into bios_tools_timer.h
// [ ] dos_tools_file_dump
// [ ] test file size/file pos works properly
// [ ] read write vram using y lookup
// [ ] finish buffer demo
// [ ] convert video mode preset names and table
// [ ] demo buffer png anim to git readme
// [ ] pixel perf compare algos
// [ ] fix MDA naming
// [ ] demo Mendelbrot fixed point
// [ ] bresenham line
// [ ] demo lines
// [ ] rectangle
// [ ] demo rectangle
// [ ] circle (unroll loop)
// [ ] demo circle
// [ ] byte aligned block transfer
// [ ] demo byte_aligned_blt

int main(int argc, char** argv) {

	return demo_text2pix(argc, argv);

}
