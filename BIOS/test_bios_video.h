/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      4.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef TEST_BIOS_VIDEO_H
#define TEST_BIOS_VIDEO_H

#include "../TEST/debug_macros.h"
#include <iostream>

#include "bios_video_services.h"

namespace test_bios_video {

	void run() {
		INFO("* testing bios video services...");
		bios::video_state_t old_vinfo, new_vinfo;
		int new_mode;
		if (YESNO("* 311\ttest get video mode?")) {
			bios::get_video_state(&old_vinfo);
			INFO("bios::get_video_state(&old_vinfo)");
			LOG((int)old_vinfo.columns);
			LOG((int)old_vinfo.mode);
			LOG((int)old_vinfo.page);
			INFO(bios::video_mode_names[old_vinfo.mode]);
		}
		if (YESNO("* 312\ttest change video mode?")) {
			INFO("* Enter new mode: ");
			std::cin >> new_mode;
			bios::set_video_mode(new_mode);
			bios::get_video_state(&new_vinfo);
			INFO(bios::video_mode_names[new_vinfo.mode]);
		}
		if (YESNO("* 313\ttest restore video mode?")) {
			bios::set_video_mode(old_vinfo.mode);
			bios::get_video_state(&new_vinfo);
			INFO(bios::video_mode_names[new_vinfo.mode]);
		}
		if (YESNO("* 314\ttest EGA/VGA subsystem info?")) {
			bios::video_subsystem_config_t info;
			LOG_AS((int)bios::video_subsystem_configuration(bios::RETURN_VIDEO_CONFIGURATION_INFORMATION, 0, &info), std::hex);
			LOG((int)info.color_mode);
			LOG((int)info.ega_memory);
			LOG((int)info.feature_bits);
			LOG((int)info.switch_settings);
		}
		if (YESNO("* 315\ttest VGA enable access to video?")) {
			LOG_AS((int)bios::video_subsystem_configuration(bios::CPU_ACCESS_TO_VIDEO_RAM, 0), std::hex);
		}
	}

}

#endif