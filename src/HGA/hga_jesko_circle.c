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

void hga_jesko_circle(uint16_t vram_segment, uint16_t x, uint16_t y, uint16_t r) {

}
