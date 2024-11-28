#include <stdio.h>

//#include "DEMOS/text2pix.h"
#include "DEMOS/text2bit.h"

int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

	return demo_text2bit(argc, argv);

}
