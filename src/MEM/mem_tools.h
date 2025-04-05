#ifndef MEM_TOOLS_H
#define MEM_TOOLS_H

#include <stdint.h>

#include "../DOS/dos_services_files_types.h"

/**
* @brief Use DOS INT 21h, Function 48h, to determine the amount of memory available to a user's program.
* @note By setting BX=FFFFh before calling, this function can be used to find the amount of
* available memory, which will be returned in BX. (The call will return an error, which can be ignored,
* since DOS cannot allocate more than 640k of memory.)
*/
uint16_t mem_available_low_paragraphs();

 /**
 * Offset Size		Description
 *	00   byte		'M' 4Dh  member of a MCB chain, (not last)
 *					'Z' 5Ah  indicates last entry in MCB chain
 *					other values cause "Memory Allocation Failure" on exit
 *	01   word		PSP segment address of MCB owner (Process Id)
 *					possible values:
 *						0 = free
 *						8 = Allocated by DOS before first user pgm loaded
 *						other = Process Id/PSP segment address of owner
 *	03   word		number of paras related to this MCB (excluding MCB)
 *	05	3bytes		reserved
 *	08  8bytes		ASCII program name, NULL terminated if less than max length (DOS 4.x+)
 *	10  nbytes	first byte of actual allocated memory block
 *
 * @note  An undocumented way to find the first or 'base' MemBlockRec to use DOS Fn 52H.
 */
void mem_dump_mcb(char* mcb);

/**
* @brief loads unformatted *upto* a page (64K bytes or 4096 paragraphs) of data from file to memory
* @note loads raw bytes
*
* @return dos_file_size_t - the actual byte count loaded
*/
dos_file_size_t mem_load_from_file(const char* path_name, char* start, uint16_t nbytes);

/**
* @brief saves unformatted *upto* a page (64K bytes or 4096 paragraphs) of memory to file
* @note saves raw bytes
*
* @return dos_file_size_t - the actual byte count saved
*/
dos_file_size_t mem_save_to_file(const char* path_name, char* start, uint16_t nbytes);


#endif
