#include "test_lins.h"

#define ERR_GRAPHICS        "ERROR: No valid graphics adapter found!\n"
#define ERR_GRAPHICS_INFO   "ERROR INFO: This version of %s requires an Hercules Graphics Adapter.\n"
#define INFO_ADAPTER        "INFO: Adapter Type = %s\n"
#define PRESS_ENTER         "Press <Enter>"

void test() {
  printf("Bresenham line drawing performance tests Hercules Graphics cards\n");
  // confirm appropriate graphics adapter present
  uint8_t adapter_type = hga_detect_adapter();
  if (adapter_type < HW_VIDEO_ADAPTER_HGA) {
      fprintf(stderr, ERR_GRAPHICS);
      fprintf(stderr, ERR_GRAPHICS_INFO, argv[0]);
      return EXIT_FAILURE;
  }
  printf(INFO_ADAPTER, hw_video_adapter_names[adapter_type]);
  // 2. wait for ENTER key and switch to HGA graphics mode
  printf(PRESS_ENTER);
  getchar();
  hga_graphics_mode();
  hga_select_display_buffer(HGA_BUFFER_1);
  hga_cls(HGA_BUFFER_1);
  getchar();

  //  wait for ENTER key and switch back to text mode
getchar();
hga_text_mode();
}
