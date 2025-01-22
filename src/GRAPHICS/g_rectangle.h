#ifndef G_RECTANGLE_H
#define G_RECTANGLE_H

#inclde <stdint.h>

#include "../HGA/hga.h"

#include "g_types.h" 
#include "g_boundry_handlers.h" 
/*   For a function that returns an int and takes a char you would do
    typedef int (*functiontype2)(char);
    and to use it
    int dosomethingwithchar(char a) { return 1; }
*/

functiontype2 func2 = &dosomethingwithchar
int result = func2('a');

void g_draw_rectangle(g_rectangle_t* r, );

#endif
