#include "hga_filled_rectangle.h"

#include "hga_table_lookup_y.h"

void hga_filled_rectangle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t colour) {
    __asm {
		.8086
		// set up registers
		cld                                                 ; clear direction flag
		mov   	ax, vram_segment							; set up VRAM segment
		mov   	es, ax
		mov 	bx, x										; BX load x(1)
		mov 	ax, bx										; AX = x
		add     ax, w 										; AX = x + w i.e x(2)
		dec 	ax											; zero base width
		// prepare lhs and rhs pixels and mask
		mov 	dx, 0FFFFh 									; DL lhs DH rhs proto-masks (little endian)
		mov 	cx, bx										; copy x1
		and 	cx, 7h			                           	; CX is x1 mod 8
		shr 	dl, cl										; shift lhs proto mask to starting pixel
		mov 	cx, ax										; copy x2
		and 	cx, 7h			                           	; CX is x2 mod 8
		xor 	cx, 7h										; convert to bits to shift left
		shl 	dh,cl 										; shift rhs proto mask to ending pixel
		// reduce x(1) and x(2) to byte pointers 
		shr		bx, 1			                           	; calculate column byte x1 / 8
	    shr		bx, 1			                            ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		bx, 1
		shr		ax, 1			                           	; calculate column byte x2 / 8
	    shr		ax, 1
	    shr		ax, 1
		// prepare width and loop registers
		mov 	cx, ax
		sub 	cx, bx										; CX line length (bytes)
		mov 	si, cx										; SI copy of width
        mov 	al, colour									; AL will select colour path
		// select hard coded colour path of exec
		test 	al, al
		jz 		BLK                                         ; branching to hard code 'colour' saves a few cycle
		mov     ax, dx                                      ; proto-mask is white bits to 'colour'
		// white pixel special case seletion
WHT:	jcxz    J0                                          ; lhs and rhs share same byte?
        dec     cx
        jcxz    J1                                          ; lhs and rhs share same word?
        not 	dx											; convert proto-mask to mask word	
		// white pixel loop registers
		mov 	cx, h										; height loop
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer
		// white pixel loop
L1:		push 	cx											; store height loop counter
		mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		mov 	cx, si										; recover width loop counter
		add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out target bits 	- 16 + EA(8)
		or      es:[di], al                            		; colour target bits	- 16 + EA(8)
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], ah                            ; colour target bits	- 16 + EA(8)
		// set up for string ops
		mov     ax, 0FFFFh                                  ; AX white
		shr     cx, 1		                                ; number of words to fill, lsb -> carry flag
		jnc     NC1                                         ; even so no byte to fill
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    NXT											; only 1 byte width
NC1:	rep     stosw		                                ; CX is checked for !=0 before even the first step
NXT		add 	bp, 2										; next row
		pop 	cx											; recover height loop counter 
		//loop 	L1
        jmp 	END
	
BLK:	mov     ax, 0                                 		; AX black

		// shared word special case 
J1:	    not     dx                                          ; convert proto-mask to mask word
        and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		jmp 	END
		// shared byte special case
J0:     and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
		and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)   

END:	pop 	BP
}
