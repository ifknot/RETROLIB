#include "hga_bresenham_line_naive.h"

#include "hga_table_lookup_y.h"

/**
* This version uses Bresenham's principles of integer incremental error to perform all octant line draws.
* Balancing the positive and negative error between the x and y coordinates.
*
* @url https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
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
void hga_bresenham_line_naive(uint16_t vram_segment, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour) {
    int16_t _sx = x0 < x1 ? 1 : -1;
    int16_t _sy = y0 < y1 ? 1 : -1;
     __asm {
		.8086
	    // set up VRAM segment in ES
	    mov   	ax, vram_segment
		mov   	es, ax
        // set up registers AX = x, BX = y, DX = dx, CX = dy, SI = error, DI = e2
        mov     dx, x1                              ; DX = x1
        sub     dx, x0                              ; DX = x1 - x0
JNZ0:   jge     J0                                  ; jump short if dx not signed i.e. positive
        neg     dx                                  ; negative so make positive
J0:     mov     cx, y1                              ; CX = y1
        sub     cx, y0                              ; CX = y1 - y0
JNZ1:   jge     J1                                  ; jump short if dy not signed i.e. positive
        neg     cx                                  ; negative so make positive
J1:     neg     cx                                  ; CX = -abs(dy)
        mov     si, dx                              ; SI = dx
        add     si, cx                              ; SI = error = dx + dy
        mov     ax, x0
        mov     bx, y0

        // line loop
L00P:   call    PLOT
        mov     di, si
		shl     di, 1                               ; DI = e2 = 2 * error
		cmp     di, cx                              ; ? e2 >= dy
		jl      L0J0
		cmp     ax, x1                              ; ? x0 == x1
		jne     L0J1
		jmp     END                                 ; if x0 == x1 break
L0J1:	add     si, cx                              ; error = error + dy
        add     ax, _sx                             ; x += sx
L0J0:	cmp     di, dx                               ; ? e2 <= dx
        jg      L00P
        cmp     bx, y1                               ; ? y0 == y1
        jne     L0J2
		jmp     END                                  ; if y0 == y1 break
L0J2:   add     si, dx                               ; error = error + dx
        add     bx, _sy                              ; y += sy
        jmp     L00P
        jmp     END
        // plot point
PLOT:   push    dx
        push    cx
        push    bx
        push    ax
        mov     cx, ax
        and     cx, 7
        xor     cx, 7
		mov     dl, colour
        mov     dh, 1
		shl		dx, cl
		not     dh
		shr		ax, 1
	    shr		ax, 1
	    shr		ax, 1
		shl     bx, 1
		mov   	bx, HGA_TABLE_Y_LOOKUP[bx]
		add     bx, ax
		and		es:[bx], dh
		or 		es:[bx], dl
		pop     ax
		pop     bx
		pop     cx
		pop     dx
        ret
END:
     }
}
