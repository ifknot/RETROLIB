#include "hga_fast_line.h"

#include "hga_table_lookup_y.h"

void hga_fast_hline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour) {
	__asm {
		.8086
	    // 1. set up VRAM segment in ES
		mov   	ax, vram_segment
		mov   	es, ax
		// 2. lookup y and setup ES:DI point to target line
		mov     bx, y1                                      ; load y1
        shl     bx, 1                                       ; convert BX to a word pointer
	   	mov   	di, HGA_TABLE_Y_LOOKUP[bx]                  ; lookup y offset		
		// 3. build lhs of line
		mov     ax, x1		
       	mov     bx, x2
		mov 	cx, ax										; copy x1
		and     cx, 7                                   	; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
		mov 	dl, 11111111b								; load DL inverse mask 
		shr 	dl, cl										; shorten mask to fit line start 
		not 	dl											; invert to make mask 
		mov 	dh, colour 									; load DH colour
		not 	dh 											; invert colour
		xor 	cl, 7										; CL = number of bits to shift left (thanks bit flip XOR)
		shl 	dh, cl										; shift inverted colour
		not		dh											; revert to set bits 
		// 4. draw lhs of line 
		add 	di, ax 
		and     es:[di], dl                             ; mask out the pixel bits
        or      es:[di], dh                             ; draw lhs line
		
		
END:
	}
}

void hga_fast_vline(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour) {
	__asm {
		.8086
	    // 1. set up VRAM segment in ES
	    mov   ax, vram_segment
		mov   es, ax
	    // 2. shift the pixel in DL right into the correct x mod 8 position
	    mov		ax, x1			                           	; load x
	    mov		cx, ax			                           	; copy of x
	    and		cx, 7h			                           	; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)
	    mov		dl, 80h			                           	; load DL with a single pixel at msb 10000000
	    shr		dl, cl			                           	; shift single bit along by x mod 8
	    // 3. x div 8
	    shr		ax, 1			                           	; calculate column byte x / 8
	    shr		ax, 1			                           	; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		// 4. setup y loop and bound the line to the screen
		mov 	bx, y1
		mov 	cx, y2
		sub 	cx, bx										; BX is the start y CX is the y count
        shl     bx, 1                                       ; BX is a word pointer so double
		// 5. lookup y and setup ES:DI point to target byte
L1:	    mov   	di, HGA_TABLE_Y_LOOKUP[bx]                  ; lookup y offset
		add   	di, ax                                      ; add in x / 8
		add 	bx, 2 										; next line
*** need to mask out for black colour ***
	    // 6. OR in the pixel
		or 		es:[di], dl
		loop 	L1
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
