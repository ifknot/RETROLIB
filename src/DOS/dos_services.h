/*
* @url http://bitsavers.informatik.uni-stuttgart.de/pdf/borland/turbo_c/Turbo_C_Reference_Guide_1987.pdf
*/
#ifndef DOS_SERVICES_H
#define DOS_SERVICES_H

#include <stdint.h>

#include "dos_services_constants.h"
#include "dos_services_types.h"

// 0  Program terminate
// 1  Keyboard input with echo
// 2  Display output
// 3  Wait for auxiliary device input
// 4  Auxiliary output
// 5  Printer output
// 6  Direct console I/O
// 7  Wait for direct console input without echo
// 8  Wait for console input without echo
// 9  Print string
// A  Buffered keyboard input
// B  Check standard input status
// C  Clear keyboard buffer, invoke keyboard function
// D  Disk reset
// E  Select disk
// F  Open file using FCB
// 10  Close file using FCB
// 11  Search for first entry using FCB
// 12  Search for next entry using FCB
// 13  Delete file using FCB
// 14  Sequential read using FCB
// 15  Sequential write using FCB
// 16  Create a file using FCB
// 17  Rename file using FCB
// 18  DOS dummy function (CP/M) (not used/listed)
// 19  Get current default drive
// 1A  Set disk transfer address
// 1B  Get allocation table information
// 1C  Get allocation table info for specific device
// 1D  DOS dummy function (CP/M) (not used/listed)
// 1E  DOS dummy function (CP/M) (not used/listed)
// 1F  Get pointer to default drive parameter table (undocumented)
// 20  DOS dummy function (CP/M) (not used/listed)
// 21  Random read using FCB
// 22  Random write using FCB
// 23  Get file size using FCB
// 24  Set relative record field for FCB

// 25  Set interrupt vector
void dos_set_interrupt_vector(uint8_t vec_num, void* phandler);

// 26  Create new program segment
// 27  Random block read using FCB
// 28  Random block write using FCB
// 29  Parse filename for FCB
// 2A  Get date
// 2B  Set date
// 2C  Get time
// 2D  Set time
// 2E  Set/reset verify switch
// 2F  Get disk transfer address
// 30  Get DOS version number
// 31  Terminate process and remain resident
// 32  Get pointer to drive parameter table (undocumented)
// 33  Get/set Ctrl-Break check state & get boot drive
// 34  Get address to DOS critical flag (undocumented)

// 35  Get interrupt vector
void* dos_get_interrupt_vector(uint8_t vec_num);

// 36  Get disk free space
// 37  Get/set switch character (undocumented)
// 38  Get/set country dependent information
// 39  Create subdirectory (mkdir)
// 3A  Remove subdirectory (rmdir)
// 3B  Change current subdirectory (chdir) 
// 3C  Create file using handle
// 3D  Open file using handle
// 3E  Close file using handle
// 3F  Read file or device using handle
// 40  Write file or device using handle
// 41  Delete file
// 42  Move file pointer using handle
// 43  Change file mode
// 44  I/O control for devices (IOCTL)
// 45  Duplicate file handle
// 46  Force duplicate file handle
// 47  Get current directory

// 48  Allocate memory blocks
uint16_t dos_allocate_memory_blocks(uint16_t paragraphs);

// 49  Free allocated memory blocks
uint16_t dos_free_allocated_memory_blocks(uint16_t segment);

// 4A  Modify allocated memory blocks
// 4B  EXEC load and execute program (func 1 undocumented)
// 4C  Terminate process with return code
// 4D  Get return code of a sub-process
// 4E  Find first matching file
// 4F  Find next matching file
// 50  Set current process id (undocumented)
// 51  Get current process id (undocumented)
// 52  Get pointer to DOS "INVARS" (undocumented)
// 53  Generate drive parameter table (undocumented)
// 54  Get verify setting
// 55  Create PSP (undocumented)
// 56  Rename file
// 57  Get/set file date and time using handle
// 58  Get/set memory allocation strategy (3.x+, undocumented)

#endif
