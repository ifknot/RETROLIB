#include "hga_bresenham_line.h"

#include "hga_table_lookup_y.h"

#include <stdio.h>

/**
* This versuion uses Bresenham's principles of integer incremental error to perform all octant line draws.
* Balancing the positive and negative error between the x and y coordinates.
* @url https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
* My implementation uses 4 execution pathways with hard coded inc and dec for improved performace.
* The 4 quadrants (x++ y++), (x++, y--), (x--, y++), and (x--, y--)
*
* plotLine(x0, y0, x1, y1)
*     dx = abs(x1 - x0)
*     sx = x0 < x1 ? 1 : -1
*     dy = -abs(y1 - y0)
*     sy = y0 < y1 ? 1 : -1
*     error = dx + dy
*
*     while true
*         plot(x0, y0)
*         e2 = 2 * error
*         if e2 >= dy
*             if x0 == x1 break
*             error = error + dy
*             x0 = x0 + sx
*         end if
*         if e2 <= dx
*             if y0 == y1 break
*             error = error + dx
*             y0 = y0 + sy
*         end if
*     end while
*/
void hga_bresenham_line(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour) {
    int16_t _x, _y, _dx, _dy, _e;
    __asm {
		.8086

		mov ax, x1
		mov bx, y1
	    mov _x, ax
        mov _y, bx

	    // set up VRAM segment in ES
	    mov   	ax, vram_segment
		mov   	es, ax
        // set up registers AX = x, BX = y, DX = dx, CX = dy, SI = error
        mov     dx, x1                              ; DX = x1
        sub     dx, x0                              ; DX = x1 - x0
        cmp     dx, 0                               ; DX = abs(dx)
        jge     J0                                  ; jump short if dx not signed i.e. positive
        neg     dx                                  ; negative so make positive
J0:     mov     cx, y1                              ; CX = y1
        sub     cx, y0                              ; CX = y1 - y0
        cmp     cx, 0
        jge     J1                                  ; jump short if dy not signed i.e. positive
        neg     cx                                  ; negative so make positive
J1:     neg     cx                                  ; CX = -abs(dy)
        mov     si, dx                              ; SI = dx
        add     si, cx                              ; SI = error = dx + dy
        mov     ax, x0
        mov     bx, y0

        mov     _dx, dx
        mov     _dy, cx
        mov     _e, si
        // select hard code colour path
		cmp     colour, 0
		jnz     WHITE
		jmp     BLACK
        // white function dispatch
WHITE:  cmp     ax, x1
        jl      J2                                  ; x0 < x1 so use x++
        cmp     bx, y1
        jg      J3                                  ; y0 > y1 so use y--
        call    L1P                               ; x-- y++
        jmp     END0
J3:     call    L3P                               ; x-- y--
        jmp     END0
J2:     cmp     bx, y1
        jg      J4                                  ; y0 > y1 so use y--
        call    L0P                                 ; x++ y++
        jmp     END0
J4:     call    L2P                                 ; x++ y--
        jmp     END0
BLACK:
        // Bresenham loop x++ y++
L0P:    push    dx                                  ; preserve corrupted registers
        push    cx
        push    bx
        push    ax
        // plot pixel
        mov     cx, ax                              ; copy of x low order byte
        and     cx, 7                               ; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
        xor     cx, 7                               ; CL = number of bits to shift left (thanks bit flip XOR)
		mov     dx, 101h                            ; DX = 0000000100000001 binary protomask DH colour DL
		shl		dx, cl                              ; shift colour bit & proto-mask into position
		not     dh                                  ; convert to mask
		shr		ax, 1			                    ; calculate column byte x1 / 8
	    shr		ax, 1			                    ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		shl     bx, 1                               ; convert BX word pointer
		// TODO: use BP with shr bx,1 (2 clocks) to save POP PUSH (19 cycles) in the main loop
		mov   	bx, HGA_TABLE_Y_LOOKUP[bx]          ; BX = VRAM row offset
		add     bx, ax                              ; ES:[BX] points to VRAM byte containing pixel location
		and		es:[bx], dh						    ; mask out target pixel
		or 		es:[bx], dl							; or in the 'colour'
		// restore registers AX = x, BX = y, DX = dx, CX = dy
		pop     ax
		pop     bx
		pop     cx
		pop     dx
		// calculate e2
		mov     di, si
		shl     di, 1                                ; DI = e2 = 2 * error
		// if e2 >= dy
		cmp     di, cx                               ; ? e2 >= dy
		jl      L0J0
		cmp     ax, x1
		jl      L0J1
		ret                                          ; if x0 == x1 return
L0J1:	add     si, cx                               ; error = error + dy
		inc     ax                                   ; x++
		// if e2 <= dx
L0J0:	cmp    di, dx                                ; ? e2 <= dx
        jg     L0P
        cmp    bx, y1
        jl      L0J2
		ret                                         ; if x0 == x1 return
L0J2:   add    si, dx                               ; error = error + dx
        inc    bx                                   ; y++
        jmp    L0P

        // Bresenham loop x-- y++
L1P:    push    dx
        push    cx
        push    bx
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
		shl     bx, 1
		// TODO: use BP with shr bx,1 (2 clocks) to save POP PUSH (19 cycles) in the main loop
		mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
		add     bx, ax
		and		es:[bx], dh
		or 		es:[bx], dl
		pop     ax
		pop     bx
		pop     cx
		pop     dx
		mov     di, si
		shl     di, 1
		cmp     di, cx
		jl      L1J0
		cmp     ax, x1
		jg      L1J1
		ret
L1J1:	add     si, cx
		dec     ax                                   ; x--
L1J0:	cmp     di, dx
        jg      L1P
        cmp     bx, y1
        jl      L1J2
        ret
L1J2:   add     si, dx
        inc     bx                                    ; y++
        jmp     L1P

		// Bresenham loop x++ y--
L2P:    push    dx
        push    cx
        push    bx
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
        shl     bx, 1
        // TODO: use BP with shr bx,1 (2 clocks) to save POP PUSH (19 cycles) in the main loop
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        or 		es:[bx], dl
        pop     ax
        pop     bx
        pop     cx
        pop     dx
        mov     di, si
		shl     di, 1
		cmp     di, cx
		jl      L2J0
		cmp     ax, x1
		jl      L2J1
		ret
L2J1:	add     si, cx
        inc     ax                                   ; x++
L2J0:	cmp     di, dx
        jg      L2P
        cmp     bx, y1
        jg      L2J2
        ret
L2J2:   add     si, dx
        dec     bx                                    ; y--
        jmp     L2P

        // Bresenham loop x-- y--
L3P:    push    dx
        push    cx
        push    bx
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
        shl     bx, 1
        // TODO: use BP with shr bx,1 (2 clocks) to save POP PUSH (19 cycles) in the main loop
        mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
        add     bx, ax
        and		es:[bx], dh
        or 		es:[bx], dl
        pop     ax
        pop     bx
        pop     cx
        pop     dx
        mov     di, si
		shl     di, 1
		cmp     di, cx
		jl      L3J0
		cmp     ax, x1
		jg      L3J1
		ret
L3J1:	add     si, cx
        dec     ax                                   ; x--
L3J0:	cmp     di, dx
        jg      L3P
        cmp     bx, y1
        jg      L3J2
        ret
L3J2:   add     si, dx
        dec     bx                                    ; y--
        jmp     L3P



END0:

    }
    printf("x = %i y=%i dx=%i dy=%i e=%i\n", _x, _y, _dx, _dy, _e);
}

