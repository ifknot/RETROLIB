/**
 *
 *  @file      mem_arena.c
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      17.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include <stdlib.h>
#include <stdio.h>

#include "mem_arena.h"

#include "../DOS/dos_services.h"

typedef struct private_mem_arena {  

  uint8_t		policy;		// e.g. MEM_POLICY_DOS or MEM_POLICY_C
  mem_address_t	start;		// base address of the arena
  char*			free;		// pointer to start of free memory within arena, initially free = start
  char*			end;		// end address limit of useable arena memory 

};

const mem_arena_t default_dos_mem_arena_t = { MEM_ARENA_POLICY_DOS, NULL, NULL, NULL };

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
	mem_arena_t* arena = (mem_arena_t*)malloc(sizeof(mem_arena_t));
	mem_size_t paragraphs = byte_count / PARAGRAPH_SIZE;	// calculate the number of paragraphs to request fron DOS				
	if (byte_count % PARAGRAPH_SIZE) {						// if mod 16 then need another paragraph for the remainder
		paragraphs++;
	}
	*arena = default_dos_mem_arena_t;
	arena->start.segoff.segment = dos_allocate_memory_blocks(paragraphs);	// ask DOS for the memory 
	if (arena->start.segoff.segment) {								// success DOS could fulfill the memory request				
		arena->free = arena->start.ptr;
		arena->end = arena->start.ptr + paragraphs * PARAGRAPH_SIZE;
	}
#ifndef NDEBUG
	else {
		fprintf(stderr, "ERROR memory request %li too large for DOS to provide!", byte_count);
	}
#endif
	return arena;
}

mem_size_t private_mem_arena_dos_delete(mem_arena_t* arena) {
	mem_size_t freed = mem_arena_capacity(arena);					// capture the capacity of the arena
	dos_free_allocated_memory_blocks(arena->start.segoff.segment);	// ask DOS to free the memory block
	free(arena);													// free up heap memory
	return freed;													// return amount freed up
}

mem_arena_t* mem_arena_new(mem_arena_policy_t policy, mem_size_t byte_request) {
	switch(policy) {
		case MEM_ARENA_POLICY_DOS:
			return private_mem_arena_dos_new(byte_request);
		case MEM_ARENA_POLICY_C:	// to do
		default:
			fprintf(stderr, "Invalid memory policy type %i", policy);
			return NULL;
	}
}

mem_size_t mem_arena_delete(mem_arena_t* arena) {
	switch(arena->policy) {
		case MEM_ARENA_POLICY_DOS:
			return private_mem_arena_dos_delete(arena);
		case MEM_ARENA_POLICY_C:	// to do
		default:
			fprintf(stderr, "Invalid memory policy type %i", arena->policy);
			return 0;
	}
}

mem_size_t mem_arena_size(mem_arena_t* arena) {
	return arena->end - arena->free;
}

mem_size_t mem_arena_capacity(mem_arena_t* arena) {
	return arena->end - arena->start.ptr;
}

void* mem_arena_alloc(mem_arena_t* arena, mem_size_t byte_request) {
	/*
	char* p = NULL;							// default return to null
	if (byte_request <= arena->size) {		// can fulfill request 
		arena->size -= byte_request;		// shrink pool size
		p = arena->pfree;					// initialize return value points to requested block
		arena->pfree += byte_request;		// now point to the start of remaining free memory 
	}
#ifndef NDEBUG
	else {
		fprintf(stderr, "ERROR memory request %li bytes too large for ARENA to ALLOC!\nLargest block of memory available %li bytes.", byte_request, arena->size);
	}
#endif			
	return p;
	*/
	return 0;
}

void* mem_arena_dealloc(mem_arena_t* arena, mem_size_t byte_request) {
	/*
	char* p = NULL_PTR;						// default return to null
	if (byte_request <= arena->capacity) {	// can fulfill request 
		arena->size += byte_request;		// grow pool size
		arena->pfree -= byte_request;		// now point to the start of enlarged free memory
		p = arena->pfree;					// initialize return value points to resized pool 
	}
#ifndef NDEBUG
	else {
		fprintf(stderr, "ERROR memory request %li bytes too large for ARENA to DEALLOC!\nLargest block of memory available %li bytes.",byte_request , arena->capacity);
	}
#endif			
	return p;*/
	return 0;
}
