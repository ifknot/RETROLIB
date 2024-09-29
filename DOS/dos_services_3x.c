/**
 *
 *  @file      dos_services_3x.cpp
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      30.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#include "dos_services_3x.h"

#include <stdint.h>
#include "dos_services_constants.h"
#include "dos_error_messages.h"

namespace dos3x {

    /**
         * @brief Get extended error information (3.x+)
         * AH = 59h
         * BX = 00 for versions  3.0, 3.1, 3.2
         * on return:
         * AX = extended error code (see DOS ERROR CODES)
         *    = 0 if no error
         * BH = error class
         * BL = suggested action
         * CH = locus
         *
         * - may be called after any  INT 21  function or from
         *   INT 24 when an error is returned
         * - must be called immediately after the error occurs
         * @note - registers CX, DX, DI, SI, BP, DS and ES are destroyed.
         *
         * @return extended_error_information_t error message, class, action, locus
         */
        void get_extended_error_information(extended_error_information_t* info) { // TODO: handle 22  Invalid disk change" ES:DI -> media ID structure
        __asm {
            .8086
            push    ds
            push    bp 

            xor     bx, bx                              ; BX = 0 DOS versions  3.0, 3.1, 3.2
            mov     ah, GET_EXTENDED_ERROR_INFORMATION
            int     DOS_SERVICE

            pop     bp
            pop     ds

            lds     di, info   
            mov     [di], ax
            mov     [di + 2], bh
            mov     [di + 3], bl
            mov     [di + 4], ch

        }

        info->scode = dos::error::messages[info->ecode];
        info->sclass = dos::error::classes[info->eclass];
        info->saction = dos::error::actions[info->eaction];
        info->slocus = dos::error::locus[info->elocus];

    }

}