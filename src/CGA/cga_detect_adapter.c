#include "cga_detect_adapter.h"

#include "../HW/hw_constants_graphics.h"
#include "../HW/hw_detect_crtc.h"

uint8_t cga_detect_adapter() {
        if (hw_detect_CRTC_at_port(crtc_port_CGA)) {
            return HW_VIDEO_ADAPTER_CGA;
        }
        return HW_VIDEO_ADAPTER_UKNOWN;
}
