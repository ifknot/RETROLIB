#include <stdio.h>

//#include "DEMOS/pixelate.h"
//#include "DEMOS/demo_lines.h"
//#include "DEMOS/demo_rectangles.h"
#include "DEMOS/demo_bresenham.h"
//#include "DEMOS/demo_pixels.h"
//#include "DEMOS/demo_circles.h"

//#include "TESTS/test_lines.h"

#include "MEM/mem_tools.h"

int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

    printf("DOS low memory size = %luKB\n", (mem_available_low_paragraphs() * 16L) / 1024L);

	return bresenham(argc, argv);

	// test();
	// return 0;
}
