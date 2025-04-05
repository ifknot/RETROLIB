#include "dos_services.h"

#include <stdio.h>

#include "dos_services_constants.h"
#include "dos_services_types.h"
#include "dos_error_messages.h"

/**
* @brief Provides a safe method for changing interrupt vectors
* @note returns nothing
* INT 21,25 - Set Interrupt Vector
* AH = 25h
* AL = interrupt number
* DS:DX = pointer to interrupt handler
*/
void dos_set_interrupt_vector(uint8_t vec_num, void* phandler) {
    __asm {
        .8086
        pushf                                ; preserve what int 21h may not
        push    ds                           ; due to unreliable behaviour

        lds     dx, phandler                ; copy pointer to handler into DS:DX
        mov     al, vec_num                 ; interrupt vector number
        mov     ah, DOS_SET_INTERRUPT_VECTOR    ; 25h service
        int     DOS_SERVICE

        pop     ds
        popf
    }
}

/**
* @brief Standard method for retrieving interrupt vectors
* INT 21,35 - Get Interrupt Vector
* AH = 35h
* AL = interrupt vector number
* on return:
* ES:BX = pointer to interrupt handler.
*
* @param vec_num
* @return void* segment:offset pointer to interrupt handler
*/
void* dos_get_interrupt_vector(uint8_t vec_num) {
    void* phandler = 0;
    __asm {
        .8086
        pushf
        push    ds

        mov     al, vec_num                 ; interrupt vector number
        mov     ah, DOS_GET_INTERRUPT_VECTOR    ; 35h service
        int     DOS_SERVICE
        lea     di, phandler
        mov     [di], bx                    ; copy segment into address_t (little endian)
        mov     [di + 2] , es               ; copy offset

        pop     ds
        popf
    }
    return phandler;
}

/**
* @brief INT 21,48 - Allocate Memory
* Allocates a specified number of memory paragraphs.
*
* AH = 48h
* BX = number of memory paragraphs requested
* @note paragraph is a memory unit of size 16 bytes,  relevant primarily (if not exclusively) in x86 real mode
* @url https://en.wikipedia.org/wiki/X86_memory_segmentation
* on return:
* AX = segment address of allocated memory block (MCB + 1para)
*    = error code if CF set  (see DOS ERROR CODES)
* BX = size in paras of the largest block of memory available
*      if CF set, and AX = 08 (Not Enough Mem)
* CF = 0 if successful
*    = 1 if error
*
* Error codes:   7          Memory control blocks destroyed
*                8          Insufficient memory
* @note 1. Call Function 59h for extended error code information (DOS 3.0 and above).
*
* - returns segment address of allocated memory block AX:0000
* - each allocation requires a 16 byte overhead for the MCB
* - returns maximum block size available if error
*
* @note 2. By setting BX=FFFFh before calling, this function can be used to find the amount of
* available memory, which will be returned in BX. (The call will return an error, which can be
* ignored, since DOS cannot allocate more than 640k of memory.)
*
* @see  INT 21,49,  INT 21,4A
*
* @param       number of paragraphs (16 bytes) requested
* @param       segment* pointer to segment variable
* @return      the segment address of the reserved memory or 0 if request failed
*/
uint16_t dos_allocate_memory_blocks(uint16_t paragraphs) {
    uint16_t available, mem_seg;
    dos_error_code_t err_code;
    available = mem_seg = err_code = 0;
    __asm {
    .8086
    pushf
    push    ds

    mov     bx, paragraphs              ; number requested paragraphs
    mov     ah, DOS_ALLOCATE_MEMORY_BLOCKS  ; allocate memory
    int     DOS_SERVICE                 ; 48h service
    jnc     OK                          ; success CF = 0
    mov     err_code, ax                ; CF set, and AX = 08 (Not Enough Mem)
    mov     available, bx               ; size in paras of the largest block of memory available
    xor     ax, ax
OK: mov     mem_seg, ax

    pop     ds
    popf
    }
#ifndef NDEBUG
    if (err_code) {
        fprintf(stderr, "%s\n", dos_error_messages[err_code]);
        if (err_code == DOS_INSUFFICIENT_MEMORY) {
            fprintf(stderr, " largest block of memory available = %u paragraphs\n", available);    // paragraph = 16 bytes
        }
    }
#endif
    return mem_seg;
}

/**
* @brief INT 21,49 - Free Allocated Memory
*
* AH = 49h
* ES = segment of the block to be returned (MCB + 1para)
*
* on return:
* AX = error code if CF set  (see DOS ERROR CODES)
*
* - releases memory and MCB allocated by INT 21,48
* - may cause unpredictable results if memory wasn't allocated using INT 21,48
* or if memory wasn't allocated by the current process
* - checks for valid MCB id, but does NOT check for process ownership care must be
* taken when freeing the memory of another process to assure the segment isn't in use
* by a TSR or ISR
* @note this function is unreliable in a TSR once resident, since COMMAND.COM and many
*  other .COM files take all available memory when they load.
* @see  INT 21,4A
*/
uint16_t dos_free_allocated_memory_blocks(uint16_t segment) {
    dos_error_code_t err_code = 0;
    __asm {
        .8086
        pushf
        push    ds

        mov     ax, segment                         ; the segment to be released
        mov     es, ax                              ; segment of the block to be returned(MCB + 1para)
        mov     ah, DOS_FREE_ALLOCATED_MEMORY_BLOCKS    ; de-allocate memory
        int     DOS_SERVICE                         ; dos call 49h
        jnc     OK                                  ; success CF = 0
        mov     err_code, ax                        ; de-allocation failed ax is dos error code
    OK:
        pop     ds
        popf
    }
#ifndef NDEBUG
    if (err_code) {
        fprintf(stderr, "%s %X\n", dos_error_messages[err_code], segment);
    }
#endif
    return err_code;
}
