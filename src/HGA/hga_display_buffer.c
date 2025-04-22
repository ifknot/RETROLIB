#include "hga_display_buffer.h"

#include "../MEM/mem_address.h"
#include "../MEM/mem_tools.h"

#include "hga_constants.h"
#include "hga_table_lookup_y.h"

void hga_select_display_buffer(uint16_t vram_segment) {
    __asm {
        .8086
		// 1. set up the registers
        mov     dx, HGA_CONTROL_REGISTER
        mov     ax, vram_segment
        cmp     ax, HGA_BUFFER_0
        je      J0
        cmp     ax, HGA_BUFFER_1
        je      J1
        jmp     END
		// 2. write the HGA control registers
J0:     mov     al, 00001010b     		; screen on buffer 0 default display page buffer B000 : 000
        out     dx, al
        jmp     END
J1:     mov     al, 10001010b     		; screen on buffer 1 second display page buffer B000 : 800
        out     dx, al
        jmp     END
END:
    }
}

void hga_write_vram_buffer_lookup(uint16_t vram_segment, uint16_t byte_column, uint16_t byte_row, uint8_t byte_pattern) {
    __asm {
		.8086

		mov   ax, vram_segment
		mov   es, ax
		mov   bx, byte_row
		add   bx, bx                                            ; row * 2 as word lookup table
		mov   di, HGA_TABLE_Y_LOOKUP[bx]                        ; lookup y offset
		add   di, byte_column                                   ; add in column
        mov   al, byte_pattern
        mov   es:[di], al

	}
}

uint8_t hga_read_vram_buffer_lookup(uint16_t vram_segment, uint16_t byte_column, uint16_t byte_row) {
	uint8_t byte_pattern;
	__asm {
		.8086
		// 1. setup HGA quad bank VRAM destination pointer ES:DI
		mov   ax, vram_segment
		mov   es, ax
		// 2. set up the registers
		mov   bx, byte_row
		add   bx, bx                                            ; row * 2 as word lookup table
		mov   di, HGA_TABLE_Y_LOOKUP[bx]                        ; lookup y offset
		add   di, byte_column                                   ; add in column
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

void hga_knit_vram_buffer(uint16_t vram_segment, uint8_t byte_pattern_a, uint8_t byte_pattern_b) {
    __asm {
		.8086
		// 1. setup HGA quad bank VRAM destination pointer ES:DI
		xor 	di, di						; top left screen(0, 0)
		mov		ax, vram_segment
		mov		es, ax						; ES:DI point to VRAM destination
		// 2. set up the registers
		cld									; increment mode
		mov		al, byte_pattern_a
		mov		ah, al						; duplicate byte pattern into AX word
		mov     bl, byte_pattern_b
		mov     bh, bl
		// 3.0 fill bank 0 pattern a
		mov     cx, HGA_WORDS_PER_BANK
		rep     stosw
		xchg    ax, bx
		// 3.0 fill bank 1 pattern b
		mov     cx, HGA_WORDS_PER_BANK
		rep     stosw
		xchg    ax, bx
		// 3.0 fill bank 2 pattern a
		mov     cx, HGA_WORDS_PER_BANK
		rep     stosw
		xchg    ax, bx
		// 3.0 fill bank 3 pattern b
		mov     cx, HGA_WORDS_PER_BANK
		rep     stosw

	}
}

dos_file_size_t  hga_load_vram_buffer(uint16_t vram_segment, const char* file_path) {
    mem_address_t start;
    start.segoff.segment = vram_segment;
    start.segoff.offset = 0;
    return mem_load_from_file(file_path, start.ptr, HGA_DISPLAY_PAGE_SIZE);
}

dos_file_size_t  hga_save_vram_buffer(uint16_t vram_segment, const char* file_path) {
    mem_address_t start;
    start.segoff.segment = vram_segment;
    start.segoff.offset = 0;
    return mem_save_to_file(file_path, start.ptr, HGA_DISPLAY_PAGE_SIZE);
}

void hga_scroll_up(uint16_t vram_segment, uint16_t lines, uint8_t byte_pattern) {
	__asm {
		.8086
		pushf								; preserve flags on entry (direction flag used)
		// setup registers 
		cld
		mov		ax, vram_segment
		mov		es, ax						; ES:DI point to VRAM destination
		mov		ds, ax						; DS:SI point to VRAM source
		mov 	bx, 0						; BX = line counter
		mov 	di, HGA_TABLE_Y_LOOKUP[bx]
		inc		bx
		mov 	si, HGA_TABLE_Y_LOOKUP[bx]
		rep 	movsw 
	
	
		// draw blank line at bottom of screen
		mov 	di, 0x7E3C 					; last pixel row VRAM offset
		mov 	cx, HGA_WORDS_PER_LINE 
		mov     al, byte_pattern
		mov 	ah, al
		rep		stosw

		popf 
	}
}

void hga_screen_scroll_up(uint16_t vram_segment, uint8_t byte_pattern) {
	for(uint16_t i = HGA_SCREEN_Y_MAX; i > 0; --i) {
		hga_scroll_up(vram_segment, i);
	}
}

/*
__asm {
		.8086
		pushf							; preserve flags on entry (direction flag used)
		// 1. skip if lines = 0
		mov 	cx, 1
		jcxz	END
		// 2. initialise registers
		mov		ax, vram_segment
		mov		es, ax					; ES:DI point to VRAM destination
		mov		ds, ax					; DS:SI point to VRAM source
		mov 	bx, 0						; BX = line counter
		// 3. loop over the number of lines to scroll up
L1:		mov 	dx, cx 						; copy of number of lines to scroll
		// 4. setup HGA quad bank VRAM *destination* pointer ES:DI
		mov 	di, HGA_TABLE_Y_LOOKUP[bx]
		inc 	bx 							; next line
		// 5. setup HGA quad bank VRAM *source* pointer DS:SI
	  	mov		si, HGA_TABLE_Y_LOOKUP[bx]
		inc 	bx							; next line
		// 6. repeat string operation copy line "below" (taking into account HGA quad bank VRAM) to line above as 45 words
		mov 	cx, HGA_WORDS_PER_LINE
		mov     ax, 0AAAAh
		rep 	stosw //movsw
		mov 	cx, dx
		loop 	L1
		// 7. write a blank last line
		mov 	ax, 0						; black
		mov 	bx, lines 					; last line
		mov 	di, HGA_TABLE_Y_LOOKUP[bx]
		mov 	cx, HGA_WORDS_PER_LINE
		rep 	stosw
	
END:    	popf
	}
*/
