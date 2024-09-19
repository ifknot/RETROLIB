/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      17.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef JOT_MACROS_H
#define JOT_MACROS_H

/**
* TODO:
* [ ] DEFAULT_STDERR
* [ ] LOG
* [ ] LOGHEX
* [ ] LOGBIN
* [ ] ASSERT
* [ ] YESNO
* [ ] ANYKEY
*/

#include <stdio.h>

#ifdef NDEBUG
#define REDIRECT_STDERR(path)
#else
#define REDIRECT_STDERR(path) do { \
					freopen(path, "w", stderr); \
				} while(0)
#endif

#ifdef NDEBUG
#define DEFAULT_STDERR(path)
#else
#define DEFAULT_STDERR() do { \
					freopen("CON","w",stderr); \
				} while(0)
#endif

#ifdef NDEBUG
#define INFO(msg)
#else
#define INFO(msg) do { \
					fprintf(stderr,"\n%s",msg); \
				} while(0)
#endif

#endif
