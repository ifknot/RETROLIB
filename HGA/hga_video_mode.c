/**
 *
 *  @file      hga_video_mode.c
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      29.09.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "hga_video_mode.h"

void hga_video_mode(const uint8_t* presets, uint8_t flags_config, uint8_t flags_ctrl) {
  __asm {
    .8086
      pushf                    ; preserve flags on entry(direction flag used)
    
      // 1. configure the Hercules control register 
      mov     dx, HGA_CONFIG_REGISTER
      mov     al, flags_config
      out     dx, al
      // 2. set the Hercules control register 
      mov     dx, HGA_CONTROL_REGISTER
      mov     al, flags_ctrl                   
      out     dx, al
      // 3. program the CRTC registers w presets data 
      lds     si, presets
      cld								        ; increment mode
      mov     cx, 16						; 16 registers of the 6845
      mov     dx, HGA_CRTC_ADDRESS_PORT
    // 3.1 write out the values to the registers
L0:   lodsw                     ; al = register ah = data
      out     dx, ax            ; write data to 6845 register
      loop    L0
      // 4. screen on
      mov     dx, HGA_CONTROL_REGISTER
      mov     al, flags_ctrl
      or      al, 00001000b     ; bit 3 = 0 screen on
      out     dx, al
    
      popf											; restore flags on exit
  }
}
