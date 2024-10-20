/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      19.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef MEM_TOOLS_H
#define MEM_TOOLS_H

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
void mem_tools_dump_mcb(char* mcb);

//void mem_tools_dump_psp(char** psp);

#endif
