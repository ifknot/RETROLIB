#ifndef MEM_TYPES_H
#define MEM_TYPES_H

#include <stdint.h>

typedef uint32_t	mem_size_t;

typedef int32_t		mem_diff_t;

/**
 * Intel 16 bit segment:offset memory type
 */
typedef struct {

    uint16_t offset;
    uint16_t segment;       // reverse order as Intel is little-endian

} mem_segoff_t;

static const mem_segoff_t default_mem_segoff_t = { 0, 0 };

/**
* Intel 32 bit address breakdown as a union
*/
typedef union {

    char* ptr;
    uint32_t memloc;
    mem_segoff_t segoff;
    uint16_t words[2];
    uint8_t bytes[4];

} mem_address_t;

#endif
