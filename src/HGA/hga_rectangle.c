#include "hga_rectangle.h" 

void hga_rectangle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t colour) {
    __asm {
		.8086
	    // 1. set up VRAM segment in ES
		mov   	ax, vram_segment
		mov   	es, ax
		// 2. lookup y and setup ES:DI point to target row
        mov 	bx, y 										; BX load y
	    shl     bx, 1                                       ; convert BX word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bx]					; lookup y offset
        // 3. build lsh & rsh proto-masks
        mov     bx, x                                       ; BX load x
		mov 	dx, 0FFFFh 									; DL lhs DH rhs proto-masks (little endian)
		mov 	cx, bx										; copy x
		and 	cx, 7h			                           	; CX is x mod 8
		shr 	dl, cl										; shift lhs proto mask to starting pixel
		mov 	cx, bx										; copy x
        add     cx, w                                       ; x + w 
		and 	cx, 7h			                           	; CX is x + w mod 8
		xor 	cx, 7h										; convert to bits to shift left
		shl 	dh,cl 										; shift rhs proto mask to ending pixel
}
