#ifndef MEM_ADDRESS_H
#define MEM_ADDRESS_H

#include <stdint.h>
#include <assert.h>

/**
* @brief sanity checking is available
*/
#define ASSERT_SEGOFF_T assert(sizeof(mem_segoff_t == 32));

/**
* @brief Intel 16 bit segment:offset memory type
* @note whilst C guarantees the ordering of the variables it makes no promises about size, padding or alignment
*/
typedef struct {           // size undefined and compiler dependent

    uint16_t offset;
    uint16_t segment;       // reverse order as Intel is little-endian

} mem_segoff_t;

/**
* @brief Intel 32 bit address breakdown as a union
* @note However, placing the struct into a union *should* force both (minimum) size and alignment
*/
typedef union {

  uint32_t memloc;
  mem_segoff_t segoff;
  uint16_t words[2];
  uint8_t bytes[4];
  void* ptr;

} mem_address_t;

#endif
