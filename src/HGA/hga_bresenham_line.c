#include "hga_bresenham_line.h"

#include "hga_table_lookup_y.h"

/**
* Bresenham line algorithm - 8086 register optimised, hardcoded, heavily inlined version
* @url https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
* The 8086 is register limited to only 7 registers and using memory variables, particularly in the pixel plotting loop, is clock cycle expensive.
* This can be amortized against memory by identifying parts of the algorithm that can be split into seperate hardcoded execution paths.
* The initial split is into 2 sections one for hardcoded white pixels the other for black.
* Within each of these 2 sections the pixel plotting loop is split into 4 hardcoded sections for incrementing y, decrementing y, incrementing x and decrementing x
* This increases the memory footprint by 8 fold but on an IBM XT the performance over naive is improved:
*           white   black       328     328
* + 86Box    38%     45%
* + DosBoxX  28%     31%        237     226
* + IBM XT   ?       ?
*
* TODO: Further, the code is split into much faster specializations for vertical and horizontal lines which more than double performance for those line types.
*/
void hga_bline0(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour) {
    __asm {
		.8086
	    // set up VRAM segment in ES
	    mov   	ax, vram_segment
		mov   	es, ax
		// select hard coded colour execution path
		mov     al, colour
		cmp     al, 0
		jz      BLACK
		jmp     WHITE
		// black pixel octant function dispatcher
BLACK:  mov     dx, x1
		sub     dx, x0                            	; dx = x1 - x0
		jge     B0
		neg     dx                                	; ax = abs(x1 - x0)
B0:     mov     cx, y1
		sub     cx, y0                            	; dy = y1 - y0
		jge     B1
		neg     cx                                	; bx = abs(y1 - y0)
B1:     cmp     cx, dx                            	; if abs(y1 - y0) < abs(x1 - x0)
        jge     B2
        // plot octants 0, 3, 4, and 7
        mov     ax, x0
        cmp     ax, x1                            	; if x1 > x0
        jle     B0X
        mov     ax, x1                            	; x = x1
		mov     bx, y1                            	; y = y1
        mov     dx, x0
        sub     dx, ax                            	; dx = x0 - x1
        mov     cx, y0
        sub     cx, bx                            	; dy = y0 - y1
        push    bp
        mov     bp, x0
        jmp     Q0                                	; plotLineLow(x1, y1, x0, y0)
B0X:    mov     bx, y0                            	; y = y0
        mov     dx, x1
        sub     dx, ax                            	; dx = x1 - x0
        mov     cx, y1
        sub     cx, bx                            	; dy = y1 - y0
        push    bp
        mov     bp, x1
        jmp     Q0                                	; plotLineLow(x0, y0, x1, y1)
        // plot octants 1, 2, 5, and 6
B2:  	mov     ax, y0
        cmp     ax, y1                            	; if y0 > y1
        jle     B0Y
        mov     ax, x1                            	; x = x0
		mov     bx, y1                            	; y = y0
        mov     dx, x0
        sub     dx, ax                            	; dx = x1 - x0
        mov     cx, y0
        sub     cx, bx                            	; dy = y1 - y0
        push    bp
        mov     bp, y0
        jmp     Q1                                	; plotLineHigh(x1, y1, x0, y0)
B0Y:    mov     ax, x0                           	; x = x0
		mov     bx, y0                            	; y = y0
        mov     dx, x1
        sub     dx, ax                            	; dx = x1 - x0
        mov     cx, y1
        sub     cx, bx                            	; dy = y1 - y0
        push    bp
        mov     bp, y1
        jmp     Q1                                	; plotLineHigh(x0, y0, x1, y1)
     	// select hard coded y++ or y-- execution path
Q0:		cmp     cx, 0                             	; if dy < 0
		jg      B4                                	; y++
		jmp     B5                                	; y--
     	// y++ set up registers DI = D = (2 * dy), SI = 2 * (dy - dx)
B4:     mov     di, cx                            	; D = dy
		add     di, cx                            	; D = 2 * dy
		sub     di, dx                            	; D = (2 * dy) - dx
        mov     si, cx                            	; SI = dy
        sub     si, dx                            	; SI = dy - dx
        shl     si, 1                             	; SI = 2 * (dy - dx)
		// plot y increasing octants for x.. x1
M0:	    push    cx                                	; loop x.. x1
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dl, 11111110b                      	; DL = pixel mask
        rol     dl, cl                             	; roll mask around by x mod 8
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx                             	; copy bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dl
        mov     bx, cx                             	; restore bx
        pop     ax
        pop     cx
        // decision variable
        cmp     di, 0                               ; if D > 0
        jle     B6
        inc     bx                                  ; y++
        add     di, si                              ; D = D + (2 * (dy - dx))
        inc     ax                                  ; x++
        cmp     ax, bp                              ; x == x1?
        jne     M0                                  ; loop
        jmp     END                                 ; done
B6:     add     di, cx
        add     di, cx                              ; D = D + 2*dy
        inc     ax                                  ; x++
        cmp     ax, bp                          ; x == x1?
        jne     M0                                  ; loop
        jmp     END                                 ; done
        // y-- set up registers DI = D = (2 * dy) - dx, SI = 2 * (dy - dx)
B5:     neg     cx                                  ; dy = -dy ie abs(dy)
        mov     di, cx                              ; D = dy
		add     di, cx                              ; D = 2 * dy
		sub     di, dx                              ; D = (2 * dy) - dx
        mov     si, cx                              ; SI = dy
        sub     si, dx                              ; SI = dy - dx
        shl     si, 1                               ; SI = 2 * (dy - dx)
		// plot y decreasing octants for x.. x1
M1:     push    cx                                  ; loop x.. x1
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dl, 11111110b                      ; DL = pixel mask
        rol     dl, cl                             ; roll mask around by x mod 8
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dl
        pop     ax
        mov     bx, cx
        pop     cx
        // decision variable
        cmp     di, 0                               ; if D > 0
        jle     B7
        dec     bx                                  ; y--
        add     di, si                              ; D = D + (2 * (dy - dx))
        inc     ax
        cmp     ax, bp
        jne     M1									; loop
        jmp     END									; done
B7:     add     di, cx
        add     di, cx                              ; D = D + 2*dy
        inc     ax
        cmp     ax, bp
        jne     M1									; loop
        jmp     END									; done
 		// select hard coded x++ or x-- execution path
Q1:		cmp     dx, 0                             	; if dx < 0
		jg      B8                                	; x++
		jmp     B9                                	; x--
     	// x++ set up registers DI = D = (2 * dx) - dy, SI =  (2 * (dx - dy))
B8:     mov     di, dx                            	; D = dx
		add     di, dx                            	; D = 2 * dx
		sub     di, cx                            	; D = (2 * dx) - dy
        mov     si, dx                            	; SI = dx
        sub     si, cx                            	; SI = dx - dy
        shl     si, 1                             	; SI = 2 * (dx - dy)
M2:     push    dx                                	; loop y.. y1 (BX)
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dx, 101h
        shl		dx, cl
        not     dh
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        pop     ax
        mov     bx, cx
        pop     dx
        // decision variable
        cmp     di, 0                            	; if D > 0
        jle     B10
        inc     ax                                  ; x++
        add     di, si                              ; D = D + (2 * (dx - dy))
        inc     bx                                  ; next y
        cmp     bx, bp
        jne     M2									; loop
        jmp     END									; done
B10:    add     di, dx
        add     di, dx                              ; D = D + 2*dx
        inc     bx                                  ; next y
        cmp     bx, bp
        jne     M2									; loop
        jmp     END									; done
     	// x-- set up registers DI = D = (2 * dx) - dy, SI =  (2 * (dx - dy))
B9:     neg     dx                                	; dx = -dx ie abs(dx)
        mov     di, dx                            	; D = dx
		add     di, dx                            	; D = 2 * dx
		sub     di, cx                            	; D = (2 * dx) - dy
        mov     si, dx                            	; SI = dx
        sub     si, cx                            	; SI = dx - dy
        shl     si, 1                             	; SI = 2 * (dx - dy)
M3:     push    dx                                  ; loop y.. y1 (BX)
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dx, 101h
        shl		dx, cl
        not     dh
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        pop     ax
        mov     bx, cx
        pop     dx
        // decision variable
        cmp     di, 0                               ; if D > 0
        jle     B11
        dec     ax                                  ; x--
        add     di, si                              ; D = D + (2 * (dx - dy))
        inc     bx                                  ; next y
        cmp     bx, bp
        jne     M3									; loop
        jmp     END									; done
B11:    add     di, dx
        add     di, dx                              ; D = D + 2*dx
        inc     bx                                  ; next y
        cmp     bx, bp
        jne     M3									; loop
        jmp     END
		// white pixel octant function dispatcher
WHITE:	mov     dx, x1
		sub     dx, x0                            	; dx = x1 - x0
		jge     J0
		neg     dx                                	; ax = abs(x1 - x0)
J0:     mov     cx, y1
		sub     cx, y0                            	; dy = y1 - y0
		jge     J1
		neg     cx                                	; bx = abs(y1 - y0)
J1:     cmp     cx, dx                            	; if abs(y1 - y0) < abs(x1 - x0)
        jge     J2
        // plot octants 0, 3, 4, and 7
        mov     ax, x0
        cmp     ax, x1                            	; if x1 > x0
        jle     JX
        mov     ax, x1                            	; x = x1
		mov     bx, y1                            	; y = y1
        mov     dx, x0
        sub     dx, ax                            	; dx = x0 - x1
        mov     cx, y0
        sub     cx, bx                            	; dy = y0 - y1
        push    bp
        mov     bp, x0
        jmp     P0                                	; plotLineLow(x1, y1, x0, y0)
JX:     mov     bx, y0                            	; y = y0
        mov     dx, x1
        sub     dx, ax                            	; dx = x1 - x0
        mov     cx, y1
        sub     cx, bx                            	; dy = y1 - y0
        push    bp
        mov     bp, x1
        jmp     P0                                	; plotLineLow(x0, y0, x1, y1)
        // plot octants 1, 2, 5, and 6
J2:  	mov     ax, y0
        cmp     ax, y1                            	; if y0 > y1
        jle     JY
        mov     ax, x1                            	; x = x0
		mov     bx, y1                            	; y = y0
        mov     dx, x0
        sub     dx, ax                            	; dx = x1 - x0
        mov     cx, y0
        sub     cx, bx                            	; dy = y1 - y0
        push    bp
        mov     bp, y0
        jmp     P1                                	; plotLineHigh(x1, y1, x0, y0)
JY:     mov     ax, x0                            	; x = x0
		mov     bx, y0                            	; y = y0
        mov     dx, x1
        sub     dx, ax                            	; dx = x1 - x0
        mov     cx, y1
        sub     cx, bx                            	; dy = y1 - y0
        push    bp
        mov     bp, y1
        jmp     P1                                	; plotLineHigh(x0, y0, x1, y1)
     	// select hard coded y++ or y-- execution path
P0:		cmp     cx, 0                             	; if dy < 0
		jg      J4                                	; y++
		jmp     J5                                	; y--
J4:     // y++ set up registers DI = D = (2 * dy), SI = 2 * (dy - dx)
        mov     di, cx                            	; D = dy
		add     di, cx                            	; D = 2 * dy
		sub     di, dx                            	; D = (2 * dy) - dx
        mov     si, cx                            	; SI = dy
        sub     si, dx                            	; SI = dy - dx
        shl     si, 1                             	; SI = 2 * (dy - dx)
		// plot y increasing octants for x.. x1
L0:	    push    cx                                	; loop x.. x1
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dx, 101h
        shl		dx, cl
        not     dh
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        or 		es:[bx], dl
        mov     bx, cx
        pop     ax
        pop     cx
        // decision variable
        cmp     di, 0                               ; if D > 0
        jle     J6
        inc     bx                                  ; y++
        add     di, si                              ; D = D + (2 * (dy - dx))
        inc     ax                                  ; x++
        cmp     ax, bp                              ; x == x1?
        jne     L0                                  ; loop
        jmp     END                                 ; done
J6:     add     di, cx
        add     di, cx                              ; D = D + 2*dy
        inc     ax                                  ; x++
        cmp     ax, bp                          	; x == x1?
        jne     L0                                  ; loop
        jmp     END                                 ; done
        // y-- set up registers DI = D = (2 * dy) - dx, SI = 2 * (dy - dx)
J5:     neg     cx                                  ; dy = -dy ie abs(dy)
        mov     di, cx                              ; D = dy
		add     di, cx                              ; D = 2 * dy
		sub     di, dx                              ; D = (2 * dy) - dx
        mov     si, cx                              ; SI = dy
        sub     si, dx                              ; SI = dy - dx
        shl     si, 1                               ; SI = 2 * (dy - dx)
		// plot y decreasing octants for x.. x1
L1:     push    cx                                  ; loop x.. x1
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dx, 101h
        shl		dx, cl
        not     dh
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        or 		es:[bx], dl
        pop     ax
        mov     bx, cx
        pop     cx
        // decision variable
        cmp     di, 0                               ; if D > 0
        jle     J7
        dec     bx                                  ; y--
        add     di, si                              ; D = D + (2 * (dy - dx))
        inc     ax
        cmp     ax, bp
        jne     L1									; loop
        jmp     END									; done
J7:     add     di, cx
        add     di, cx                              ; D = D + 2*dy
        inc     ax
        cmp     ax, bp
        jne     L1									; loop
        jmp     END									; done
 		// select hard coded x++ or x-- execution path
P1:		cmp     dx, 0                             	; if dx < 0
		jg      J8                                	; x++
		jmp     J9                                	; x--
     	// x++ set up registers DI = D = (2 * dx) - dy, SI =  (2 * (dx - dy))
J8:     mov     di, dx                            	; D = dx
		add     di, dx                            	; D = 2 * dx
		sub     di, cx                            	; D = (2 * dx) - dy
        mov     si, dx                            	; SI = dx
        sub     si, cx                            	; SI = dx - dy
        shl     si, 1                             	; SI = 2 * (dx - dy)
L2:     push    dx                                	; loop y.. y1 (BX)
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dx, 101h
        shl		dx, cl
        not     dh
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        or 		es:[bx], dl
        pop     ax
        mov     bx, cx
        pop     dx
        // decision variable
        cmp     di, 0                               ; if D > 0
        jle     J10
        inc     ax                                  ; x++
        add     di, si                              ; D = D + (2 * (dx - dy))
        inc     bx                                  ; next y
        cmp     bx, bp
        jne     L2									; loop
        jmp     END									; done
J10:    add     di, dx
        add     di, dx                              ; D = D + 2*dx
        inc     bx                                  ; next y
        cmp     bx, bp
        jne     L2									; loop
        jmp     END									; done
     	// x-- set up registers DI = D = (2 * dx) - dy, SI =  (2 * (dx - dy))
J9:     neg     dx                                	; dx = -dx ie abs(dx)
        mov     di, dx                            	; D = dx
		add     di, dx                            	; D = 2 * dx
		sub     di, cx                            	; D = (2 * dx) - dy
        mov     si, dx                            	; SI = dx
        sub     si, cx                            	; SI = dx - dy
        shl     si, 1                             	; SI = 2 * (dx - dy)
L3:     push    dx                                  ; loop y.. y1 (BX)
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dx, 101h
        shl		dx, cl
        not     dh
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        or 		es:[bx], dl
        pop     ax
        mov     bx, cx
        pop     dx
        // decision variable
        cmp     di, 0                               ; if D > 0
        jle     J11
        dec     ax                                  ; x--
        add     di, si                              ; D = D + (2 * (dx - dy))
        inc     bx                                  ; next y
        cmp     bx, bp
        jne     L3									; loop
        jmp     END									; done
J11:    add     di, dx
        add     di, dx                              ; D = D + 2*dx
        inc     bx                                  ; next y
        cmp     bx, bp
        jne     L3									; loop
        jmp     END
        // fast vertical line
VLINE:  mov   	dh, 00000001                                ; DH is (proto)mask byte
		mov     dl, colour                                  ; DL load 'colour'
		mov		ax, x0			                           	; AX load x
        mov		cx, ax			                           	; CX copy of x
        and		cx, 7h			                           	; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)										; rotate mask bit by x mod 8
		xor     cx, 7h                                      ; convert to bits to shift left
	    shl		dx, cl			                           	; shift colour bit & proto-mask into position
		not     dh                                          ; convert to mask
	    shr		ax, 1			                           	; calculate column byte x / 8
	    shr		ax, 1			                           	; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		mov 	bx, y0                                      ; BX load y0
		mov 	cx, y1                                      ; CX load y1
		cmp     bx, cx                                      ; ? x0 < x1
		jl      VJ0
		xchg    bx, cx                                      ; swap registers ie swap x0 x1
VJ0:	sub 	cx, bx										; convert CX line length
        shl     bx, 1                                       ; convert BX word pointer
VL0:	mov   	di, HGA_TABLE_Y_LOOKUP[bx]                  ; lookup y offset
		add   	di, ax                                      ; add in x / 8
		add 	bx, 2 										; next line
		and		es:[di], dh								    ; mask out target pixel
		or 		es:[di], dl									; or in the 'colour'
		loop 	VL0                                         ; for line length
		jmp     DONE
        // fast horizontal line
HLINE:  cld                                                 ; clear direction flag
		mov 	bx, y0										; BX load y0
	    shl     bx, 1                                       ; convert BX word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bx]					; lookup y offset
		mov 	bx, x0										; BX load x0
		mov 	ax, x1 										; AX load x1
		cmp     ax, bx                                      ; ? x0 < x1
		jg      JG0
		xchg    ax, bx                                      ; no - swap x0 and x1
