#include "hga_filled_rectangle.h"

#include "hga_table_lookup_y.h"

void hga_filled_rectangle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t colour) {
    __asm {
		.8086
		cld                                                  ; clear direction flag
		mov   	ax, vram_segment
		mov   	es, ax
		mov 	bx, x1										; BX load x1
		mov 	ax, x2 										; AX load x2
		mov 	dx, 0FFFFh 									; DL lhs DH rhs proto-masks (little endian)
		mov 	cx, bx										; copy x1
		and 	cx, 7h			                           	; CX is x1 mod 8
		shr 	dl, cl										; shift lhs proto mask to starting pixel
		mov 	cx, ax										; copy x2
		and 	cx, 7h			                           	; CX is x2 mod 8
		xor 	cx, 7h										; convert to bits to shift left
		shl 	dh,cl 										; shift rhs proto mask to ending pixel
		shr		bx, 1			                           	; calculate column byte x1 / 8
	    shr		bx, 1			                            ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		bx, 1
		shr		ax, 1			                           	; calculate column byte x2 / 8
	    shr		ax, 1
	    shr		ax, 1
		mov 	cx, ax
		sub 	cx, bx										; CX line length (bytes)
		mov 	si, y1										; SI load y1
	    shl     si, 1                                       ; convert SI word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[si]					; lookup y offset
        mov 	al, colour
		mov 	ah, al
		test 	al, al
		jz 		BLK                                         ; branching to hard code 'colour' saves a few cycle
		mov     ax, dx                                      ; proto-mask is white bits to 'colour'
		
WHT:    jcxz    J0                                          ; lhs and rhs share same byte?
        dec     cx
        jcxz    J1                                          ; lhs and rhs share same word?
        not 	dx											; convert proto-mask to mask word
		add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out target bits 	- 16 + EA(8)
		or      es:[di], al                            		; colour target bits	- 16 + EA(8)
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], ah                            ; colour target bits	- 16 + EA(8)
		mov     ax, 0FFFFh                                   ; AX white
		shr     cx, 1		                                 ; number of words to fill, lsb -> carry flag
		jnc     NC1                                          ; even so no byte to fill
		stosb	                                             ; odd do one byte al 'colour'
		jcxz    END
NC1:	rep     stosw		                                 ; CX is checked for !=0 before even the first step
		loop 	WHT
        jmp 	END
	
BLK:	jcxz    J0
        dec     cx
        jcxz    J1                                  	    ; lhs and rhs share same word?
        not 	dx										 	; convert proto-mask to mask word
        add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out target bits 	- 16 + EA(8)
		or      es:[di], al                            		; colour target bits	- 16 + EA(8)
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], ah                            ; colour target bits	- 16 + EA(8)
		mov     ax, 0                                       ; AX black
		shr     cx, 1		                                ; number of words to fill, lsb -> carry flag
		jnc     NC0                                         ; even so no byte to fill
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    END
NC0:	rep     stosw		                                ; CX is checked for !=0 before even the first step

		loop 	BLK
	
        jmp 	END
	
J1:	    not     dx                                          ; convert proto-mask to mask word
        and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		jmp 	END
	
J0:     and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
		and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)   

END:
}
