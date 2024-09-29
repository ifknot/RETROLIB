/**
 *
 *  @file      dos_services_files.cpp
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      29.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#include "dos_services_files.h"
#include "dos_services_constants.h"
#include "dos_services_types.h"
#include "dos_error_messages.h"

namespace dos {


	/**
	* INT 21,36 - Get Disk Free Space
	* AH = 36h
	* DL = drive number (0=default, 1=A:)
	* 
	* on return:
	* AX = sectors per cluster
	*    = FFFF if drive is invalid
	* BX = number of available clusters
	* CX = number of bytes per sector
	* DX = number of clusters per drive
	* 
	* - used to determine available space on specified disk
	* - see	INT 21,1B   INT 21,1C
	*/
	void get_disk_free_space(uint8_t drive_number, file::disk_space_info_t* info) {
		__asm {
			.8086
			push	ds
			pushf

			mov		dl, drive_number
			mov		ah, GET_DISK_FREE_SPACE
			int		DOS_SERVICE
			cmp		ax, DOS_ERROR				; FFFFh invalid drive
			jne		OK

			cld
			les		di, info
			stosw								; store the error marker
			xor ax, ax
			stosw								; zero the rest of info struct
			stosw
			stosw
			jmp		END

	OK:		lds		di, info
			mov		[di], ax					; sectors per cluster
			mov		[di + 2], bx				; available clusters
			mov		[di + 4], cx				; bytes per sector
			mov		[di + 6], dx				; clusters per drive 

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (info->sectors_per_cluster == 0xFFFF) {
			std::cout << dos::error::messages[dos::error::INVALID_DRIVE_SPECIFIED] << "drive_number=" << drive_number << std::endl;
		}

#endif
	}

	/**
	* INT 21, 3C - Create File Using Handle
	* AH = 3C
	* CX = file attribute(see FILE ATTRIBUTES)
	* DS:DX = pointer to ASCIIZ path name
	* 
	* on return :
	* CF = 0 if successful
	*    = 1 if error
	* AX = files handle if successful
    *    = error code if failure(see DOS ERROR CODES)
	* 
	* @note - if file already exists, it is truncated to zero bytes on opening
	*/
	file::handle_t create_file_using_handle(const char * path_name, file::attributes_t create_attributes) {
		file::handle_t fhandle;
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			lds		dx, path_name
			mov		cx, create_attributes
			mov		ah, CREATE_FILE_USING_HANDLE
			int		DOS_SERVICE
			jnc		OK
			mov		err_code, ax
			xor		ax,ax
	OK:		mov		fhandle, ax

	END:	popf
			pop		ds
		}

#ifndef NDEBUG
		
		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_path=" << path_name << std::endl;
		}

#endif

		return fhandle;
	}

	/**
	* INT 21,3D Open File Using Handle
	* AH = 3D
	* AL = open access mode
	*      00  read only
	*      01  write only
	*      02  read/write
	* DS:DX = pointer to an ASCIIZ file name
	* 
	* on return:
	* AX = file handle if CF not set
	*    = error code if CF set  (see DOS ERROR CODES)
	*/
	file::handle_t open_file_using_handle(const char * path_name, uint8_t access_attributes) {
		file::handle_t fhandle;
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			lds		dx, path_name
			mov		al, access_attributes
			mov		ah, OPEN_FILE_USING_HANDLE
			int		DOS_SERVICE
			jnc		OK
			mov		err_code, ax
			xor		ax, ax
	OK:		mov		fhandle, ax

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_path=" << path_name << std::endl;
		}

#endif

		return fhandle;
	}

	/**
	* INT 21,3E - Close File Using Handle
	* AH = 3E
	* BX = file handle to close
	* 
	* on return:
	* AX = error code if CF set  (see DOS ERROR CODES)
	* 
	* - if file is opened for update, file time and date stamp as well as file size are updated in the directory
	* - handle is freed
	*/
	error_code_t close_file_using_handle(file::handle_t fhandle) {
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			mov		bx, fhandle
			mov		ah, CLOSE_FILE_USING_HANDLE
			int		DOS_SERVICE
			jnc		END
			mov		err_code, ax

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_handle=" << fhandle << std::endl;
		}

#endif

		return err_code;
	}

	/**
	* INT 21,3F - Read From File or Device Using Handle
	* AH = 3F
	* BX = file handle
	* CX = number of bytes to read
	* DS:DX = pointer to read buffer
	* 
	* on return:
	* AX = number of bytes read is CF not set
	*    = error code if CF set  (see DOS ERROR CODES)
	*
	* - read specified number of bytes from file into buffer DS:DX
	* - when AX is not equal to CX then a partial read occurred due to end of file
	* - if AX is zero, no data was read, and EOF occurred before read
	*/
	uint16_t read_file_using_handle(file::handle_t fhandle, char* buffer, uint16_t nbytes) {
		uint16_t bytes_read = 0;
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			lds		dx, buffer
			mov		cx,	nbytes
			mov		bx, fhandle
			mov		ah, READ_FILE_OR_DEVICE_USING_HANDLE
			int		DOS_SERVICE
			jnc		OK
			mov		err_code, ax
			jmp		END

	OK:		mov		bytes_read, ax

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_handle=" << fhandle << std::endl;
		}

