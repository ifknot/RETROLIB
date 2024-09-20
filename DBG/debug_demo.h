/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      20.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef DEBUG_DEMO_H
#define DEBUG_DEMO_H

#include "DBG/debug_macros.h"

void debug_demo() {

	int x = 11;
	float f = 11.1;
	if (YESNO("redirect stderr to LOG.TXT?")) {
		REDIRECT_STDERR("LOG.TXT");
		INFO(" file info ");
		DEFAULT_STDERR();
	}
	INFO(" con info ");
	ANYKEY("any key");
	LOG(x, % d);
	LOG(x, % X);
	LOG(f, % 9.3f);
	PRESSENTER("");
	LOG(&x, % p);
	LOGBIN(x);

}

#endif