/**
 *
 *  @brief     HGA macro definitions for assembly
 *  @details   The founder of Hercules Computer Technology, Van Suwannukul, created the Hercules Graphics
 *             Card so that he could work on his doctoral thesis on an IBM PC using the Thai alphabet,
 *             which was impossible at the low resolution of CGA or the fixed character set of MDA.
 *             It initially retailed in 1982 for $499.
 *  @url https://en.wikipedia.org/wiki/Hercules_Graphics_Card
 *  Everything is hard coded to shave a few clock cycles off the execution time
 *  Using #define enables immediate addressing mode - in which the data operand is a part of the instruction itself
 *  This matters on an 8088 where reg,imm is only 4 clock cycles as opposed to 12+EA for reg,mem
 *  This advantage dwindles on the 286 (2:5) and 386 (2:4), vanishing on the 486(1:1) and beyond
 */
#ifndef HGA_CONSTANTS_H
#define HGA_CONSTANTS_H

 /**
 *  Hercules macro definitions for assembly
 */
#define HGA_SCREEN_X_MIN        0
#define HGA_SCREEN_X_MAX        720
#define HGA_SCREEN_Y_MIN        0
#define HGA_SCREEN_Y_MAX        348
#define HGA_SCREEN_Y_SCALE      0.483
#define HGA_PIXELS_PER_BYTE     8
#define HGA_BYTES_PER_LINE      90
#define HGA_WORDS_PER_LINE      45
#define HGA_BYTES_PER_SCREEN  32768           
#define HGA_WORDS_PER_SCREEN  16384
#define HGA_CHARS_PER_SCREEN  2000

#define HGA_BUFFER_0        0xB000
#define HGA_BUFFER_1        0xB800

 /**
 *  @def   HGA_VIDEO_RAM_SEGMENT
 *  @brief Hercules VRAM starts at B000:0000
 *  @details The Hercules display adapter displays in both text mode and graphics mode,
 *  with a graphics resolution of 720x348 pixels, and contains enough RAM for 2 page displays.
 *  Each display page is 32K, having the capacity to host a 4K text page and a graphics page.
 *  B000:0000 - B000:7FFF   First Page
 *  B000:8000 - B000:FFFF   Second Page
 */
#define HGA_VIDEO_RAM_SEGMENT   0B000h
#define HGA_PAGE_2_OFFSET       800h
#define HGA_BANK_OFFSET         2000h       // 8K per each of the 4 banks of VRAM

 // Write a CRTC register number (0 to 11h) to this port to select the CRTC register that will appear at port 03B5h
#define HGA_CRTC_ADDRESS_PORT       3B4h

// Read from this port to the latched data of the register requested at 3B4h
#define HGA_CRTC_DATA_PORT          3B5h

/**
*  @def   HGA_CONTROL_REGISTER
*  @brief Writing to port 03B8h has these extra effects (assuming the appropriate 3BFh bits have previously been enabled):
*      + If bit 1 is set, graphics mode is selected.
*      + If bit 7 is set, the graphics frame buffer is at 0B8000h. Otherwise it's at 0B000h.
*  @details In "full" mode, a Hercules card occupies memory between 0B0000h and 0BFFFFh, for two frame buffers.
*  In "half" mode, only one frame buffer is present, from 0B0000h to 0B7FFFh; this allows the card to be used in a dual-head configuration.
*          03B8
*          ?7?6?5?4?3?2?1?0?
*          ? ? ? ? ? ? ? ? ?
*          ?????????????????  bits
*           ? ? ? ? ? ? ? ???? 0:
*           ? ? ? ? ? ? ?????? 1:  0 = set text mode   1 = permit graphics mode
*           ? ? ? ? ? ???????? 2:
*           ? ? ? ? ?????????? 3:  0 = screeen off     1 = screen on
*           ? ? ? ???????????? 4:
*           ? ? ?????????????? 5:  0 = blink off       1 = blinking enabled
*           ? ???????????????? 6:
*           ?????????????????? 7:  0 = display screen page 1
*                                  1 = display screen page 2
*/
#define HGA_CONTROL_REGISTER        3B8h

// Read light pen values form this port
#define HGA_CRTC_LIGHT_PEN_LATCH    3B9h

/**
     *  @def   CRTC_STATUS_PORT
     *  @brief CRTC Status Register
     *  @details This is a read - only register - MDA only two bits of the value read from this port are defined but
     *  for a Hercules adapter a third bit (7) is available to read for vertical synchronization.
     *  Further, reading from port 03BAh gives Hercules card ID in bits 4-6:
     *          03BA
     *          ?7?6?5?4?3?2?1?0?
     *          ? ? ? ? ? ? ? ? ?
     *          ?????????????????  bits
     *           ? ? ? ? ? ? ? ???? 0:  H-Sync          0 = off     1 = on
     *           ? ? ? ? ? ? ?????? 1:
     *           ? ? ? ? ? ???????? 2:
     *           ? ? ? ? ?????????? 3:  Current pixel   0 = off     1 = on
     *           ? ? ? ???????????? 4:  0?          1?               1?
     *           ? ? ?????????????? 5:  0??Hercules 0??Hercules Plus 0??Hercules InColor
     *           ? ???????????????? 6:  0?          0?               1?
     *           ?????????????????? 7:  V-Sync          0 = *on*    1 = off
     *
     */
#define HGA_CRTC_STATUS_PORT        3BAh

     // Light Pen reset port - writing any value to this port resets the light pen
#define HGA_CRTC_LIGHT_PEN_RESET        3BBh

/**
*  @def   HGA_CONFIG_REGISTER
*  @brief Writing to port 03BFh enables extra bits in the Mode Control register at 03B8h:
*      + Set bit 0 of 3BFh to enable bit 1 of 3B8h
*      + Set bit 1 of 3BFh to enable bit 7 of 3B8h and the second 32k of RAM ("Full" mode).
*  @details An *additional* (write only) configuration register can be addressed from 3BFh.
*          03BF
*          ?7?6?5?4?3?2?1?0?
*          ? ? ? ? ? ? ? ? ?
*          ?????????????????  bits
*           ? ? ? ? ? ? ? ???? 0:  0 = text only   1 = permit graphics mode
*           ? ? ? ? ? ? ?????? 1:  0 = page 1 only 1 = permit page 2 VRAM
*           ? ? ? ? ? ???????? 2:
*           ? ? ? ? ?????????? 3:
*           ? ? ? ???????????? 4:
*           ? ? ?????????????? 5:
*           ? ???????????????? 6:
*           ?????????????????? 7:
*
*/
#define HGA_CONFIG_REGISTER             3BFh

// HGA Status Register at I/O address (same as CGA)
#define HGA_STATUS_REG                  3DAh

#endif