JG0:	mov 	dx, 0FFFFh 									; DL lhs DH rhs proto-masks (little endian)
		mov 	cx, bx										; copy x0
		and 	cx, 7h			                           	; CX is x0 mod 8
		shr 	dl, cl										; shift lhs proto mask to starting pixel
		mov 	cx, ax										; copy x1
		and 	cx, 7h			                           	; CX is x1 mod 8
		xor 	cx, 7h										; convert to bits to shift left
		shl 	dh,cl 										; shift rhs proto mask to ending pixel
		shr		bx, 1			                           	; calculate column byte x0 / 8
	    shr		bx, 1			                            ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		bx, 1
		shr		ax, 1			                           	; calculate column byte x1 / 8
	    shr		ax, 1
	    shr		ax, 1
		mov 	cx, ax
		sub 	cx, bx										; CX line length (bytes)
		mov 	al, colour
		mov 	ah, al
		test 	al, al
		jz 		HBLK                                        ; branching to hard code 'colour' saves a few cycle
		mov     ax, dx                                      ; proto-mask is white bits to 'colour'
HWHT:   jcxz    HJ0                                         ; lhs and rhs share same byte?
        dec     cx
        jcxz    HJ1                                         ; lhs and rhs share same word?
		not 	dx											; convert proto-mask to mask word
		add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out target bits 	- 16 + EA(8)
		or      es:[di], al                            		; colour target bits	- 16 + EA(8)
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], ah                            ; colour target bits	- 16 + EA(8)
		mov     ax, 0FFFFh                                  ; AX white
		shr     cx, 1		                                ; number of words to fill, lsb -> carry flag
		jnc     HNC1                                        ; even so no byte to fill
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    DONE
HNC1:	rep     stosw		                                ; CX is checked for !=0 before even the first step
        jmp 	DONE
