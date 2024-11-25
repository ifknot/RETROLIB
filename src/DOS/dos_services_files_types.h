#ifndef DOS_SERVICES_FILES_TYPES_H
#define DOS_SERVICES_FILES_TYPES_H

#include <stdint.h>


typedef uint16_t dos_file_handle_t;

typedef uint16_t dos_file_attributes_t;

typedef int32_t dos_file_position_t;            // N.B. signed offset

typedef uint32_t dos_file_size_t;

typedef enum {
        FSEEK_SET,
        FSEEK_CUR,
        FSEEK_END
} dos_file_seek_attributes_t;

/**
* Bitfields for create file attributes:
*
* 0     read-only
* 1     hidden
* 2     system
* 3     volume label (ignored)
* 4     reserved, must be zero (directory)
* 5     archive bit
* 7     if set, file is shareable under Novell NetWare
*
* |5|4|3|2|1|0|  CX  valid file attributes
*  | | | | | `---- 1 = read only                        (DOSBOX ignored)
*  | | | | `----- 1 = hidden
*  | | | `------ 1 = system
*  | `--------- not used for this call
*  `--
*/
typedef enum {
        CREATE_READ_WRITE = 0,
        CREATE_READ_ONLY,
        CREATE_HIDDEN,
        CREATE_SYSTEM = 4,
        CREATE_VOLUME = 8,
        CREATE_ARCHIVE = 32,
        CREATE_SHAREABLE = 128
} dos_file_create_attributes_t;

/**
* Access modes in AL:
* |7|6|5|4|3|2|1|0|  AL
* | | | | | `-------- read/write/update access mode
* | | | | `--------- reserved, always 0
* | `-------------- sharing mode (see below) (DOS 3.1+)
* `--------------- 1 = private, 0 = inheritable (DOS 3.1+)
*
* Sharing mode bits (DOS 3.1+):        Access mode bits:
* 654                                                                    210
* 000  compatibility mode (exclusive)    000  read access
* 001  deny others read/write access     001  write access
* 010  deny others write access                  010  read/write access
* 011  deny others read access
* 100  full access permitted to all
*
* - will open normal, hidden and system files
* @note file pointer is placed at beginning of file
*/
typedef enum {
        ACCESS_READ_ONLY = 0,
        ACCESS_WRITE_ONLY,
        ACCESS_READ_WRITE,
        SHARE_EXCLUSIVE = 0,
        DENY_READ_WRITE = 16,
        DENY_WRITE = 32,
        DENY_READ = 48,
        SHARE_FULL = 64,
        PRIVATE = 128
} dos_file_access_attributes_t;

/**
* DOS int 21h, 36h      Get Disk Free Space
*/
typedef struct {

        int16_t sectors_per_cluster;    // 0FFFFH if the drive number is invalid
        int16_t available_clusters;
        int16_t bytes_per_sector;
        int16_t clusters_per_drive;

} dos_file_disk_space_info_t;

#endif
