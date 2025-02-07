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
		//dec 	ax											; zero base width
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
		dec     si                                          ; zero base width
		// select hard coded colour path of exec
		mov 	al, colour									; AL will select colour path
		test 	al, al
		jz 		BLK                                         ; branching to hard code 'colour' 
		// white pixel special case seletion
WHT:	mov 	ax, dx										; white pixels are same as proto-mask
		jcxz    J0                                          ; lhs and rhs share same byte?
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
		add 	di, bx										; DI + x i.e. have ES:DI point to lhs
		and     es:[di], dl                            		; mask out target bits 	- 16 + EA(8)
		or      es:[di], al                                 ; colour target bits	- 16 + EA(8)
		inc 	di											; next byte
		// set up for string ops
		mov     ax, 0FFFFh                                  ; AX white
		mov 	cx, si										; CX = byte width loop counter
		shr     cx, 1		                                ; CX = word width loop counter, lsb -> carry flag
		jnc     NC1                                         ; even so no byte to fill
		// string ops
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    NXT											; only 1 byte width
NC1:	rep     stosw		                                ; CX is checked for !=0 before even the first step
        // DI ptr aligned for rhs line
        and     es:[di], dh                                 ; mask out target bits 	- 16 + EA(8)
		not 	dh											; AX has been reused by string ops
		or      es:[di], dh                                 ; colour target bits	- 16 + EA(8)
NXT:	add 	bp, 2										; next row
		pop 	cx											; recover height loop counter
		loop 	L1
        jmp 	END

BLK:	

		// shared word special case
J1:	    mov 	cx, h										; height loop
		not     dx                                          ; convert proto-mask to mask word
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer     
LJ1:    mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		add 	bp, 2										; next row
		loop 	LJ1
		jmp 	END
		// shared byte special case
J0:    	mov 	cx, h										; height loop
        not     dx                                          ; convert proto-mask to mask word
		and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer
LJ0:    mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)
		add 	bp, 2										; next row
		loop 	LJ0
END:	pop 	bp											; restore BP for graceful function exit
    }
}