HBLK:	jcxz    HJ0                                         ; lhs and rhs share same byte?
        dec     cx
        jcxz    HJ1                                  	    ; lhs and rhs share same word?
		not 	dx										 	; convert proto-mask to mask word
		add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out black bits
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out black bits
		mov     ax, 0                                       ; AX black
		shr     cx, 1		                                ; number of words to fill, lsb -> carry flag
		jnc     HNC0                                        ; even so no byte to fill
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    DONE
HNC0:	rep     stosw		                                ; CX is checked for !=0 before even the first step
        jmp 	DONE
HJ1:	not     dx                                          ; convert proto-mask to mask word
        and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		jmp 	DONE
HJ0:    and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
        and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)
		jmp     DONE
END:    pop     bp
DONE:
    }

}

/**
* This version is a register, inlined and special case optimised version of Bresenham's principles of integer incremental error to perform all octant line draws.
* @see hga_bresenham_line_naive.c
* @url https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
*
* Performance gains can be made be de-generalizing the algorithm into hard-coded execution paths at the expense of increased memory footprint:
* + Implementation uses 4 execution pathways with hard coded inc and dec for improved diagonal performace 7.5% faster.
* + Further, splitting the code into specific colour sections gains 17% faster.
* + Selecting horizontal and vertical optimised routines delivers 89.8% speed gains for horizontal and vertical line cases!
* + The canonical computing dichotomy, space and time.
*/
void hga_bresenham_line(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour) {
    __asm { // best time on IBM XT - diagonal 200 lines takes 125 bios system clock ticks
		.8086
	    // set up VRAM segment in ES
	    mov   	ax, vram_segment
		mov   	es, ax
        // set up registers AX = x, BX = y, DX = dx, CX = dy, SI = error, DI = e2
        mov     dx, x1                              ; DX = x1
        sub     dx, x0                              ; DX = x1 - x0
        jnz     JNZ0                                ; x0 == x1 ?
        // select fast vertical line code path
        jmp    VLINE
JNZ0:   jge     J0                                  ; jump short if dx not signed i.e. positive
        neg     dx                                  ; negative so make positive
J0:     mov     cx, y1                              ; CX = y1
        sub     cx, y0                              ; CX = y1 - y0
        jnz     JNZ1                                ; y0 == y1 ?
        // select fast horizontal line code path
        jmp     HLINE
JNZ1:   jge     J1                                  ; jump short if dy not signed i.e. positive
        neg     cx                                  ; negative so make positive
J1:     neg     cx                                  ; CX = -abs(dy)
        mov     si, dx                              ; SI = dx
        add     si, cx                              ; SI = error = dx + dy
        mov     ax, x0
        mov     bx, y0
		//cmp     colour, 0                           select diagonal lines hard code colour path
		//jnz     WHITE
		//jmp     BLACK
		// diagonal lines dispatcher
WHITE:  cmp     ax, x1                              ; white function dispatch
        jl      J2                                  ; x0 < x1 so use x++
        cmp     bx, y1
        jg      J3                                  ; y0 > y1 so use y--
        jmp     L1P                                 ; x-- y++
J3:     jmp     L3P                                 ; x-- y--
J2:     cmp     bx, y1
        jg      J4                                  ; y0 > y1 so use y--
        jmp     L0P                                 ; x++ y++
J4:     jmp     L2P                                 ; x++ y--
BLACK:  jmp END
        // Bresenham loop x++ y++ NB embedding the plot pixel code saves call (23 - 36)  + ret (20 - 34) cycles 8086 *every* pixel plotted.
L0P:    push    dx                                  ; preserve corrupted registers
        push    cx                                  ; (15 cycles) 8086
        push    ax
        mov     cx, ax                              ; copy of x low order byte
        and     cx, 7                               ; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
        xor     cx, 7                               ; CL = number of bits to shift left (thanks bit flip XOR)
		mov     dx, 101h                            ; DX = 0000000100000001 binary protomask DH colour DL
		shl		dx, cl                              ; shift colour bit & proto-mask into position
		not     dh                                  ; convert to mask
		shr		ax, 1			                    ; calculate column byte x1 / 8
	    shr		ax, 1			                    ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		mov     cx, bx								; preserve bx (2 cycles)
		shl     bx, 1                               ; convert BX word pointer
		mov   	bx, HGA_TABLE_Y_LOOKUP[bx]          ; BX = VRAM row offset
		add     bx, ax                              ; ES:[BX] points to VRAM byte containing pixel location
		and		es:[bx], dh						    ; mask out target pixel
		or 		es:[bx], dl							; or in the 'colour'
		pop     ax									; (12 cycles) 8086
		mov     bx, cx								; saves 17 - 4 cycles over push/pop ie 4% faster overall
		pop     cx                                  ; (15 cycles) 8086
		pop     dx                                  ; restore corrupted registers
		mov     di, si
		shl     di, 1                               ; DI = e2 = 2 * error
		cmp     di, cx                              ; ? e2 >= dy
		jl      L0J0
		cmp     ax, x1                              ; ? x0 == x1
		jne     L0J1
		jmp     END                                 ; if x0 == x1 break
L0J1:	add     si, cx                              ; error = error + dy
        inc     ax                                  ; x++
L0J0:	cmp    di, dx                               ; ? e2 <= dx
        jg     L0P
        cmp    bx, y1                               ; ? y0 == y1
        jne    L0J2
		jmp    END                                  ; if y0 == y1 break
L0J2:   add    si, dx                               ; error = error + dx
        inc    bx                                   ; y++
        jmp    L0P

        // Bresenham loop x-- y++
L1P:    push    dx
        push    cx
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
		mov     dx, 101h
		shl		dx, cl
		not     dh
		shr		ax, 1
	    shr		ax, 1
	    shr		ax, 1
		mov     cx, bx
		shl     bx, 1
		mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
		add     bx, ax
		and		es:[bx], dh
		or 		es:[bx], dl
		pop     ax
		mov     bx, cx
		pop     cx
		pop     dx
		mov     di, si
		shl     di, 1
		cmp     di, cx
		jl      L1J0
		cmp     ax, x1
		jne     L1J1
		jmp     END
L1J1:	add     si, cx
		dec     ax                                          ; x--
L1J0:	cmp     di, dx
        jg      L1P
        cmp     bx, y1
        jne     L1J2
        jmp     END
L1J2:   add     si, dx
        inc     bx                                          ; y++
        jmp     L1P

		// Bresenham loop x++ y--
L2P:    push    dx
        push    cx
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dx, 101h
        shl		dx, cl
        not     dh
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        or 		es:[bx], dl
        pop     ax
        mov     bx, cx
        pop     cx
        pop     dx
        mov     di, si
		shl     di, 1
		cmp     di, cx
		jl      L2J0
		cmp     ax, x1
		jne     L2J1
		jmp     END
L2J1:	add     si, cx
        inc     ax                                          ; x++
L2J0:	cmp     di, dx
        jg      L2P
        cmp     bx, y1
        jne     L2J2
        jmp     END
L2J2:   add     si, dx
        dec     bx                                          ; y--
        jmp     L2P

        // Bresenham loop x-- y--
L3P:    push    dx
        push    cx
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
        mov     dx, 101h
        shl		dx, cl
        not     dh
        shr		ax, 1
        shr		ax, 1
        shr		ax, 1
        mov     cx, bx
        shl     bx, 1
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        or 		es:[bx], dl
        pop     ax
        mov     bx, cx
        pop     cx
        pop     dx
        mov     di, si
		shl     di, 1
		cmp     di, cx
		jl      L3J0
		cmp     ax, x1
		jne     L3J1
		jmp     END
L3J1:	add     si, cx
        dec     ax                                          ; x--
L3J0: 	cmp     di, dx
        jg      L3P
        cmp     bx, y1
        jne     L3J2
        jmp     END
L3J2:   add     si, dx
        dec     bx                                          ; y--
        jmp     L3P

        // fast horizontal line
HLINE:  cld                                                 ; clear direction flag
		mov 	bx, y0										; BX load y0
	    shl     bx, 1                                       ; convert BX word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bx]					; lookup y offset
		mov 	bx, x0										; BX load x0
		mov 	ax, x1 										; AX load x1
		cmp     ax, bx                                      ; ? x0 < x1
		jg      JG0
		xchg    ax, bx                                      ; no - swap x0 and x1
JG0:	mov 	dx, 0FFFFh 									; DL lhs DH rhs proto-masks (little endian)
		mov 	cx, bx										; copy x0
		and 	cx, 7h			                           	; CX is x0 mod 8
		shr 	dl, cl										; shift lhs proto mask to starting pixel
		mov 	cx, ax										; copy x1
		and 	cx, 7h			                           	; CX is x1 mod 8
		xor 	cx, 7h										; convert to bits to shift left
		shl 	dh,cl 										; shift rhs proto mask to ending pixel
		shr		bx, 1			                           	; calculate column byte x0 / 8
	    shr		bx, 1			                            ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		bx, 1
		shr		ax, 1			                           	; calculate column byte x1 / 8
	    shr		ax, 1
	    shr		ax, 1
		mov 	cx, ax
		sub 	cx, bx										; CX line length (bytes)
		mov 	al, colour
		mov 	ah, al
		test 	al, al
		jz 		HBLK                                        ; branching to hard code 'colour' saves a few cycle
		mov     ax, dx                                      ; proto-mask is white bits to 'colour'
HWHT:   jcxz    HJ0                                         ; lhs and rhs share same byte?
        dec     cx
        jcxz    HJ1                                         ; lhs and rhs share same word?
		not 	dx											; convert proto-mask to mask word
		add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out target bits 	- 16 + EA(8)
		or      es:[di], al                            		; colour target bits	- 16 + EA(8)
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], ah                            ; colour target bits	- 16 + EA(8)
		mov     ax, 0FFFFh                                  ; AX white
		shr     cx, 1		                                ; number of words to fill, lsb -> carry flag
		jnc     HNC1                                        ; even so no byte to fill
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    END
HNC1:	rep     stosw		                                ; CX is checked for !=0 before even the first step
        jmp 	END
