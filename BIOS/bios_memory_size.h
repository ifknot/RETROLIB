/**

    @brief     
    @details   ~
    @author    Jeremy Thornton
    @date      21.11.2023
    @copyright © Jeremy Thornton, 2023. All right reserved.

**/
#ifndef BIOS_MEMORY_SIZE_H
#define BIOS_MEMORY_SIZE_H

#include <stdint.h>

#define BIOS_REPORT_MEMORY_SIZE             12h

namespace bios {

    /**
    * @brief INT 12h (18)             Report Memory Size
    * 
    * Reports the number of contiguous 1K memory blocks in the system (up to 640K).
    * On entry:      No parameters
    *
    * on return:       AX         Memory size (up to 640K)
    * 
    * @details This service determines memory by either examining the DIP switches on the system board or 
    * using the value stored in the CMOS battery, depending on the type of machine.
    * 
    * The value returned in AX is the same as that stored at memory location 0:0413h, which is initialized
    * during the power-up tests - this is the amount of memory available to the entire system. 
    * 
    * @note 1. This is *not* the amount of memory available to the user's program. 
    * Use DOS INT 21h, Function 48h, to determine the amount of memory available to a user's program.
    *
    * @note 2. To determine the amount of memory above the 1024 address range, use INT 15h, Service 88h.
    * 
    * @note 3. If the CMOS battery memory value or the DIP switch value is greater than the actual amount of
    * memory, then the actual amount of memory is returned by this interrupt.
    */
    uint16_t memory_size_KiB();

}

#endif