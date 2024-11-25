#include "vga_detect_adapter.h"

#include "../HW/hw_constants_graphics.h"
#include "../BIOS/bios_video_services.h"

/**
* @brief detect VGA by attempting to use a VGA specific Video Subsystem Configuration (EGA/VGA) function
* @details
* calling INT 10h,12h - Video Subsystem Configuration (EGA/VGA) with
*       BL = 32h  CPU access to video RAM
*       AL = 0  enable CPU access to video RAM and I/O ports
* Is only compatible with VGA and will *only* return AL = 12h if a valid value was passed in AL
*
* on return: AL = 12h if a valid value was passed in AL ie VGA+
*
*/
uint8_t vga_detect_adapter() {
    return (bios_video_subsystem_configuration(BIOS_CPU_ACCESS_TO_VIDEO_RAM, 0, 0) == 0x12)
        ? HW_VIDEO_ADAPTER_VGA
        : HW_VIDEO_ADAPTER_UKNOWN;
}
