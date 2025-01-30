#include "hga_filled_rectangle.h" 

#include "hga_table_lookup_y.h"

void hga_filled_rectangle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t colour) {
	__asm {
		.8086
	    
END:
	}
}

/*
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
		jz 		Z0
		mov     ax, dx                                      ; proto-mask is white bits to 'colour'
Z0:	    jcxz    J0                                          ; lhs and rhs share same byte?
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
		// 7.1.2 work out fill 'colour'
		mov 	al, colour
		mov 	ah, al
		test 	al, al
		jz 		Z1
		mov     ax, 0FFFFh                                   ; AX white
Z1:     // 7.1.3 handle odd or even line lengths
		cld                                                  ; clear direction flag
		shr     cx, 1		                                 ; number of words to fill, lsb -> carry flag
		jnc     NC0                                          ; even so no byte to fill
		stosb	                                             ; odd do one byte al 'colour'
		jcxz    END
NC0:	// 7.1.4 remaining word(s) ax 'colour'
		rep     stosw		                                 ; CX is checked for !=0 before even the first step
        jmp 	END
J1:		// 7.2.0 special case same word (saves 48 clock cycles on 8086 line lengths 2 - 15)
        not     dx                                           ; convert proto-mask to mask word
        // 7.2.1 colour the shared lhs:rhs word                                       Clock Cycles
        and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		jmp 	END
J0:     // 7.3.0 special case same byte (saves 48 clock cycles on 8086 line lengths 0 - 7)
		and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
        // 7.3.1 colour the combined lhs&rhs byte for row height					Clock Cycles
		mov 	cx, h
L0:		and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)
*/
