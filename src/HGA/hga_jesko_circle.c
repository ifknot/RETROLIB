#include "hga_jesko_circle.h"

#include "hga_table_lookup_y.h"

/*
circle( int mx, int my, int r ) {
    int t1 = r / 16;
    int t2 = 0;
    int x  = r;
    int y  = 0;
    while ( x >= y ) {
        plot( mx + x, my + y );
        plot( mx + x, my - y );
        plot( mx - x, my + y );
        plot( mx - x, my - y );
        plot( mx + y, my + x );
        plot( mx + y, my - x );
        plot( mx - y, my + x );
        plot( mx - y, my - x );
        y  = y + 1;
        t1 = t1 + y;
        t2 = t1 - x;
        if ( t2 >= 0 ) {
            t1 = t2;
            x  = x - 1;
        }
    }
}
*/

void hga_jesko_circle(uint16_t vram_segment, uint16_t mx, uint16_t my, uint16_t r) {
    __asm {
		.8086
	    // set up VRAM segment in ES
	    mov    ax, vram_segment
		mov    es, ax
		// set up registers AX = mx, BX = my, CX = x, DX = y, SI = t1, DI = t2
		mov    ax, mx
		mov    bx, my
		mov    cx, r
		mov    dx, 0
		mov    si, cx                     ; copy r
		shr    si, 1
		shr    si, 1
		shr    si, 1
		shr    si, 1                      ; r / 16
		mov    di, 0
        // plot pixels in each octant NB repeated sub/add reg, reg (3 cycles) faster than moving from memory again (8 cyckes + EA)
PLOT:   cmp    cx, dx
        jl     END                          ; while ( x >= y )
        add    ax, cx                       ; mx + x
        add    bx, dx                       ; my + y
        call   PIXEL                        ; plot( mx + x, my + y );
        sub    ax, cx
        sub    bx, dx
        /*
        sub    bx, dx
        sub    bx, dx
        call   PIXEL                        ; plot( mx + x, my - y );
        sub    ax, cx
        sub    ax, cx
        call   PIXEL                        ; plot( mx - x, my - y );
        add    bx, dx
        add    bx, dx
        call   PIXEL                        ; plot( mx - x, my + y );

        add    ax, cx
        add    ax, dx
        sub    bx, dx
        add    bx, cx
        call   PIXEL                        ; plot( mx + y, my + x );
        sub    bx, cx
        sub    bx, cx
        call   PIXEL                        ; plot( mx + y, my - x );
        sub    ax, dx
        sub    ax, dx
        call   PIXEL                        ; plot( mx - y, my - x );
        add    bx, cx
        add    bx, cx
        call   PIXEL                        ; plot( mx - y, my + x );
        add    ax, dx                       ; reset mx and my
        sub    dx, cx
        */
        // increment/decrement decision
        inc    dx                           ; y++
        add    si, dx                       ; t1 = t1 + y
        mov    di, si                       ; t2 = t1
        sub    di, cx                       ; t2 = t1 - x
        cmp    di, 0
        jl     PLOT                         ; if ty >= 0
        mov    si, di                       ; t1 = t2
        dec    cx                           ; x--
        jmp    PLOT

PIXEL:  push    dx
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
