
#ifndef MEM_TYPES_H
#define MEM_TYPES_

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

/**
* Intel 32 bit address breakdown as a union
*/
typedef union {

  uint32_t memloc;
  segoff_t segoff;
  uint16_t words[2];
  uint8_t bytes[4];
  void* ptr;

} mem_address_t;

#endif