#endif

		return bytes_read;
	}

	/**
	* INT 21,40 - Write To File or Device Using Handle
	* AH = 40h
	* BX = file handle
	* CX = number of bytes to write, a zero value truncates/extends the file to the current file position
	* DS:DX = pointer to write buffer
	*
	* on return:
	* AX = number of bytes written if CF not set
	*    = error code if CF set  (see DOS ERROR CODES)
	* 
	* - if AX is not equal to CX on return, a partial write occurred
	* - this function can be used to truncate a file to the current file position by writing zero bytes
	*/
	uint16_t write_file_using_handle(file::handle_t fhandle, char* buffer, uint16_t nbytes) {
		uint16_t bytes_written = 0;
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			lds		dx, buffer
			mov		cx, nbytes
			mov		bx, fhandle
			mov		ah, WRITE_FILE_OR_DEVICE_USING_HANDLE
			int		DOS_SERVICE
			jnc		OK
			mov		err_code, ax
			jmp		END

	OK:		mov		bytes_written, ax

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_handle=" << fhandle << std::endl;
		}

#endif

		return bytes_written;
	}

	/**
	* INT 21,41 - Delete File
	* AH = 41h
	* DS:DX = pointer to an ASCIIZ filename
	* 
	* on return:
	* AX = error code if CF set  (see DOS ERROR CODES)
	* 
	* - marks first byte of file directory entry with E5 to indicate the file has been deleted.  
	* - The rest of the directory entry stays intact until reused.   
	* - FAT pointers are returned to DOS
	* @note - documented as not accepting wildcards in filename but actually does in several DOS versions
	*/
	error_code_t delete_file(char* path_name) {
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			lds		dx, path_name
			mov		ah, DELETE_FILE
			int		DOS_SERVICE
			jnc		END
			mov		err_code, ax

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_path=" << path_name << std::endl;
		}

#endif

		return err_code;
	}

	/**
	* INT 21,42 - Move File Pointer Using Handle
	* AH = 42h
	* AL = origin of move:
	*      00 = beginning of file plus offset  (SEEK_SET)
	*      01 = current location plus offset	(SEEK_CUR)
	*      02 = end of file plus offset  (SEEK_END)
	* BX = file handle
	* CX:DX = (signed) offset from origin of new file position
	* CX = high order word of number of bytes to move
	* DX = low order word of number of bytes to move
	* 
	* on return:
	* CF clear if successful
	*     DX:AX = new file position in bytes from start of file
	*     DX = high order word of number of bytes to move
	*	  AX = low order word of number of bytes to move
	* CF set on error
	*    AX = error code
	* 
	* @note WARNING: for origins 01h and 02h, the pointer may be positioned before the
	* start of the file; no error is returned in that case (except under Windows NT), 
	* but subsequent attempts at I/O will produce errors.
	* If the new position is beyond the current end of file, the file will be extended by the next write! 
	* For FAT32 drives, the file must have been opened with AX=6C00h with the "extended size"
	* flag in order to expand the file beyond 2GB
	* 
	* @note BUG: using this method to grow a file from zero bytes to a very large size
	* can corrupt the FAT in some versions of DOS; the file should first
	* be grown from zero to one byte and then to the desired large size
	*/
	file::position_t move_file_pointer_using_handle(file::handle_t fhandle, uint8_t forigin, file::position_t fposition) {
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			lea		si, fposition						; DS:SI = address int32_t fposition
			mov		dx, [si]							; DX low order word of fposition
			mov		cx, [si + 2]						; CX hi order word of fposition
			mov		bx, fhandle
			mov		al, forigin							; SEEK_SET, SEEK_CUR, SEEK_END
			mov		ah, MOVE_FILE_POINTER_USING_HANDLE
			int		DOS_SERVICE
			jnc		OK
			mov		err_code, ax
			jmp		END

	OK:		lea		di, fposition						; DS:DI = address int32_t fposition
			mov		[di], ax							; low order word of fposition = AX
			mov		[di + 2], dx						; hi order word of fposition = DX

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_handle=" << fhandle << std::endl;
		}

#endif

		return fposition;
	}

	/**
	* INT 21,43 - Get/Set File Attributes
	* AH = 43h
	* AL = 00 to get attribute
	*    = 01 to set attribute
	* DS:DX = pointer to an ASCIIZ path name
	* CX = attribute to set
	*
	* |5|4|3|2|1|0|  CX  valid file attributes
	*  | | | | | `---- 1 = read only
	*  | | | | `----- 1 = hidden
	*  | | | `------ 1 = system
	*  | `--------- not used for this call
	*  `---------- 1 = archive
	*
	* on return:
	* AX = error code if CF set  (see DOS ERROR CODES)
	* CX = the attribute if AL was 00
	*/
	file::attributes_t get_file_attributes(char* path_name) {
		file::attributes_t attributes;
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			lds		dx, path_name
			xor		cx, cx
			xor		al, al						; AL = 00 to get attribute
			mov		ah, CHANGE_FILE_MODE
			int		DOS_SERVICE
			jnc		OK
			mov		err_code, ax
			xor		ax, ax
	OK:		mov		attributes, ax

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_path=" << path_name << std::endl;
		}

#endif

		return attributes;
	}

	/**
	* @note DOSBOX does not allow
	* @see file::attributes_t get_file_attributes(char* path_name)
	*/
	error_code_t set_file_attributes(char* path_name, file::attributes_t attributes) {
		error_code_t err_code = 0;
		__asm {
			.8086
			push	ds
			pushf

			lds		dx, path_name
			mov		cx, attributes
			mov		al, 1					; AL = 01 to set attribute
			mov		ah, CHANGE_FILE_MODE
			int		DOS_SERVICE
			jnc		END
			mov		err_code, ax

	END:	popf
			pop		ds
		}

#ifndef NDEBUG

		if (err_code) {
			std::cout << dos::error::messages[err_code] << "file_path=" << path_name << std::endl;
		}

#endif

		return err_code;
	}

}