#include "hga_fast_line.h"

#include "hga_table_lookup_y.h"

void hga_hline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour) {
	__asm {
		.8086
		cld                                                 ; clear direction flag
	    // 1. set up VRAM segment in ES
		mov   	ax, vram_segment
		mov   	es, ax
		// 2. lookup y and setup ES:DI point to target row
		mov 	bx, y1										; BX load y1
	    shl     bx, 1                                       ; convert BX word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bx]					; lookup y offset
		// 3. set up registers
		mov 	bx, x1										; BX load x1
		mov 	ax, x2 										; AX load x2
		// 4. build lsh & rsh proto-masks
		mov 	dx, 0FFFFh 									; DL lhs DH rhs proto-masks (little endian)
		mov 	cx, bx										; copy x1
		and 	cx, 7h			                           	; CX is x1 mod 8
		shr 	dl, cl										; shift lhs proto mask to starting pixel
		mov 	cx, ax										; copy x2
		and 	cx, 7h			                           	; CX is x2 mod 8
		xor 	cx, 7h										; convert to bits to shift left
		shl 	dh,cl 										; shift rhs proto mask to ending pixel
		// 5. reduce x1 and x2 to column bytes
		shr		bx, 1			                           	; calculate column byte x1 / 8
	    shr		bx, 1			                            ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		bx, 1
		shr		ax, 1			                           	; calculate column byte x2 / 8
	    shr		ax, 1
	    shr		ax, 1
		// 6. calculate line length in bytes
		mov 	cx, ax
		sub 	cx, bx										; CX line length (bytes)
		// 7.0 work out 'colour' bits into al AND ah
		mov 	al, colour
		mov 	ah, al
		test 	al, al
		jz 		BLK                                         ; branching to hard code 'colour' saves a few cycle
		mov     ax, dx                                      ; proto-mask is white bits to 'colour'
WHT:    jcxz    J0                                          ; lhs and rhs share same byte?
        dec     cx
        jcxz    J1                                          ; lhs and rhs share same word?
        // 7.1.0 general case
		not 	dx											; convert proto-mask to mask word
		// 7.1.1 colour lhs and rhs line
		add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out target bits 	- 16 + EA(8)
		or      es:[di], al                            		; colour target bits	- 16 + EA(8)
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], ah                            ; colour target bits	- 16 + EA(8)
		// fill 'colour'
		mov     ax, 0FFFFh                                   ; AX white
        // 7.1.3 handle odd or even line lengths
		shr     cx, 1		                                 ; number of words to fill, lsb -> carry flag
		jnc     NC1                                          ; even so no byte to fill
		stosb	                                             ; odd do one byte al 'colour'
		jcxz    END
NC1:	// 7.1.4 remaining word(s) ax 'colour'
		rep     stosw		                                 ; CX is checked for !=0 before even the first step
        jmp 	END
BLK:	jcxz    J0                                          ; lhs and rhs share same byte?
        dec     cx
        jcxz    J1                                  	    ; lhs and rhs share same word?
		not 	dx										 	; convert proto-mask to mask word
		add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out black bits
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out black bits
		mov     ax, 0                                       ; AX black
		shr     cx, 1		                                ; number of words to fill, lsb -> carry flag
		jnc     NC0                                         ; even so no byte to fill
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    END
NC0:	rep     stosw		                                ; CX is checked for !=0 before even the first step
        jmp 	END
J1:		// 7.2.0 special case same word (saves 48 clock cycles on 8086 line lengths 2 - 15)
        not     dx                                          ; convert proto-mask to mask word
        // 7.2.1 colour the shared lhs:rhs word                                       Clock Cycles
        and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		jmp 	END
J0:     // 7.3.0 special case same byte (saves 48 clock cycles on 8086 line lengths 0 - 7)
		and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
        // 7.3.1 colour the combined lhs&rhs byte										Clock Cycles
		and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)
END:
	}
}

void hga_vline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour) {
	__asm {
		.8086
	    // 1. set up VRAM segment in ES
	    mov   	ax, vram_segment
		mov   	es, ax
		// 2. setup registers
		mov   	dh, 00000001                                ; DH is (proto)mask byte
		mov     dl, colour                                  ; DL load 'colour'
		mov		ax, x1			                           	; AX load x
        mov		cx, ax			                           	; CX copy of x
        and		cx, 7h			                           	; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)										; rotate mask bit by x mod 8
		xor     cx, 7h                                      ; convert to bits to shift left
        // 3. setup pixel and mask
	    shl		dx, cl			                           	; shift colour bit & proto-mask into position
		not     dh                                          ; convert to mask
		// 4. convert x to column byte
	    shr		ax, 1			                           	; calculate column byte x / 8
	    shr		ax, 1			                           	; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		// 5. setup y loop and lookup pointer
		mov 	bx, y1                                      ; BX load y1
		mov 	cx, y2                                      ; CX load y2
		sub 	cx, bx										; convert CX line length
        shl     bx, 1                                       ; convert BX word pointer
		// 5. lookup y and setup ES:DI point to target byte
L0:	    mov   	di, HGA_TABLE_Y_LOOKUP[bx]                  ; lookup y offset
		add   	di, ax                                      ; add in x / 8
		add 	bx, 2 										; next line
	    // 6. colour the selected pixel
		and		es:[di], dh								    ; mask out target pixel
		or 		es:[di], dl									; or in the 'colour'
		loop 	L0                                          ; for line length
	}
}
