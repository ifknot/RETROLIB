/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      30.10.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef DOS_SERVICES_FILES_TYPES_H
#define DOS_SERVICES_FILES_TYPES_H

#include <stdint.h>

namespace dos {

	namespace file {

		typedef uint16_t handle_t;

		typedef uint16_t attributes_t;

		typedef int32_t position_t;		// N.B. signed offset

		typedef uint32_t size_t;

		enum seek_attributes {
			FSEEK_SET,
			FSEEK_CUR,
			FSEEK_END
		};

		/**
		* Bitfields for create file attributes:
		*
		* 0	read-only
		* 1	hidden
		* 2	system
		* 3	volume label (ignored)
		* 4	reserved, must be zero (directory)
		* 5	archive bit
		* 7	if set, file is shareable under Novell NetWare
		* 
		* |5|4|3|2|1|0|  CX  valid file attributes
		*  | | | | | `---- 1 = read only			(DOSBOX ignored)
		*  | | | | `----- 1 = hidden				
		*  | | | `------ 1 = system			
		*  | `--------- not used for this call
		*  `--
		*/
		enum create_attributes {
			CREATE_READ_WRITE = 0,
			CREATE_READ_ONLY,		
			CREATE_HIDDEN,
			CREATE_SYSTEM = 4,
			CREATE_VOLUME = 8,
			CREATE_ARCHIVE = 32,
			CREATE_SHAREABLE = 128,
		};

		/**
		* Access modes in AL:
		* |7|6|5|4|3|2|1|0|  AL
		* | | | | | `-------- read/write/update access mode
		* | | | | `--------- reserved, always 0
		* | `-------------- sharing mode (see below) (DOS 3.1+)
		* `--------------- 1 = private, 0 = inheritable (DOS 3.1+)
		*
		* Sharing mode bits (DOS 3.1+):	       Access mode bits:
		* 654									 210
		* 000  compatibility mode (exclusive)    000  read access
		* 001  deny others read/write access     001  write access
		* 010  deny others write access			 010  read/write access
		* 011  deny others read access
		* 100  full access permitted to all
		*
		* - will open normal, hidden and system files
		* @note file pointer is placed at beginning of file
		*/
		enum access_attributes {
			ACCESS_READ_ONLY = 0,
			ACCESS_WRITE_ONLY,
			ACCESS_READ_WRITE,
			SHARE_EXCLUSIVE = 0,
			DENY_READ_WRITE = 16,
			DENY_WRITE = 32,
			DENY_READ = 48,
			SHARE_FULL = 64,
			PRIVATE = 128
		};

		/**
		* DOS int 21h, 36h	Get Disk Free Space
		*/
		struct disk_space_info_t {

			int16_t sectors_per_cluster;	// 0FFFFH if the drive number is invalid
			int16_t available_clusters;
			int16_t bytes_per_sector;
			int16_t clusters_per_drive;

		};

	}

}

#endif