/*


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

plotLine(x0, y0, x1, y1)
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

/*

plotLine(x0, y0, x1, y1)
    dx = x1 - x0
    dy = y1 - y0
    D = 2*dy - dx
    y = y0

    for x from x0 to x1
        plot(x, y)
        if D > 0
            y = y + 1
            D = D - 2*dx
        end if
        D = D + 2*dy

*/

/*

// calculate delata x 'dx' and delta y 'dy'
WHITE:
mov ax, y0
cmp ax, y1
jle  W_INC_Y_LINE
jmp W_DEC_Y_LINE

W_INC_Y_LINE: //white line increasing values of y
// setup registers
mov     ax, x1                                  ; AX = x1
sub     ax, x0                                  ; AX = dx = (x1 - x0)

mov     si, y1                                  ; SI = y1
sub     si, y0                                  ; SI = dy = (y1 - y0)

mov     dx, si                                  ; DX = D = dy
add     dx, dx                                  ; D = (2 * dy)
sub     dx, ax                                  ; D = (2 * dy) - dx

mov     cx, ax                                  ; CX = dx
add     cx, cx                                  ; CX = 2 * dx

add     si, si                                  ; SI = 2 * dy

mov     ax, x0                                  ; AX is x
mov     bx, y0                                  ; BX is y

// plot pixel
WPLOT0: push    dx                                      ; preserve corrupted registers
push    cx
push    bx
push    ax
// plot pixel
mov     cx, ax                                  ; copy of x low order byte
and     cx, 7                                   ; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
xor     cx, 7                                   ; CL = number of bits to shift left (thanks bit flip XOR)
		mov     dx, 101h
		shl		dx, cl			                        ; shift colour bit & proto-mask into position
		not     dh                                      ; convert to mask
		shr		ax, 1			                        ; calculate column byte x1 / 8
	    shr		ax, 1			                        ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		shl     bx, 1                                   ; convert BX word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bx]
		add     di, ax                                  ; ES:[DI] points to VRAM byte containing pixel location
		and		es:[di], dh								; mask out target pixel
		or 		es:[di], dl							    ; or in the 'colour'
		// calulate next pixel
		pop     ax
		pop     bx
		pop     cx
		pop     dx                                      ; restore registers
		// decision variable
		cmp     dx, 0                                   ; if D > 0
		jg      WELSE0
		add     dx, si                                  ; D = D + (2 * dy)
		inc     ax                                      ; x++
cmp     ax, x1                                  ; x0 to x1
jle     WPLOT0
jmp     END
WELSE0: sub     dx, cx                                  ; D = D - (2 * dx)
inc     bx                                      ; y++
inc     ax                                      ; x++
cmp     ax, x1                                  ; x0 to x1
jle     WPLOT0
jmp     END

W_DEC_Y_LINE: //white line decreasing values of y setup registers
// setup registers
mov     ax, y1                                  ; AX = y1
sub     ax, y0                                  ; AX = dy = (y1 - y0)
mov     si, x1                                  ; SI = x1
sub     si, x0                                  ; SI = dx = (x1 - x0)
mov     dx, si                                  ; DX = D = dx
shl     dx, 1                                   ; D = (2 * dx)
sub     dx, ax                                  ; D = (2 * dx) - dy
mov     cx, si                                  ; CX = dx
sub     cx, ax                                  ; CX = (dx - dy)
shl     cx, 1                                   ; CX = 2 * (dx - dy)
shl     si, 1                                   ; SI = 2 * dx


mov     bx, y0                                  ; BX is y
mov     ax, x0                                  ; AX is x
// plot pixel
WPLOT2: push    dx                                      ; preserve corrupted registers
push    cx
push    bx
push    ax
// plot pixel
mov     cx, ax                                  ; copy of x low order byte
and     cx, 7                                   ; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
xor     cx, 7                                   ; CL = number of bits to shift left (thanks bit flip XOR)
		mov     dx, 101h
		shl		dx, cl			                        ; shift colour bit & proto-mask into position
		not     dh                                      ; convert to mask
		shr		ax, 1			                        ; calculate column byte x1 / 8
	    shr		ax, 1			                        ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		shl     bx, 1                                   ; convert BX word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bx]
		add     di, ax                                  ; ES:[DI] points to VRAM byte containing pixel location
		//shr     bx, 1
		and		es:[di], dh								; mask out target pixel
		or 		es:[di], dl							    ; or in the 'colour'
		// calulate next pixel
		pop     ax
		pop     bx
		pop     cx
		pop     dx                                      ; restore registers
		// decision variable
		cmp     dx, 0                                   ; if D > 0
		jle     WELSE2
		inc     ax                                      ; x++
add     dx, cx                                  ; D = D + (2 * (dx - dy))
dec     bx                                      ; y--
cmp     bx, y1
jge     WPLOT2
jmp     END
WELSE2: add     dx, si                                  ; D = D + 2*dx
dec     bx                                      ; y--
cmp     bx, y1
jge     WPLOT2
jmp     END


*/
