#ifndef HGA_G_FUNCTIONS_H
#define HGA_G_FUNCTIONS_H

#include "hga.h"

#include "../GRAPHICS/g_types.h"
#include "../GRAPHICS/g_bounding_functions.h"

void g_draw_pixel(g_point_t*, g_attributes_t*, g_bounding_function*);

void g_draw_line(g_point_t*, g_point_t*, g_attributes_t*, g_bounding_function*);

void g_draw_rectangle(g_rectangle_t*, g_attributes_t*, g_bounding_function*);

void g_draw_filled_rectangle(g_rectangle_t*, g_attributes_t*, g_bounding_function*);

#endif
