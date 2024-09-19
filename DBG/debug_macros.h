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
* [x] DEFAULT_STDERR
* [ ] LOG
* [ ] LOGHEX
* [ ] LOGBIN
* [ ] ASSERT
* [ ] YESNO
* [ ] ANYKEY
*/

#include <stdio.h>

/*
* @brief __fprintf_as_binary(sizeof(x), x);
* @note Assumes little endian
* @url https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
*/
void __fprintf_as_binary(size_t const size, void const* const ptr);

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

#ifdef NDEBUG
#define LOG(var, spec)
#else
#define LOG(var, spec) do { \
					fprintf(stderr, "\n%s=" #spec, #var, var); \
				} while(0)
#endif

#ifdef NDEBUG
#define LOGBIN(var)
#else
#define LOGBIN(var) do { \
					__fprintf_as_binary(sizeof(var), &var); \
				} while(0)
#endif

#endif
