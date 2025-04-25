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

void hga_byte_scroll_left(uint16_t vram_segment, uint8_t byte_pattern) {
	__asm {
		.8086
		pushf								; preserve flags on entry (direction flag used)
		// setup registers
		cld
		mov		dx, vram_segment			; keep a copy of VRAM segment in DX to use with DS later
		mov 	ax, ds						; keep a copy of DATA segment in AX to use with DS later
		mov		es, dx						; ES:DI point to VRAM sgment also as destination
		mov 	bx, 0						; BX column ripple count

		// copy VRAM column right to left

END:
		popf
	}
}

void hga_pixel_scroll_up(uint16_t vram_segment, uint8_t byte_pattern) {
	__asm {
		.8086
		pushf								; preserve flags on entry (direction flag used)
		// setup registers
		mov 	bx, HGA_WORDS_PER_LINE		; BX column count
		mov 	dx, 5 //HGA_SCREEN_Y_MAX - 1 	; DX row count
		mov		ax, vram_segment
		mov		es, ax						; ES:DI will point to VRAM segment destination
		mov 	ds, ax						; DS:SI will point to VRAM segment destination
		mov 	ax, 0						; AX row ripple count
		cld									; increment string ops
		// copy VRAM line below to line above
		mov 	di, 0						; bank 0
		mov 	si, HGA_BANK_OFFSET			; bank 1
L1:		mov 	cx, bx						; CX column count
		rep 	movsw 						; copy words
		inc 	ax							; next line
		cmp		ax, dx
		je		BLANK
		add		di, HGA_BANK_OFFSET - HGA_BYTES_PER_LINE	; bank 1
		add		si, HGA_BANK_OFFSET - HGA_BYTES_PER_LINE	; bank 2
		mov 	cx, bx						; CX column count
		rep 	movsw 						; even - copy words
		inc 	ax
		cmp		ax, dx
		je		BLANK
		add		di, HGA_BANK_OFFSET - HGA_BYTES_PER_LINE	; bank 2
		add		si, HGA_BANK_OFFSET - HGA_BYTES_PER_LINE	; bank 3
		mov 	cx, bx						; CX column count
		rep 	movsw 						; copy words
		inc 	ax
		cmp		ax, dx
		je		BLANK
		add		di, HGA_BANK_OFFSET - HGA_BYTES_PER_LINE	; bank 3
		sub		si, HGA_BANK_OFFSET * 3						; bank 0
		mov 	cx, bx						; CX column count
		rep 	movsw 						; even - copy words
		inc 	ax
		cmp		ax, dx
		je		BLANK
		sub		di, HGA_BANK_OFFSET * 3
		jmp 	L1

		// draw blank line over last copied line ES:DI STOS
BLANK:	mov		al, byte_pattern
		mov 	ah, al
		mov 	di, 7E3Ch
		mov 	cx, bx						; CX column count
		//rep 	stosw 						; store AX
END:
		popf
	}
}

void hga_screen_scroll_up(uint16_t vram_segment, uint8_t byte_pattern) {
	for(uint16_t i = HGA_SCREEN_Y_MAX; i > 0; --i) {
		//hga_scroll_up(vram_segment, i, byte_pattern);
	}
}

/*

mov 	cx, bx						; CX column count
		test    cx, 1						; odd or even column count?
		jz     	J1							; even - use STOSW
		movsb								; odd - copy first byte
		dec 	cx 							; row count even
J1:		shr 	cx, 1						; column count / 2
		rep 	movsw 						; even - copy words
		cmp		ax, dx
		jne		L1

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
