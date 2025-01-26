#include "hga_rectangle.h"
#include "hga_table_lookup_y.h"

void hga_rectangle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t colour) {
    __asm {
		.8086
		// draw the top horizontal line
		// 1. set up VRAM segment in ES
		mov   	ax, vram_segment
		mov   	es, ax
		// 2. lookup y and y + h and setup ES:DI point to target row use SI as swap reg
		mov     ax, y
        mov 	bx, ax 										; BX load y
	    shl     bx, 1                                       ; convert BX word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bx]					; DI = lookup y offset
		add     ax, h
		mov 	bx, ax 										; BX load y + h
		shl     bx, 1                                       ; convert BX word pointer
		mov   	si, HGA_TABLE_Y_LOOKUP[bx]					; SI = lookup y + h offset
		// 3. set up registers
		mov 	bx, x										; BX load x
        // 4. build lsh & rsh proto-masks (would a look up table be quicker?)
        mov 	dx, 0FFFFh 									; DL lhs DH rhs proto-masks (little endian)
		mov 	cx, bx										; copy x
		and 	cx, 7h			                           	; CX is x mod 8
		shr 	dl, cl										; shift lhs proto mask to starting pixel
		mov     cx, w                                       ; CX load width
		dec     cx                                          ; zero base width
		add     cx, bx                                      ; CX = x + w
		and 	cx, 7h			                           	; CX is x + w mod 8
		xor 	cx, 7h										; convert to bits to shift left
		shl 	dh,cl 										; shift rhs proto mask to ending pixel
		// 5. reduce x to column bytes
		shr		bx, 1			                           	; calculate column byte x1 / 8
	    shr		bx, 1			                            ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		bx, 1
  		// 6. reduce w to column bytes
  		mov     cx, w
  		shr     cx, 1
  		shr     cx, 1
  		shr     cx, 1
		// 7.0 work out 'colour' bits into al AND ah (would a look up table be quicker?)
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
		add     di, bx
		add     si, bx
		and     es:[di], dl
		or      es:[di], al
		and     es:[si], dl
		or      es:[si], al
		mov     bx, cx
		inc     di
		inc     si
		and     es:[di + bx], dh
		or      es:[di + bx], ah
		and     es:[si + bx], dh
		or      es:[si + bx], ah

		// 7.1.2 work out fill 'colour'
		mov 	al, colour
		mov 	ah, al
		test 	al, al
		jz 		Z1
		mov     ax, 0FFFFh                                   ; AX white
Z1:     // 7.1.3 handle odd or even line lengths
		cld                                                  ; clear direction flag
		shr     cx, 1		                                 ; number of words to fill, lsb -> carry flag
		mov     bx, cx                                       ; copy of word count
		jnc     NC0                                          ; even so no byte to fill
		stosb	                                             ; odd do one byte al 'colour'
		xchg    di, si
		stosb                                                ; odd byte colour lower line
		jcxz    END
NC0:	// 7.1.4 remaining word(s) ax 'colour'
        mov     bx, cx
		rep     stosw		                                 ; CX is checked for !=0 before even the first step
		xchg    di, si
		mov     cx, bx
		rep     stosw
		jmp 	END
J1:		// 7.2.0 special case same word (saves 48 clock cycles on 8086 line lengths 2 - 15)
        not     dx                                           ; convert proto-mask to mask word
        // 7.2.1 colour the shared lhs:rhs word                                       Clock Cycles
        and     es:[di + bx], dx                             ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                             ; colour target word	- 16 + EA(8)
		xchg    di, si                                       ; swap lines
		and     es:[di + bx], dx
		or      es:[di + bx], ax
		jmp 	END
J0:     // 7.3.0 special case same byte (saves 48 clock cycles on 8086 line lengths 0 - 7)
		and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
        // 7.3.1 colour the combined lhs&rhs byte										Clock Cycles
		and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)
		xchg    di, si                                      ; swap lines
		and     es:[di + bx], dl
		or      es:[di + bx], al
END:
		// 1. setup registers for lhs vline
		mov   	dh, 00000001                                ; DH is (proto)mask byte
		mov     dl, colour                                  ; DL load 'colour'
		mov		ax, x			                           	; AX load x
        mov		cx, ax			                           	; CX copy of x
        and		cx, 7h			                           	; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)
		xor     cx, 7h                                      ; convert to bits to shift left
        // 2. setup pixel and mask
	    shl		dx, cl			                           	; shift colour bit & proto-mask into position
		not     dh                                          ; convert to mask
		// 3. convert x to column byte
	    shr		ax, 1			                           	; calculate column byte x / 8
	    shr		ax, 1			                           	; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		// 4. setup y loop and lookup pointer
		mov 	bx, y                                      	; BX load y
		inc 	bx											; first pixel already drawn hline
		mov 	cx, h                                      	; CX load height
		dec 	cx										    ; top and bottom pixels already in place hline
        shl     bx, 1                                       ; convert BX word pointer
		// 5. lookup y and setup ES:DI point to target byte
L0:	    mov   	di, HGA_TABLE_Y_LOOKUP[bx]                  ; lookup y offset
		add   	di, ax                                      ; add in x / 8
		add 	bx, 2 										; next line
	    // 6. colour the selected pixel
		and		es:[di], dh								    ; mask out target pixel
		or 		es:[di], dl									; or in the 'colour'
		loop 	L0
/*
		// 7. setup registers for rhs vline
		mov   	dh, 00000001                                ; DH is (proto)mask byte
		mov     dl, colour                                  ; DL load 'colour'
		mov		ax, x			                           	; AX load x
		add 	ax, w 										; AX = x + w
        mov		cx, ax			                           	; CX copy of x + w
        and		cx, 7h			                           	; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)										; rotate mask bit by x mod 8
		xor     cx, 7h                                      ; convert to bits to shift left
        // 8. setup pixel and mask
	    shl		dx, cl			                           	; shift colour bit & proto-mask into position
		not     dh                                          ; convert to mask
		// 9.0 convert x to column byte
	    shr		ax, 1			                           	; calculate column byte x / 8
	    shr		ax, 1			                           	; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		// 10. setup y loop and lookup pointer
		mov 	bx, y                                      	; BX load y
		inc 	bx											; first pixel already drawn hline
		mov 	cx, h                                      	; CX load height
		dec     cx										    ; top and bottom pixels already in place hline
        shl     bx, 1                                       ; convert BX word pointer
		// 11. lookup y and setup ES:DI point to target byte
L1:	    mov   	di, HGA_TABLE_Y_LOOKUP[bx]                  ; lookup y offset
		add   	di, ax                                      ; add in x / 8 + w /8
		add 	bx, 2 										; next line
	    // 12. colour the selected pixel
		and		es:[di], dh								    ; mask out target pixel
		or 		es:[di], dl									; or in the 'colour'
		loop 	L1
*/
    }
}
