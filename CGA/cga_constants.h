/**
 *
 *  @file      cga_constants.h
 *  @brief     CGA macro definitions for assembly
 *  @details   Everything is hard coded to shave a few clock cycles off the execution time
 *  Using #define enables immediate addressing mode - in which the data operand is a part of the instruction itself
 *  This matters on an 8088 where reg,imm is only 4 clock cycles as opposed to 12+EA for reg,mem
 *  This advantage dwindles on the 286 (2:5) and 386 (2:4), vanishing on the 486(1:1) and beyond
 *  @author    Jeremy Thornton
 *  @date      2.04.2022
 *  @copyright © Jeremy Thornton, 2022. All right reserved.
 *
 */
#ifndef CGA_CONSTANTS_H
#define CGA_CONSTANTS_H

#include <stdint.h>

/**
 *  @def   CGA_VIDEO_RAM_SEGMENT
 *  @brief CGA VRAM starts at B8000 (736K bytes into the 8086 addressable 1MB)
 *  @details Both Low-res 320x200, 4-color (video modes 04H and 05H) and Hi-res 640x200, 2-color (video mode 06H) 
 *  Layout : Interleaved scan lines, packed - pixel.
 *  Even - numbered scan lines begin at segment B800
 *  Odd - numbered scan lines begin at segment BA00
 */
#define CGA_VIDEO_RAM_SEGMENT	0B800h
#define CGA_EVEN_VRAM_SEGMENT	0B800h
#define CGA_ODD_VRAM_SEGMENT	0BA00h
#define CGA_BYTES_PER_LINE		80

// CGA Mode Control Register at I/O address 
#define CGA_CONTROL_REG			3D8h
// CGA Status Register at I/O address 
#define CGA_STATUS_REG			3DAh

// When bit 0 of the CGA Mode Control Register is set the CPU can touch the CGA buffer without causing snow
#define CGA_CTRL_BIT0	1
// When bit 3 of the CGA Mode Control Register is set during vertical retrace.
#define CGA_CTRL_BIT3	8

#endif
