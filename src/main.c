#include <stdio.h>

//#include "DEMOS/pixelate.h"
//#include "DEMOS/demo_lines.h"
//#include "DEMOS/demo_rectangles.h"
#include "DEMOS/demo_bresenham.h"

int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

	return bresenham(argc, argv);


}
