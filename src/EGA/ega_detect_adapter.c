#include "ega_detect_adapter.h"

#include "../HW/hw_constants_graphics.h"
#include "../BIOS/bios_video_services.h"

/**
* @brief detect EGA by attempting to use an EGA specific Video Subsystem Configuration (EGA/VGA) function
* @note check for VGA first
* @details if fails VGA test then calling INT 10,12 - Video Subsystem Configuration (EGA/VGA)
* AH = 12h
* BL = 10  return video configuration information
*
* on return:
* BH = 0 if color mode in effect
*    = 1 if mono mode in effect
* BL = 0 if 64k EGA memory
*    = 1 if 128k EGA memory
*    = 2 if 192k EGA memory
*    = 3 if 256k EGA memory
* CH = feature bits
* CL = switch settings
*
* @note If upon return from this call, BL>3, then must be running on a CGA or MDA(HGA) - not an EGA or VGA.
*/
uint8_t ega_detect_adapter() {
        bios_video_subsystem_config_t info;
        bios_return_video_configuration_information(&info);
        return (info.ega_memory < 4) ? HW_VIDEO_ADAPTER_EGA : HW_VIDEO_ADAPTER_UKNOWN;
}
