#ifndef DOS_SERVICES_3X_TYPES_H
#define DOS_SERVICES_3X_TYPES_H

#include <stdint.h>

typedef struct {

	uint16_t ecode;
	uint8_t eclass;
	uint8_t eaction;
	uint8_t elocus;
	const char* scode;
	const char* sclass;
	const char* saction;
	const char* slocus;

} dos3x_extended_error_information_t ;

#endif
