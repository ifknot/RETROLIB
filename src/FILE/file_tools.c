#include "file_tools.h"

#include <string.h>

#include "file_constants.h"

#include "../DOS/dos_services_files.h"
#include "../DOS/dos_services_files_types.h"


const char* file_get_extension(const char* pfile_path) {
	const char* p = strrchr(pfile_path, FILE_EXTENSION_DELIM);
	if (!p || p == pfile_path) {
		return 0;
	}
	else {
		return ++p;
	}
}

file_size_t file_get_size(const dos_file_handle_t fhandle) {
   	dos_file_position_t fpos, fsize;
   	fpos = fsize =0;
   	fpos = dos_move_file_pointer(fhandle, 0, FSEEK_CUR);	// get current position
   	fsize = dos_move_file_pointer(fhandle, 0, FSEEK_END);	// get size ie end position
   	dos_move_file_pointer(fhandle, fpos, FSEEK_SET);		// restore file pointer
   	return fsize;
}
