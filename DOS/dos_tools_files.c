/**
 *
 *  @file      dos_tools_files.c
 *  @brief
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      2.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#include "dos_tools_files.h"
#include "dos_services_files.h"

#include <string.h>

char* dos_tools_file_extension_ptr(char* file_path) {
	char* p = strrchr(file_path, '.');
	if (!p || p == file_path) {
		return 0;
	}
	else {
		return ++p;
	}
}

dos_file_size_t dos_tools_file_size(dos_file_handle_t fhandle) {
	dos_file_position_t fpos, fsize;
	fpos = fsize =0;
	dos_move_file_pointer_using_handle(fhandle, FSEEK_CUR, &fpos);	// get current position
	dos_move_file_pointer_using_handle(fhandle, FSEEK_END, &fsize);	// get size ie end position
	dos_move_file_pointer_using_handle(fhandle, FSEEK_SET, &fpos);		// restore file pointer
	return fsize;
}

dos_file_size_t dos_tools_file_dump(FILE* out_stream, dos_file_handle_t fhandle) {
	return 0;
}


