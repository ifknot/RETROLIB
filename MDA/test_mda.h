/**
 *
 *  @brief     The Monochrome Display Adapter
 *  @details   MDA, also MDA card, Monochrome Display and Printer Adapter, MDPA is IBM's 
 *  standard video display card and computer display standard for the IBM PC introduced in 1981. 
 *  The MDA does not have any pixel-addressable graphics modes, only a single monochrome text 
 *  mode which can display 80 columns by 25 lines of high-resolution text characters or symbols 
 *  useful for drawing forms.
 *  @author    Jeremy Thornton
 *  @date      25.05.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef TEST_MDA_H
#define TEST_MEM_ARENA_H

#include "../../TEST/debug_macros.h"

#include "../gfx_detect_crtc.h"

#include "mda.h"

namespace test_mda {

	void run() {
		INFO("* testing MDA card toolbox...");
		if(gfx::detect_CRTC_at_port(gfx::crtc_port_MDA));
		LOG(gfx::detect_CRTC_at_port(gfx::crtc_port_CGA));
		if (YESNO("* 811\ttest fill VRAM functions ?")) {
			
			if (YESNO("\t* 812\t fill vram ?")) {
				mda::fill_vram_buffer('+', mda::attribute::reverse);
			}
			if (YESNO("\t* 814\t fill characters ?")) {
				mda::fill_vram_characters(1);
			}
			if (YESNO("\t* 813\t fill attributes?")) {
				mda::fill_vram_attributes(mda::attribute::normal);
			}
			if (YESNO("\t* 811\t cls ?")) {
				mda::cls();
			}
		}
	}

}

#endif