HBLK:	jcxz    HJ0                                         ; lhs and rhs share same byte?
        dec     cx
        jcxz    HJ1                                  	    ; lhs and rhs share same word?
		not 	dx										 	; convert proto-mask to mask word
		add 	di, bx										; have ES:DI point to lhs
		and     es:[di], dl                            		; mask out black bits
		mov 	bx, cx										; rhs offset = length
		inc 	di											; next byte
		and     es:[di + bx], dh                            ; mask out black bits
		mov     ax, 0                                       ; AX black
		shr     cx, 1		                                ; number of words to fill, lsb -> carry flag
		jnc     HNC0                                        ; even so no byte to fill
		stosb	                                            ; odd do one byte al 'colour'
		jcxz    END
HNC0:	rep     stosw		                                ; CX is checked for !=0 before even the first step
        jmp 	END
HJ1:	not     dx                                          ; convert proto-mask to mask word
        and     es:[di + bx], dx                            ; mask out target word 	- 16 + EA(8)
		or      es:[di + bx], ax                            ; colour target word	- 16 + EA(8)
		jmp 	END
HJ0:    and     dl, dh                                      ; combine proto-mask into dl
		not     dl		                                    ; convert proto-mask to mask
		and     al, ah                                      ; combine 'colour' bits into al
        and     es:[di + bx], dl                            ; mask out target bits 	- 16 + EA(8)
		or      es:[di + bx], al                            ; colour target bits	- 16 + EA(8)
		jmp     END

		// fast vertical line
