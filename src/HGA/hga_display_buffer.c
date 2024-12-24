#include "hga_display_buffer.h"
#include "hga_constants.h"
#include "hga_table_lookup_y.h"

void hga_select_display_buffer(uint8_t select) {
    __asm {
      .8086

      mov     dx, HGA_CONTROL_REGISTER
      mov 	  al, select
      and 	  al, 00000001b				; only bit 0 selects buffer
      //jz      J0
      mov     al, 10001010b     		; screen on buffer 1 second display page buffer B000 : 800
      jmp     J1
J0:   mov     al, 00001010b     		; screen on buffer 0 default display page buffer B000 : 000
J1:   out     dx, al

    }
}

void hga_write_vram_buffer_lookup(uint16_t vram_segment, uint16_t x, uint16_t y, uint8_t byte_pattern) {

    __asm {
		.8086

		mov   ax, vram_segment
		mov   es, ax
		mov   bx, y
		add   bx, bx                                            ; y * 2 as word lookup table
		mov   di, HGA_TABLE_Y_LOOKUP[bx]                        ; lookup y offset
		add   di, x                                             ; add in x
        mov   al, byte_pattern
        mov   es:[di], al

	}
}

uint8_t hga_read_vram_buffer_lookup(uint16_t vram_segment, uint16_t x, uint16_t y) {
	uint8_t byte_pattern;
	__asm {
		.8086

		mov   ax, vram_segment
		mov   es, ax
		mov   bx, y
		add   bx, bx                                            ; y * 2 as word lookup table
		mov   di, HGA_TABLE_Y_LOOKUP[bx]                        ; lookup y offset
		add   di, x                                             ; add in x
        mov   al, es:[di]
        mov   byte_pattern, al

	}
	return byte_pattern;
}

void hga_fill_vram_buffer(uint16_t vram_segment, uint8_t byte_pattern) {
	__asm {
		.8086

		// 1. setup HGA quad bank VRAM destination pointer ES:DI
		xor 	di, di						; top left screen(0, 0)
		mov		ax, vram_segment
		mov		es, ax						; ES:DI point to VRAM destination
		// 2. set up the registers
		cld									; increment ES:DI
		mov		al, byte_pattern
		mov		ah, al						; duplicate byte pattern into AX word
		mov		cx, HGA_WORDS_PER_SCREEN	; set counter to full screen
		// 3. move the byte:byte pattern to VRAM
		rep		stosw						; chain store byte pattern word to VRAM

	}
}

void hga_scroll_up(uint16_t start, uint16_t lines) {

}
