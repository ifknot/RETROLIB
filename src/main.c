#include <stdio.h>

//#include "DEMOS/text2pix.h"
#include "TEST/test_video_info.h"
int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

	//return demo_text2pix(argc, argv);
	return test_video_info();

}
