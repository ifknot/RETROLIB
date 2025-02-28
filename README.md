# RETROLIB
Check out the [Wiki](https://github.com/ifknot/RETROLIB/wiki) for details on setting up Open Watcom 2 and CMake on Linux and Mac OS X

### DOING:
---
- [ ] bresenham line
  - [ ] white pixels
  - [ ] optimise
  - [ ] black pixels
- [ ] hga pixel perf compare algos
### TODO:
---
- [ ] demo hga pixelate text
  - [ ] hga_scroll_up
  - [ ] bible
  - [ ] shakespeare
  - [ ] move to done
---
- [ ] demo rectangle recursive generative Mondrian-esque
  - [ ] math_random
  - [ ] move to done
---
- [ ] code as a creative medium
  - [ ] demo buffer png anim to git readme
  - [ ] demo Mandelbrot fixed point
    - [ ] hga
    - [ ] incolor
    - [ ] cga
  - [ ] demo Mondrian
    - [ ] incolor
    - [ ] cga
- [ ] incolor video modes
- [ ] hga_scroll_down
- [ ] test file size/file pos works properly
---
- [ ] Q10.6
  - [ ] conversions
  - [ ] operators
  - [ ] trigonometry
  - [ ] 2D vectors
- [?] Q16.16
---
- [ ] Mandelbrot
  - [ ] Q10.6
  - [ ] Q16.16
---
- [ ] hga read pixel
---
- [ ] hga circle (unroll loop)
- [ ] demo circle
---
- [ ] hga byte aligned block transfer
- [ ] demo byte_aligned_blt
---
- [ ] draw some 1 bit playing cards for a nice change
- [ ] mouse input
- [ ] drag and drop playing cards
---
- [?] rectangle dashed line
- [?] pattern filled rectangle
---
### DONE
---
- [x] hga rectangle
  - [x] rectangle
- [x] hga filled rectangle
- [x] hga hline
- [x] hga vline
- [x] hga 2 types plot pixel
- [x] DOSAPPS\VIDEOINF.EXE
- [x] hga finish buffer demo
- [x] hga read write vram using y lookup
- [x] hga test ytable
- [x] hga fix MDA naming
- [X] convert video mode preset names and table
- [x] TEXT2BIT demo
- [x] 86Box on Mac https://github.com/86Box/86Box
- [x] MacBox for 86Box https://github.com/Moonif/MacBox
- [x] get it all working as below on ARM OS X
- [x] switch to Linux on T480
- [x] use git gui
- [x] compile open watcom 2
- [x] use Zed
- [x] wcl HELLO.EXE test use DOSBOX
- [x] convert to source and build directories within RETROLIB base directory
- [x] use CMake
- [x] cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS -D CMAKE_SYSTEM_PROCESSOR=I86 -S. -B /path_to/build_dir
- [x] cmake --build /path_to/build_dir --config DEBUG --target HELLOW
- [x] bld.sh
- [x] cmake outisde src -B ../
- [x] cmake OW2 C90 extensions -za99 (Pass remaining options to the native tool.)
- [x] configure versioning in .h files
- [x] does it work with comments? - yes
- [x] install and try cmake-gui
- [x] build from DBG and BIOS
- [x] ow2 model large fix errors using compiler option -ml
- [x] clean up headers

---
- [x] mem_arena
- [x] TIME into bios_tools_timer.h

### MAYBE

- [ ] getopt own version
- [ ] dos_tools_file_dump
---
- [ ] --graphviz

---
- [ ] use NASA's IKOS https:github.com/NASA-SW-VnV/ikos'

---
- [ ] get bbd-for-c to compile https:github.com/grassator/bdd-for-c

---
- [ ] use bdd-for-c
