#include <stdio.h>

#include "DEMOS/TEXT2PIX.h"
//#include "HGA/demo_hga.h"
//#include "MEM/demo_mem_arena.h"


// TODO:
// [x] mem_arena
// [ ] get bbd-for-c to compile
// [ ] use bdd-for-c
// [ ] dos_tools_file_dump
// [ ] draw some 1 bit playing cards for a nice change 
// [x] TIME into bios_tools_timer.h
// [ ] test file size/file pos works properly 
// [ ] read write vram
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