VLINE:  mov   	dh, 00000001                                ; DH is (proto)mask byte
		mov     dl, colour                                  ; DL load 'colour'
		mov		ax, x0			                           	; AX load x
        mov		cx, ax			                           	; CX copy of x
        and		cx, 7h			                           	; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)										; rotate mask bit by x mod 8
		xor     cx, 7h                                      ; convert to bits to shift left
	    shl		dx, cl			                           	; shift colour bit & proto-mask into position
		not     dh                                          ; convert to mask
	    shr		ax, 1			                           	; calculate column byte x / 8
	    shr		ax, 1			                           	; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		mov 	bx, y0                                      ; BX load y0
		mov 	cx, y1                                      ; CX load y1
		cmp     bx, cx                                      ; ? x0 < x1
		jl      VJ0
		xchg    bx, cx                                      ; swap registers ie swap x0 x1
VJ0:	sub 	cx, bx										; convert CX line length
        shl     bx, 1                                       ; convert BX word pointer
VL0:	mov   	di, HGA_TABLE_Y_LOOKUP[bx]                  ; lookup y offset
		add   	di, ax                                      ; add in x / 8
		add 	bx, 2 										; next line
		and		es:[di], dh								    ; mask out target pixel
		or 		es:[di], dl									; or in the 'colour'
		loop 	VL0                                         ; for line length

END:
    }
}

/*
if abs(y1 - y0) < abs(x1 - x0)
    if x0 > x1
        plotLineLow(x1, y1, x0, y0)
    else
        plotLineLow(x0, y0, x1, y1)
    end if
else
    if y0 > y1
        plotLineHigh(x1, y1, x0, y0)
    else
        plotLineHigh(x0, y0, x1, y1)
    end if
end if

plotLineLow(x0, y0, x1, y1)
    dx = x1 - x0
    dy = y1 - y0
    yi = 1
    if dy < 0
        yi = -1
        dy = -dy
    end if
    D = (2 * dy) - dx
    y = y0

    for x from x0 to x1
        plot(x, y)
        if D > 0
            y = y + yi
            D = D + (2 * (dy - dx))
        else
            D = D + 2*dy
        end if

plotLineHigh(x0, y0, x1, y1)
    dx = x1 - x0
    dy = y1 - y0
    xi = 1
    if dx < 0
        xi = -1
        dx = -dx
    end if
    D = (2 * dx) - dy
    x = x0

    for y from y0 to y1
        plot(x, y)
        if D > 0
            x = x + xi
            D = D + (2 * (dx - dy))
        else
            D = D + 2*dx
        end if
*/
