/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      21.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef HW_CONSTANTS_GRAPHICS_H
#define HW_CONSTANTS_GRAPHICS_H

 /**
 * Types of video adapter.
 * Dual screen the hi nibble is adpter 1 and the low nibble is adapter 2
 */
#define HW_VIDEO_ADAPTER_UKNOWN        0
#define HW_VIDEO_ADAPTER_MDA           1
#define HW_VIDEO_ADAPTER_HGA           2
#define HW_VIDEO_ADAPTER_HGA_PLUS      3
#define HW_VIDEO_ADAPTER_HGA_INCOLOR   4
#define HW_VIDEO_ADAPTER_HGA_CLONE     5
#define HW_VIDEO_ADAPTER_CGA           6
#define HW_VIDEO_ADAPTER_EGA           7
#define HW_VIDEO_ADAPTER_MCGA          8
#define HW_VIDEO_ADAPTER_VGA           9
#define HW_VIDEO_ADAPTER_SVGA          10



/**
* MDA/HGA and CGA 6845 CRT Controller port numbers.
*/
enum hw_crtc_port_t {
    crtc_port_MDA = 0x03B4,
    crtc_port_HGA = 0x03B4,
    crtc_port_CGA = 0x03D4
};

static char hw_video_adapter_names[12][32] = {
    "Unknown",
    "MDA",
    "Hercules",
    "Hercules Plus",
    "Hercules InColor",
    "Hercules Clone",
    "CGA",
    "EGA",
    "MCGA",
    "VGA",
    "SVGA",
    "Dual Adapters"
};

#endif
