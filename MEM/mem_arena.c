



#include "mem_arena.h"

#include "../DOS/dos_services.h"

// private
typedef struct {  

  uint8_t             type;      // e.g. MEM_POLICY_DOS or MEM_POLICY_C
  uint8_t             version;   // BCD 0.1
  void*               pfree;     // pointer to the start of free memory pool
  mem_size_t          size;	 // current size available free memory (bytes) pool 	
  mem_address_t       pool;	 // base address of the DOS memory block used by the arena pool
  mem_size_t          capacity;  // total allotted memory (bytes) pool

} mem_arena_t;

/**
* The DOS INT 21,48 - Allocate Memory function takes a 16 bit word size request in paragraphs (16 bytes)
* and returns a memory segment address as a 16 bit value.
* Because the request is a 16 bit word the, *theoretical*, maximum number of paragraphs requestable to
* allocate is 65535 paragraphs or 65535 x 16 = 1048560 bytes.
* Which is 16 bytes, one paragraph, less than 1 Mebibyte (MiB)
* @note 1MiB Max the original 8086/8088 microprocessors' 20-bit address bus can address 1,048,576 bytes.
* However, the actual amount of allocatable RAM is restricted 640K or less (+ possible upper memory area)
* and will depend on installed RAM.
*/
mem_arena_t* private_mem_arena_dos_new(mem_size_t byte_count) {
  mem_arena_t* arena = new(arena_t);
  arena->pfree = NULL_PTR;								// setup default values...
  arena->pool.memloc = arena->size = arena->capacity = 0;
  mem_size_t paragraphs = byte_count / PARAGRAPH_SIZE;	// calculate the number of paragraphs to request fron DOS
  if (byte_count % PARAGRAPH_SIZE) {		// if mod 16 then need another paragraph for the remainder
    paragraphs++;
  }
  arena->pool.segoff.segment = dos::allocate_memory_blocks(paragraphs);	// ask DOS for the memory 
  if (arena->pool.segoff.segment) {						// success DOS could fulfill the memory request				
    arena->pfree = (char*)arena->pool.memloc;			// initialize values...
    arena->size = arena->capacity = paragraphs * PARAGRAPH_SIZE;
  }
#ifndef NDEBUG
  else {
	fprintf(stderr, "ERROR memory request %li too large for DOS to provide!", byte_count);
  }
#endif
  return arena;
}

mem_size_t private_mem_arena_dos_delete(arena_t* arena) {
	mem_size_t sz = arena->capacity;						// capture the capacity of the arena
	dos::free_allocated_memory_blocks(arena->pool.segoff.segment);	// ask DOS to free the memory block
	delete arena;											// free up arena_t memory
	return sz;												// return amount freed up
}

mem_arena_t* mem_arena_new(mem_policy_t policy, uint32_t size) {
	switch(policy) {
		case MEM_POLICY_DOS:
			return private_mem_arena_dos_new(size);
		default:
			fprintf(stderr, "Invalid memory policy type %i", policy);
			return 0;
	}
}

mem_size_t mem_arena_delete(mem_arena_t* arena) {
	switch(arena->policy) {
		case MEM_POLICY_DOS:
			return private_mem_arena_dos_delete(arena);
		default:
			fprintf(stderr, "Invalid memory policy type %i", policy);
			return 0;
	}
}

mem_size_t mem_arena_size(mem_arena_t* arena) {
	return arena->size;
}

mem_size_t mem_arena_capacity(mem_arena_t* arena) {
	return arena->capacity;
}

void* mem_arena_alloc(mem_arena_t* arena, mem_size_t size) {

}

void* mem_arena_dealloc(mem_arena_t* arena, mem_size_t size) {

}
