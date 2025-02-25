#include "hga_bresenham_line.h"

#include "hga_table_lookup_y.h"

void hga_bresenham_line(uint16_t vram_segment, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t colour) {
    __asm {
		.8086
	    // 1. set up VRAM segment in ES
	    mov   	ax, vram_segment
		mov   	es, ax
        // 2. set up registers
        mov     cx, x2
        sub     cx, x1                                  ; CX is delta_x
        mov     si, y2
        sub     si, y1                                  ; SI is delta_y
        mov     dx, bx                                  ; DX is decision variable
        shl     dx,  1                                  ; 2 * delta_y
        sub     dx, cx                                  ; 2 * delta_y - delta
        mov     bx, y1                                  ; BX is y
        mov     ax, x1                                  ; AX is x
        push    bp                                      ; preserve BP for __asm

        // plot pixel
        push    dx                                      ; preserve corrupted registers
        push    cx
        push    ax

        mov     cx, ax                                  ; copy of x low order byte
        and     cx, 7                                   ; mask off 0111 lower bits ie x mod 8 (thanks powers of 2)
        xor     cx, 7                                   ; CL = number of bits to shift left (thanks bit flip XOR)
       	mov   	dh, 00000001                            ; DH is (proto)mask byte
		mov     dl, colour                              ; DL load 'colour'
		shl		dx, cl			                        ; shift colour bit & proto-mask into position
		not     dh                                      ; convert to mask
		shr		ax, 1			                        ; calculate column byte x1 / 8
	    shr		ax, 1			                        ; poor old 8086 only has opcodes shifts by an implicit 1 or CL
	    shr		ax, 1
		mov 	bp, bx									; BP load y
	    shl     bp, 1                                   ; convert BP word pointer
		mov   	di, HGA_TABLE_Y_LOOKUP[bp]
		add     di, ax                                  ; ES:[DI] points to VRAM byte containing pixel location
		and		es:[di], dh								; mask out target pixel
		or 		es:[di], dl							    ; or in the 'colour'

		pop     ax
		pop     cx                                      ; restore registers
		pop     dx

		pop     bp                                      ; restore BP for graceful function exit
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
