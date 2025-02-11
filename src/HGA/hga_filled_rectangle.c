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
		add     ax, w 										; AX = x + w
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
		mov     ah, al
		test 	al, al
		jz      BLK                                         ; black 'colour' leave AX = 0
		mov 	ax, dx										; white pixels are same as proto-mask
		jmp     WHT
		// black special case selection
BLK:	jcxz    BJ0                                         ; lhs and rhs share same byte?
		dec     cx
        jcxz    BJ1                                         ; lhs and rhs share same word?
        not 	dx											; convert proto-mask to mask word
		// black general case
		mov 	cx, h										; height loop
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer
		// black pixel loop
BPIX:	push 	cx											; store height loop counter
		mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		add 	di, bx										; DI + x i.e. have ES:DI point to lhs
		and     es:[di], dl                           		; mask out target bits 	- 16 + EA(8)
		//or      es:[di], al                                 ; colour target bits	- 16 + EA(8)
		inc 	di											; next byte
		add     di, si                                      ; extend DI to rhs
		and     es:[di], dh                                 ; mask out target bits 	- 16 + EA(8)
		//or      es:[di], ah                                 ; colour target bits	- 16 + EA(8)
		sub     di, si                                      ; restore DI to lhs
		// set up for string ops
		//push    ax                                          ; preserve rhs white bits
		//mov     ax, 0FFFFh                                  ; AX white
		mov 	cx, si										; CX = byte width loop counter
		shr     cx, 1		                                ; CX = word width loop counter, lsb -> carry flag
		jnc     BNC                                         ; even so no byte to fill
		// string ops
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    BNXT                                        ; only 1 byte width
BNC:	rep     stosw		                                ; CX is checked for !=0 before even the first step
BNXT:	add 	bp, 2										; next row
		//pop     ax
        pop 	cx											; recover height loop counter
		loop 	BPIX
        jmp 	END
        // shared word special case
BJ1:	mov 	cx, h										; height loop
		not     dx                                          ; convert proto-mask to mask word
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer
BL1:    mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		//or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		add 	bp, 2										; next row
		loop 	BL1
		jmp 	END
		// shared byte special case
BJ0:    mov 	cx, h										; height loop
		and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		//and     al, ah                                      ; combine 'colour' bits into al
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer
BL0:    mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		//or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)
		add 	bp, 2										; next row
		loop 	BL0
		jmp     END
        // white special case selection
WHT:	jcxz    WJ0                                         ; lhs and rhs share same byte?
        dec     cx
        jcxz    WJ1                                         ; lhs and rhs share same word?
        not 	dx											; convert proto-mask to mask word
        // white general case
		mov 	cx, h										; height loop
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer
		// white pixel loop
WPIX:	push 	cx											; store height loop counter
		mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		add 	di, bx										; DI + x i.e. have ES:DI point to lhs
		and     es:[di], dl                           		; mask out target bits 	- 16 + EA(8)
		or      es:[di], al                                 ; colour target bits	- 16 + EA(8)
		inc 	di											; next byte
		add     di, si                                      ; extend DI to rhs
		and     es:[di], dh                                 ; mask out target bits 	- 16 + EA(8)
		or      es:[di], ah                                 ; colour target bits	- 16 + EA(8)
		sub     di, si                                      ; restore DI to lhs
		// set up for string ops
		push    ax                                          ; preserve rhs white bits
		mov     ax, 0FFFFh                                  ; AX white
		mov 	cx, si										; CX = byte width loop counter
		shr     cx, 1		                                ; CX = word width loop counter, lsb -> carry flag
		jnc     WNC                                         ; even so no byte to fill
		// string ops
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    WNXT                                        ; only 1 byte width
WNC:	rep     stosw		                                ; CX is checked for !=0 before even the first step
WNXT:	add 	bp, 2										; next row
	    pop     ax
        pop 	cx											; recover height loop counter
		loop 	WPIX
        jmp 	END
        // shared word special case
WJ1:	mov 	cx, h										; height loop
		not     dx                                          ; convert proto-mask to mask word
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer
WL1:    mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		add 	bp, 2										; next row
		loop 	WL1
		jmp 	END
		// shared byte special case
WJ0:    mov 	cx, h										; height loop
		and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
		push 	bp 											; preserve BP for __asm
		mov 	bp, y										; BP load y1
	    shl     bp, 1                                       ; convert BP word pointer
WL0:    mov   	di, HGA_TABLE_Y_LOOKUP[bp]					; lookup y offset
		and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)
		add 	bp, 2										; next row
		loop 	WL0
END:	pop 	bp											; restore BP for graceful function exit
    }
}
