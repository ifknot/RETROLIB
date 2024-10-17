/**
 *
 *  @brief     The memory arena paradigm - one lifetime to bind them all.
 *  @details   Untangling lifetimes with absurdly simple linear allocator, lightning fast allocation and 
 *  deallocation, eliminating lifetime soup of per-allocation freeing/stack fragility/garbage collection requirements.
 *  Memory Arena the area where dynamic runtime memory is stored. 
 *  The memory arena consists of the heap and unused memory. 
 *  The heap is where all user-allocated memory is located. 
 *  The heap grows up from a lower memory address to a higher memory address.
 *  synonyms: region, zone, arena, area, or memory context.
 *  @note *By design* unable to free individual allocated memory blocks *only* the whole backing memory block arena 
 *  @url https://www.youtube.com/watch?v=3IAlJSIjvH0&t=4s
 *  @author    Jeremy Thornton
 *  @date      8.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef MEM_ARENA_H //const mem_arena_t default_ = { MEM_ARENA_POLICY_DOS, NULL, 0, 0 };
#define MEM_ARENA_H

#include "mem_constants.h" 
#include "mem_types.h"

typedef enum {
  MEM_ARENA_POLICY_DOS,
  MEM_ARENA_POLICY_C
} mem_arena_policy_t;

typedef struct private_mem_arena mem_arena_t;

mem_arena_t* mem_arena_new(mem_arena_policy_t policy, mem_size_t byte_request);

mem_size_t mem_arena_delete(mem_arena_t* arena);

mem_size_t mem_arena_size(mem_arena_t* arena);

mem_size_t mem_arena_capacity(mem_arena_t* arena);

void* mem_arena_alloc(mem_arena_t* arena, mem_size_t byte_request);

void* mem_arena_dealloc(mem_arena_t* arena, mem_size_t byte_request);

#endif
