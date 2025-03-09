#include "hga_bresenham_line.h"

#include "hga_table_lookup_y.h"

#include <stdio.h>

/**
* Register optimised
*
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
*/
void hga_bline0(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour) {
    int16_t _dx, _dy, _abs_dx, _abs_dy;
    __asm {
		.8086
	    // set up VRAM segment in ES
	    mov   	ax, vram_segment
		mov   	es, ax
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
*/
		// set up registers AX = x, BX = y, DX = dx, CX = dy, DI = D
		mov     dx, x1
		sub     dx, x0                            ; dx = x1 - x0
		mov     _dx, dx
		mov     ax, dx                            ; copy dx
		jge     ABS_DX
		neg     dx                                ; ax = abs(x1 - x0)
ABS_DX: mov     _abs_dx, dx
		mov     cx, y1
		sub     cx, y0                            ; dy = y1 - y0
		mov     _dy, cx
		mov     bx, cx                            ; copy dy
		jge     ABS_DY
		neg     cx                                ; bx = abs(y1 - y0)
ABS_DY: mov     _abs_dy, cx

        cmp     cx, dx                            ; if abs(y1 - y0) < abs(x1 - x0)
        jge     ELSE_ABS
        mov     dx, ax                            ; restore dx
        mov     cx, bx                            ; restore dy
        mov     ax, x0                            ; x = x0
        cmp     ax, x1                            ; if x0 > x1
        jle     ELSE_X0_X1
        // plotLineLow(x1, y1, x0, y0)
        jmp END
ELSE_X0_X1:
        // plotLineLow(x0, y0, x1, y1)
        mov     bx, y0                            ; y = y0
        jmp     PLOT_LINE_LOW
ELSE_ABS:
        jmp END

PLOT_LINE_LOW:
        mov     di, cx                            ; D = dy
		add     di, cx                            ; D = 2 * dy
		sub     di, dx                            ; D = (2 * dy) - dx
		mov     ax, x0                            ; x = x0
		mov     bx, y0                            ; y = y0

		cmp     cx, 0                             ; if dy < 0
		jge     OCTANT_0
		jmp     END//OCTANT_1
OCTANT_0:
        mov     si, cx                              ; SI = dy
        sub     si, dx                              ; SI = dy - dx
        shl     si, 1                               ; SI = 2 * (dy - dx)
        //push    bp
        //mov     bp, x1
    	// plot(x,y)
PLOT_0:	push    dx
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

        cmp     di, 0                               ; if D > 0
        jle     ELSE_0
        inc     bx                                  ; y++
        add     di, si                              ; D = D + (2 * (dy - dx))

        inc     ax
        cmp     ax, x1//bp
        jne     PLOT_0
        jmp     END
ELSE_0:
        add     di, cx
        add     di, cx                              ; D = D + 2*dy
        inc     ax
        cmp     ax, x1//bp
        jne     PLOT_0
        jmp     END

OCTANT_1:
        neg     cx                                  ; dy = -dy ie abs(dy)
        mov     si, cx                              ; SI = dy
        sub     si, dx                              ; SI = dy - dx
        shl     si, 1                               ; SI = 2 * (dy - dx)
        //push    bp
        //mov     bp, x1
PLOT_1:
        // plot(x,y)
        push    dx
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

        cmp     di, 0                               ; if D > 0
        jle     ELSE_1
        dec     bx                                  ; y--
        add     di, si                              ; D = D + (2 * (dy - dx))

        inc     ax
        cmp     ax, x1//bp
        jne     PLOT_1
        jmp     END
ELSE_1:
        add     di, cx
        add     di, cx                              ; D = D + 2*dy
        inc     ax
        cmp     ax, x1//bp
        jne     PLOT_1
        jmp     END



END:    //pop     bp
    }

    printf(" (%i, %i, %i, %i)",_dx, _dy, _abs_dx, _abs_dy);

}

/**
* This version uses Bresenham's principles of integer incremental error to perform all octant line draws.
* Balancing the positive and negative error between the x and y coordinates.
*
* @url https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
* @url https://zingl.github.io/bresenham.html
*
* Performance gains can be made be de-generalizing the algorithm into hard-coded execution paths at the expense of increased memory footprint:
* + My implementation uses 4 execution pathways with hard coded inc and dec for improved diagonal performace 7.5% faster.
* + Further, splitting the code into specific colour sections gains 17% faster.
* + Selecting horizontal and vertical optimised routines delivers 89.8% speed gains for horizontal and vertical line cases!
* + The canonical computing dichotomy, space and time.
*
* plotLine(x0, y0, x1, y1) {
*     dx = abs(x1 - x0)
*     sx = x0 < x1 ? 1 : -1
*     dy = -abs(y1 - y0)
*     sy = y0 < y1 ? 1 : -1
*     error = dx + dy
*
*     while true {
*         plot(x0, y0)
*         e2 = 2 * error
*         if e2 >= dy {
*             if x0 == x1 break
*             error = error + dy
*             x0 = x0 + sx
*         }
*         if e2 <= dx {
*             if y0 == y1 break
*             error = error + dx
*             y0 = y0 + sy
*         }
*     }
* }
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
