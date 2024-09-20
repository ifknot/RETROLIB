/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      17.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

/**
* TODO:
* [x] DEFAULT_STDERR
* [x] LOG
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
static void __fprintf_as_binary(size_t const size, void const* const ptr);

/**
* Helper functions user interaction
*/
static bool __yesno(char* msg);

static void __pressenter(char* msg);

static char __anykey(char* msg);

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

#ifdef NDEBUG
#define YESNO(msg)
#else
#define YESNO(msg) __yesno(msg)
#endif

#ifdef NDEBUG
#define PRESSENTER(msg)
#else 
#define PRESSENTER(msg) __pressenter(msg)
#endif

#ifdef NDEBUG
#define ANYKEY(msg)
#else
#define ANYKEY(msg) __anykey(msg)
#endif

#endif
