/**
 *
 *  @file      mda_toolbox.cpp
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      25.05.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "mda_toolbox.h"

namespace mda {


    void fill_vram_buffer(char ascii, attribute_t attrib) {
        __asm {
            .8086
            pushf

            mov             ax, MDA_VIDEO_RAM_SEGMENT
            mov             es, ax                  ; ES:DI will point to x, y screen byte
            xor             di, di
            mov             ah, attrib              ; 'normal' attribute
            mov             al, ascii               ; default 'black'
            mov             cx, CHARS_PER_SCREEN    ; 80 columns x 25 rows
            cld                                     ; increment mode
            rep             stosw                   ; fill screen

            popf
        }
    }

	void fill_vram_attributes(attribute_t attrib) {
        __asm {
            .8086

            mov             ax, MDA_VIDEO_RAM_SEGMENT
            mov             es, ax                  ; ES:DI will point to x,y screen byte
            xor             di, di
            mov             al, attrib
            mov             cx, CHARS_PER_SCREEN    ; 80 columns x 25 rows
            cld                                     ; increment mode
    L0:     inc             di                      ; skip ascii byte
            stosb                                   ; set attribute byte
            loop    L0                              ; whole screen
        }
    }

    void fill_vram_characters(char ascii) {
        __asm {
            .8086

            mov             ax, MDA_VIDEO_RAM_SEGMENT
            mov             es, ax                  ; ES:DI will point to x,y screen byte
            xor             di, di
            mov             al, ascii
            mov             cx, CHARS_PER_SCREEN    ; 80 columns x 25 rows
            cld                                     ; increment mode
    L0:     stosb                                   ; set ascii byte
            inc             di                      ; skip attribute byte
            loop    L0                              ; whole screen
        }
    }

}