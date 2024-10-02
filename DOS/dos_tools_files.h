/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      2.10.2024
 *  @copyright © Jeremy Thornton, 2024. All right reserved.
 *
 */
#ifndef DOS_TOOLS_FILE_H
#define DOS_TOOLS_FILE_H

#include "dos_services_files_types.h"

/**
*  @brief	Retrieve position of file extension (if there is one) in file path
*  @param	char* file_path, ptr to extant file path C string
*  @retval  char* position of start of extension in file_path
*/
char* dos_tools_file_extension_ptr(char* file_path);

/**
*   @brief Retrieve the file size of the file using its file name and path
*/
dos_file_size_t dos_tools_file_size(char* file_path);

/**
* @brief Retrieve the file size of the file using its file handle
*/
dos_file_size_t dos_tools_file_size(dos_file_handle_t fhandle);

/**
* @brief Skip over whitespace(s) where whitespace is defined in the null terminated string whitespaces
*/
dos_file_size_t dos_tools_file_skip_white_space(dos_file_handle_t fhandle, char* whitespaces);

dos_file_size_t dos_tools_file_skip_line(const dos_file_handle_t fhandle);

#endif
