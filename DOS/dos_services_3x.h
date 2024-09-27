/**
 *
 *  @brief     DOS Services for DOS 3.x and above
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      30.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef DOS_SERVICE_3X_H
#define DOS_SERVICE_3X_H

#include "dos_services_3x_types.h"

namespace dos3x {

	// 59  Get extended error information (3.x+)
	void get_extended_error_information(extended_error_information_t* info);

	// 5A  Create temporary file (3.x+)
	// 5B  Create new file (3.x+)
	// 5C  Lock/unlock file access (3.x+)
	// 5D  Critical error information (undocumented 3.x+)
	// 5E  Network services (3.1+)
	// 5F  Network redirection (3.1+)
	// 60  Get fully qualified file name (undocumented 3.x+)
	// 62  Get address of program segment prefix (3.x+)
	// 63  Get system lead byte table (MSDOS 2.25 only)
	// 64  Set device driver look ahead  (undocumented 3.3+)
	// 65  Get extended country information (3.3+)
	// 66  Get/set global code page (3.3+)
	// 67  Set handle count (3.3+)
	// 68  Flush buffer (3.3+)
	// 69  Get/set disk serial number (undocumented DOS 4.0+)
	// 6A  DOS reserved (DOS 4.0+)
	// 6B  DOS reserved
	// 6C  Extended open/create (4.x+)
	// F8  Set OEM INT 21 handler (functions F9-FF) (undocumented)


}

#endif