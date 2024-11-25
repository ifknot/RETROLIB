#ifndef CGA_MODE4_CONSTANTS_H
#define CGA_MODE4_CONSTANTS_H

#include <stdint.h>

 /**
  * Mode 4 macro definitions for assembly
  * 
  * Graphic Mode: 320x200, 2-bit Color - this was the most popular CGA mode used for graphics. 
  * The display resolution is 320x200 pixels with a pixel ratio of 1:1.2 (visually 320x240). 
  * Only four colors could be displayed on the screen at a time, and they were mostly fixed, which is why so many CGA games look the same.
  *
  * Everything is hard coded to shave a few clock cycles off the execution time
  * Using #define enables immediate addressing mode - in which the data operand is a part of the instruction itself
  * This matters on an 8088 where reg,imm is only 4 clock cycles as opposed to 12+EA for reg,mem
  * This advantage dwindles on the 286 (2:5) and 386 (2:4), vanishing on the 486(1:1) and beyond
  */
#define CGA_MODE4_SCREEN_X_MIN 0
#define CGA_MODE4_SCREEN_X_MAX 320
#define CGA_MODE4_SCREEN_Y_MIN 0
#define CGA_MODE4_SCREEN_Y_MAX 200
#define CGA_MODE4_SCREEN_Y_SCALE 0.625
#define CGA_MODE4_PIXELS_PER_BYTE		4

#endif
