/**
 *
 *  @file      mem_tools.c
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      19.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "mem_tools.h"

#include <stdio.h>

#include "mem_constants.h"

void mem_tools_dump_mcb(dos_mcb_t* mcb) {
	fprintf(stderr, "\nDOS MCB @%p\ntype\t%c\nowner\t@%X\nsize\t%i\nname\t%s", 
		mcb, mcb->signature, mcb->owner_id,mcb->size_paras,mcb->owner_name);
}
