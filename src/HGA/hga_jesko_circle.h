#ifndef HGA_JESK_CIRCLE_H
#define HGA_JESK_CIRCLE_H

#include <stdint.h>

/**
* @url https://schwarzers.com/algorithms/
*/
void hga_jesko_circle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t r, uint8_t colour);

#endif

/*
Beside Bresenham some others have tried to draw circles on raster displays and plotters.

Method of Horn​

d = −r
x = r
y = 0
Repeat until y > x {
    Pixel (x, y) and symmetrical pixel are colored
    d = d + 2×y + 1
    y++
    If d > 0 {
        d = d - 2×x + 2
        x--
    }
}

Midpoint Method

x = r
y = 0
P = 1 - r
while ( x > y ) {}
    Pixel (x, y) and symmetrical pixel are colored
    y++
    if ( P <= 0 ) {
        // Mid-point is inside or on the perimeter
        P = P + 2*y + 1
    }
    else {
        // Mid-point is outside the perimeter
        x--
        P = P + 2*y - 2*x + 1
    }
}

Jesko's Method

y = 0;
t1 = r >> 4;
x = r;
while ( x >= y ) {
    Pixel (x, y) and symmetrical pixel are colored
    y++
    t1 += y
    t2 = t1 - x
    if ( t2 >= 0 ) {
        t1 = t2
        x--
    }
}
*/
