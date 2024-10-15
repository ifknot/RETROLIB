



#include "mem_arena.h"

typedef struct {  

  char           type;      // e.g. MEM_POLICY_DOS or MEM_POLICY_C
  char           version;   // BCD 0.1
  char*          pfree;     // pointer to the start of free memory pool
  uint32_t       size;		  // current amount of free memory (bytes) pool 	
  mem_address_t  pool;			// base address of the DOS memory block used by the arena pool
  uint32_t       capacity;  // starting size of the memory (bytes) pool

} mem_arena_t;

mem_arena_t* mem_arena_new(mem_policy_t policy, uint32_t size);

uint32_t mem_arena_delete(mem_arena_t* arena);

uint32_t mem_arena_size(mem_arena_t* arena);

uint32_t mem_arena_capacity(mem_arena_t* arena);

void* mem_arena_alloc(mem_arena_t* arena, uint32_t size);

void* mem_arena_dealloc(mem_arena_t* arena, uint32_t size);
