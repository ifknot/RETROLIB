#ifndef FILE_EXTENSION_H
#define	FILE_EXTENSION_H

#include "file_types.h"
#include "../DOS/dos_services_files_types.h"

/**
*  @brief	Retrieve position of file extension (if there is one) in file path
*  @param	const char* file_path, ptr to extant file path C string
*  @retval  const char* position of start of extension in file_path
*/
const char* file_get_extension(const char* file_path);

file_size_t file_get_size(const dos_file_handle_t handle);

#endif
