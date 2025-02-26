#include "hga_bresenham_line.h"

#include "hga_table_lookup_y.h"

/**
* hard coded colour and octant paths of executuion for maximum performance
*/
void hga_bresenham_line(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour) {
    __asm {
		.8086
	    // set up VRAM segment in ES
	    mov   	ax, vram_segment
		mov   	es, ax
		// select hard code colour path
		cmp     colour, 0
		jz      BLACK
        // set up registers
WHITE:  mov     ax, x1                                  ; AX = x1
        sub     ax, x0                                  ; AX = dx = (x1 - x0)
        mov     si, y1                                  ; SI = y1
        sub     si, y0                                  ; SI = dy = (y1 - y0)
        mov     cx, si                                  ; CX = dy
        sub     cx, ax                                  ; CX = (dy - dx)
        shl     si, 1                                   ; SI = (2 * dy)
        shl     cx, 1                                   ; CX = 2 * (dy - dx)
        mov     dx, si                                  ; D = (2 * dy)
        sub     dx, ax                                  ; D = (2 * dy) - dx
        mov     bx, y0                                  ; BX is y
        mov     ax, x0                                  ; AX is x

        // draw white line octant 0
WLINE0: push    dx                                      ; preserve corrupted registers
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
		jle     WELSE
		inc     bx                                      ; y++
        add     dx, cx                                  ; D = D + (2 * (dy - dx))
        inc     ax
        cmp     ax, x1
        jle     WLINE0
        jmp     END
WELSE:  add     dx, si                                  ; D = D + 2*dy
        inc     ax
        cmp     ax, x1
        jle     WLINE0
        jmp     END

        // set up registers
BLACK: mov     ax, x1                                  ; AX = x1
        sub     ax, x0                                  ; AX = dx = (x1 - x0)
        mov     si, y1                                  ; SI = y1
        sub     si, y0                                  ; SI = dy = (y1 - y0)
        mov     cx, si                                  ; CX = dy
        sub     cx, ax                                  ; CX = (dy - dx)
        shl     si, 1                                   ; SI = (2 * dy)
        shl     cx, 1                                   ; CX = 2 * (dy - dx)
        mov     dx, si                                  ; D = (2 * dy)
        sub     dx, ax                                  ; D = (2 * dy) - dx
        mov     bx, y0                                  ; BX is y
        mov     ax, x0                                  ; AX is x

        // draw black line octatnt 0
BLINE0:  push    dx                                      ; preserve corrupted registers
        push    cx
        push    ax
        // plot pixel
        mov     cx, ax                                  ; copy of x low order byte
        and     cx, 7                                   ; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
        xor     cx, 7                                   ; CL = number of bits to shift left (thanks bit flip XOR)
        mov     dh, 1
		shl		dh, cl			                        ; shift colour bit & proto-mask into position
		not     dh                                      ; convert to mask
		shr		ax, 1			                        ; calculate column byte x1 / 8
	    shr		ax, 1			                        ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		shl     bx, 1                                   ; convert BX word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bx]
		add     di, ax                                  ; ES:[DI] points to VRAM byte containing pixel location
		shr     bx, 1
		and		es:[di], dh								; mask out target pixel
		//calculate next pixel
		pop     ax
		pop     cx
		pop     dx                                      ; restore registers
		// decision variable
		cmp     dx, 0                                   ; if D > 0
		jle     BELSE
		inc     bx                                      ; y++
		add     dx, cx                                  ; D = D + (2 * (dy - dx))
		inc     ax
        cmp     ax, x1
        jle     BLINE0
BELSE:  add     dx, si                                  ; D = D + 2*dy
        inc     ax
        cmp     ax, x1
        jle     BLINE0

END:

    }
}

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
