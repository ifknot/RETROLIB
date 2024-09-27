/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      30.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef DOS_SERVICES_3X_TYPES_H
#define DOS_SERVICES_3X_TYPES_H

#include <stdint.h>

namespace dos3x {

	struct extended_error_information_t {

		uint16_t ecode;
		uint8_t eclass;
		uint8_t eaction;
		uint8_t elocus;
		const char* scode;
		const char* sclass;
		const char* saction;
		const char* slocus;

	};

}

#endif