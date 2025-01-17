#include "hga_fast_line.h"

#include "hga_table_lookup_y.h"

void hga_fast_hline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour) {
	__asm {
		.8086
	    // 1. set up VRAM segment in ES
		mov   	ax, vram_segment
		mov   	es, ax
		// 2. lookup y and setup ES:DI point to target row
		mov 	bx, y1										; BX load y
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
		// 6 work out 'colour' bits into al AND ah
		mov     al, colour
		mov     ah, al
		test    al, al                                      ; is it black?
		jz      BLK
		mov     ax, dx                                      ; proto-mask is white bits to 'colour'
BLK:    // 7.1 special case same byte
		test	cx, cx                                      ; lhs and rhs share same byte?
		jnz     GEN
		and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
        // 7.2 colour the combined lhs/rhs byte
		and     es:[di + bx], dl                            ; mask out target bits
		or      es:[di + bx], al                            ; colour target bits
		jmp     END
GEN:		// 7. general case
		// work out line size into cx
		// work out 'colour' bits into bl
		// not dx into mask
		// and dl, dh lhs and rhs bytes
		// or in 'colour' lhs and rhs bytes
		// 8. fill in solid byte if cx test 1
		// 9. fill in word(s) if cx > 1
END:
	}
}

void hga_fast_vline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour) {
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
L1:	    mov   	di, HGA_TABLE_Y_LOOKUP[bx]                  ; lookup y offset
		add   	di, ax                                      ; add in x / 8
		add 	bx, 2 										; next line
	    // 6. colour the selected pixel
		and		es:[di], dh								    ; mask out target pixel
		or 		es:[di], dl									; or in the 'colour'
		loop 	L1                                          ; for line length
	}
}

/*
// 3.1 setup DL lhs and DH rhs of the line
		mov     dx, 80FFh                                  ;
		mov		cx, x1						                ; load x1
		and     cx, 7                                       ; x1 mod 8
		sar     dl, cl
		// 3.2 setup DH rhs of the line
		mov 	cx, x2						                ; load x2
		and     cx, 7                                       ; x2 mod 8
		shr     dh, cl                                      ; DH is the mask of the rhs line
*/

/*
		// 3. construct left most byte
		mov		cx, ax						; copy x1
		and		cx, 07h						; mask off 0111 lower bits (mod 8)
		mov		dl, 0FFh					; load DL with 1111111
		shr		dl, cl						; shift along by x mod 8
		// 4. construct right most byte
		mov 	cx, bx						; copy x2
		and		cx, 07h						; mask off 0111 lower bits (mod 8)
		mov 	dh, 0FFh					; load DH with 1111111
		shr 	dh, cl						; shift along by x mod 8
		not 	dh							; invert the bits
		// 5. x1 div 8
	    shr		ax, 1			            ; calculate column byte x1 / 8
	    shr		ax, 1			            ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		// 6. x2 div 8
	    shr		bx, 1			            ; calculate column byte x2 / 8
	    shr		bx, 1			            ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		bx, 1
		// 7. OR left and right ends of the line
		add 	di, ax 						; select lhs byte
		or 		es:[di], dl					; OR in lhs of line
		sub 	bx, ax						; distance to rhs
		jnz		J1							; lhs and rhs not share same byte
		and		es:[di], dh					; occupy same byte and will need to mask out the right end
J1:		add 	di, bx 						; select rhs byte
		or 		es:[di], dh					; OR in rhs of line
		*/
