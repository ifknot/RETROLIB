/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      30.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef TEST_DOS_3X_H
#define TEST_DOS_3X_H

#include "../TEST/debug_macros.h"

#include "dos_services_3x.h"

namespace test_dos_3x {

	void run() {
		INFO("* testing DOS 3.x+ services...");
		dos3x::extended_error_information_t info;
		dos3x::get_extended_error_information(&info);
		LOG(info.ecode);
		LOG((int)info.eclass);
		LOG((int)info.eaction);
		LOG((int)info.elocus);
		LOG(info.scode);
		LOG(info.sclass);
		LOG(info.saction);
		LOG(info.slocus);
	}

}

#endif