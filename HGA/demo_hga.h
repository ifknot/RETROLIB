/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      29.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef DEMO_HGA_H
#define DEMO_HGA_H

#include <assert.h>

#include "..\DBG\debug_macros.h"

#include "hga.h"
#include "../HARDWARE/hw_detect_graphics_adapter.h"
#include "../BIOS/bios_video_services.h"
#include "../BIOS/bios_video_services_types.h"

void demo_hga() {
	uint16_t colour, mono, buffer = HGA_BUFFER_0;
	bios_video_mode_t mode;
	mono = hw_detect_monochrome_adapter();
	colour = hw_detect_colour_adapter();
	if (colour) {
		fprintf(stderr, "\ncolour display = %s\n", hw_video_adapter_names[colour]);

	if(mono) fprintf(stderr, "monochrome display = %s\n", hw_video_adapter_names[mono]);

	INFO("***** DEMO HERCULES GRAPHICS FUNCTIONS *****");
	if (!hga_detect_adapter()) {
		INFO("ERROR! No Hercules Family Graphics Adapter Installed.");
		return;
	}
	if (YESNO("* switch to HGA Graphics Mode buffer 0 ? ")) {
		hga_graphics_mode();
		INFO("HGA Graphics Mode.");
		if (YESNO("* switch to HGA buffer 1 ? ")) {
			hga_select_display_buffer(1);
			buffer = HGA_BUFFER_1;
		}
		if (YESNO("* clear screen HGA Graphics Mode? ")) {
			hga_cls(buffer);
		}
		if (YESNO("* fill display with AAh pattern ? ")) {
			hga_fill_vram_buffer(buffer, 0xAA);
		}
		if (YESNO("* fill display with FFh pattern ? ")) {
			hga_fill_vram_buffer(buffer, 0xFF);
		}
		YESNO("* switch back to MDA Text Mode... ");
		mda_text_mode();
		INFO("MDA Text Mode.");
	}
}

#endif
