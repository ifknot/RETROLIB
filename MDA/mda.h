/**
 *
 *  @brief     Monochrome Display Adapter
 *  @details   4k of RAM at address 0B0000h for its display buffer. 
 * This address is not completely decoded; the entire 32k from 0B0000h to 0B7FFFh is filled 
 * with repeats of this 4k area.
 * I/O addresses 03B0h-03BFh.
 * The printer port uses IRQ7.
 *  @url https://www.seasip.info/VintagePC/mda.html
 *  @author    Jeremy Thornton
 *  @date      25.05.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef MDA_H
#define MDA_H

#include "mda_toolbox.h"

#endif