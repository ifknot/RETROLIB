/**
 *
 *  @file      main.c
 *  @brief     RETROLIB
 *  @details   DOS Game Library
 *  @author    Jeremy Thornton
 *  @date      17.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */

#include "DBG/debug_macros.h"

/**
* TODO:
* [ ] Q10_6_t 
* [ ] Q vectors
* [ ] actor model
* [ ] HGA plot
* [ ] particles 
* [ ] gravity
*/
int main() {

	int x = 11;
	float f = 11.1;
	if (YESNO("test?")) {
		REDIRECT_STDERR("log.txt");
		INFO(" file info ");
		DEFAULT_STDERR();
		INFO(" con info ");
		ANYKEY("any key");
		LOG(x, % d);
		LOG(x, % X);
		LOG(f, % 9.3f);
		PRESSENTER("");
		LOG(&x, % p);
		LOGBIN(x);
	}
	return 0;

}
