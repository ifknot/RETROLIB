/*
* @brief  DOS Error Codes, Error Code Classes, Error Code Action Codes and Locus
 *
 * Of the following error codes, only error codes 1-12 are returned in AX upon exit from interrupt 21 or 24.
 * The rest are obtained by issuing the "get extended error" function call.
 * @see INT 21,59
*/
#ifndef DOS_ERROR_MESSAGES_H
#define DOS_ERROR_MESSAGES_H

#define MAX_ERR_STRING_LEN  64

/**
* Useful for user code to raise first classic 12h codes
*/
typedef enum {
    DOS_SUCCESS = 0,
    DOS_INVALID_FUNCTION_NUMBER,
    DOS_FILE_NOT_FOUND,
    DOS_PATH_NOT_FOUND,
    DOS_TOO_MANY_OPEN_FILES,
    DOS_ACCESS_DENIED,
    DOS_INVALID_HANDLE,
    DOS_MCB_DESTROYED,
    DOS_INSUFFICIENT_MEMORY,
    DOS_INVALID_MEMORY_BLOCK_ADDRESS,
    DOS_INVALID_ENVIRONMENT,
    DOS_INVALID_FORMAT,
    DOS_INVALID_ACCESS_MODE,
    DOS_INVALID_DATA,
    DOS_RESERVED,
    DOS_INVALID_DRIVE_SPECIFIED,
    DOS_ATTEMPT_TO_REMOVE_CURRENT_DIRECTORY,
    DOS_NOT_SAME_DEVICE,
    DOS_NO_MORE_FILES
} dos_error_codes;

/**
* @brief translate DOS error code to standard message.
*
* Of the following error codes, only error codes 1 - 12h are returned in AX upon exit from interrupt 21 or 24;
* The rest are obtained by issuing the "get extended error" function call
* @see INT 21, 59
*/
static const char dos_error_messages[85][MAX_ERR_STRING_LEN] = {
        "00  Success ",
        // DOS 2.0+
        "01  Invalid function number ",
        "02  File not found ",
        "03  Path not found ",
        "04  Too many open files (no handles left) ",
        "05  Access denied ",
        "06  Invalid handle ",
        "07  Memory control blocks destroyed ",
        "08  Insufficient memory ",
        "09  Invalid memory block address ",
        "0A  Invalid environment ",
        "0B  Invalid format ",
        "0C  Invalid access mode (open mode is invalid) ",
        "0D  Invalid data ",
        "0E  Reserved ",
        "0F  Invalid drive specified ",
        "10  Attempt to remove current directory ",
        "11  Not same device ",
        "12  No more files ",
        // DOS 3.0+ (INT 24 errors)
        "13  Attempt to write on a write-protected diskette ",
        "14  Unknown unit ",
        "15  Drive not ready ",
        "16  Unknown command ",
        "17  CRC error ",
        "18  Bad request structure length ",
        "19  Seek error ",
        "1A  Unknown media type ",
        "1B  Sector not found ",
        "1C  Printer out of paper ",
        "1D  Write fault ",
        "1E  Read fault ",
        "1F  General failure ",
        "20  Sharing violation ",
        "21  Lock violation ",
        "22  Invalid disk change ",      // ES:DI -> media ID structure
        /*
        * Format of media ID structure:
        * Offset        Size    Description     (Table 01681)
        * 00h 12 BYTEs  ASCIZ volume label of required disk
        * 0Ch   DWORD   serial number (DOS 4.0+)
        */
        "23  FCB unavailable ",
        "24  Sharing buffer overflow ",
        "25  Reserved ",
        "26  Unable to complete file operation ",
        "27  Reserved ",
        "28  Reserved ",
        "29  Reserved ",
        "30  Reserved ",
        "31  Reserved ",
        // OEM network errors (INT 24)
        "32  Network request not supported ",
        "33  Remote computer not listening ",
        "34  Duplicate name on network ",
        "35  Network name not found ",
        "36  Network busy ",
        "37  Network device no longer exists ",
        "38  NetBIOS command limit exceeded ",
        "39  Network adapter error ",
        "3A  Incorrect network response ",
        "3B  Unexpected network error ",
        "3C  Incompatible remote adapter ",
        "3D  Print queue full ",
        "3E  No space for print file ",
        "3F  Print file deleted ",
        "40  Network name deleted ",
        "41  Network access denied ",
        "42  Network device type incorrect ",
        "43  Network name not found ",
        "44  Network name limit exceeded ",
        "45  Network BIOS session limit exceeded ",
        "46  Temporarily paused ",
        "47  Network request not accepted ",
        "48  Print or disk redirection is paused ",
        /*
        49h (73)  network software not installed
        (LANtastic) invalid network version
        4Ah (74)  unexpected adapter close
        (LANtastic) account expired
        4Bh (75)  (LANtastic) password expired
        4Ch (76)  (LANtastic) login attempt invalid at this time
        4Dh (77)  (LANtastic v3+) disk limit exceeded on network node
        4Eh (78)  (LANtastic v3+) not logged in to network node
        4Fh (79)  reserved
        */
        "49  Reserved ",
        "4A  Reserved ",
        "4B  Reserved ",
        "4C  Reserved ",
        "4D  Reserved ",
        "4E  Reserved ",
        "4F  Reserved ",
        // end of errors reportable via INT 24
        "50  File already exists ",
        "51  Undocumented - duplicated FCB ",
        "52  Cannot make directory entry ",
        "53  Fail on INT 24 ",
        // network-related errors (non-INT 24) - DOS 3.3+
        "54  Too many redirections ",
        "55  Duplicate redirection ",
        "56  Invalid password ",
        "57  Invalid parameter ",
        "58  Network device fault ",
        // DOS 4.0 +
        "59  Function not supported by network ",
        "5A  Required system component not installed "
        /*
            5Bh (91)  (DOS 4.0+,NetWare4) timer server table overflowed
            5Ch (92)  (DOS 4.0+,NetWare4) duplicate in timer service table
            5Dh (93)  (DOS 4.0+,NetWare4) no items to work on
            5Fh (95)  (DOS 4.0+,NetWare4) interrupted / invalid system call
            64h (100) (MSCDEX) unknown error
            64h (100) (DOS 4.0+,NetWare4) open semaphore limit exceeded
            65h (101) (MSCDEX) not ready
            65h (101) (DOS 4.0+,NetWare4) exclusive semaphore is already owned
            66h (102) (MSCDEX) EMS memory no longer valid
            66h (102) (DOS 4.0+,NetWare4) semaphore was set when close attempted
            67h (103) (MSCDEX) not High Sierra or ISO-9660 format
            67h (103) (DOS 4.0+,NetWare4) too many exclusive semaphore requests
            68h (104) (MSCDEX) door open
            68h (104) (DOS 4.0+,NetWare4) operation invalid from interrupt handler
            69h (105) (DOS 4.0+,NetWare4) semaphore owner died
            6Ah (106) (DOS 4.0+,NetWare4) semaphore limit exceeded
            6Bh (107) (DOS 4.0+,NetWare4) insert drive B: disk into A: / disk changed
            6Ch (108) (DOS 4.0+,NetWare4) drive locked by another process
            6Dh (109) (DOS 4.0+,NetWare4) broken pipe
            6Eh (110) (DOS 5.0+,NetWare4) pipe open/create failed
            6Fh (111) (DOS 5.0+,NetWare4) pipe buffer overflowed
            70h (112) (DOS 5.0+,NetWare4) disk full
            71h (113) (DOS 5.0+,NetWare4) no more search handles
            72h (114) (DOS 5.0+,NetWare4) invalid target handle for dup2
            73h (115) (DOS 5.0+,NetWare4) bad user virtual address / protection violation
            74h (116) (DOS 5.0+) VIOKBD request
            74h (116) (NetWare4) error on console I/O
            75h (117) (DOS 5.0+,NetWare4) unknown category code for IOCTL
            76h (118) (DOS 5.0+,NetWare4) invalid value for verify flag
            77h (119) (DOS 5.0+,NetWare4) level four driver not found by DOS IOCTL
            78h (120) (DOS 5.0+,NetWare4) invalid / unimplemented function number
            79h (121) (DOS 5.0+,NetWare4) semaphore timeout
            7Ah (122) (DOS 5.0+,NetWare4) buffer too small to hold return data
            7Bh (123) (DOS 5.0+,NetWare4) invalid character or bad file-system name
            7Ch (124) (DOS 5.0+,NetWare4) unimplemented information level
            7Dh (125) (DOS 5.0+,NetWare4) no volume label found
            7Eh (126) (DOS 5.0+,NetWare4) module handle not found
            7Fh (127) (DOS 5.0+,NetWare4) procedure address not found
            80h (128) (DOS 5.0+,NetWare4) CWait found no children
            81h (129) (DOS 5.0+,NetWare4) CWait children still running
            82h (130) (DOS 5.0+,NetWare4) invalid operation for direct disk-access handle
            83h (131) (DOS 5.0+,NetWare4) attempted seek to negative offset
            84h (132) (DOS 5.0+,NetWare4) attempted to seek on device or pipe
        ---JOIN/SUBST errors---
            85h (133) (DOS 5.0+,NetWare4) drive already has JOINed drives
            86h (134) (DOS 5.0+,NetWare4) drive is already JOINed
            87h (135) (DOS 5.0+,NetWare4) drive is already SUBSTed
            88h (136) (DOS 5.0+,NetWare4) can not delete drive which is not JOINed
            89h (137) (DOS 5.0+,NetWare4) can not delete drive which is not SUBSTed
            8Ah (138) (DOS 5.0+,NetWare4) can not JOIN to a JOINed drive
            8Bh (139) (DOS 5.0+,NetWare4) can not SUBST to a SUBSTed drive
            8Ch (140) (DOS 5.0+,NetWare4) can not JOIN to a SUBSTed drive
            8Dh (141) (DOS 5.0+,NetWare4) can not SUBST to a JOINed drive
            8Eh (142) (DOS 5.0+,NetWare4) drive is busy
            8Fh (143) (DOS 5.0+,NetWare4) can not JOIN/SUBST to same drive
            90h (144) (DOS 5.0+,NetWare4) directory must not be root directory
            91h (145) (DOS 5.0+,NetWare4) can only JOIN to empty directory
            92h (146) (DOS 5.0+,NetWare4) path is already in use for SUBST
            93h (147) (DOS 5.0+,NetWare4) path is already in use for JOIN
            94h (148) (DOS 5.0+,NetWare4) path is in use by another process
            95h (149) (DOS 5.0+,NetWare4) directory previously SUBSTituted
            96h (150) (DOS 5.0+,NetWare4) system trace error
            97h (151) (DOS 5.0+,NetWare4) invalid event count for DosMuxSemWait
            98h (152) (DOS 5.0+,NetWare4) too many waiting on mutex
            99h (153) (DOS 5.0+,NetWare4) invalid list format
            9Ah (154) (DOS 5.0+,NetWare4) volume label too large
            9Bh (155) (DOS 5.0+,NetWare4) unable to create another TCB
            9Ch (156) (DOS 5.0+,NetWare4) signal refused
            9Dh (157) (DOS 5.0+,NetWare4) segment discarded
            9Eh (158) (DOS 5.0+,NetWare4) segment not locked
            9Fh (159) (DOS 5.0+,NetWare4) invalid thread-ID address
        -----
            A0h (160) (DOS 5.0+) bad arguments
            A0h (160) (NetWare4) bad environment pointer
            A1h (161) (DOS 5.0+,NetWare4) invalid pathname passed to EXEC
            A2h (162) (DOS 5.0+,NetWare4) signal already pending
            A3h (163) (DOS 5.0+) uncertain media
            A3h (163) (NetWare4) ERROR_124 mapping
            A4h (164) (DOS 5.0+) maximum number of threads reached
            A4h (164) (NetWare4) no more process slots
            A5h (165) (NetWare4) ERROR_124 mapping
            B0h (176) (MS-DOS 7.0) volume is not locked
            B1h (177) (MS-DOS 7.0) volume is locked in drive
            B2h (178) (MS-DOS 7.0) volume is not removable
            B4h (180) (MS-DOS 7.0) lock count has been exceeded
            B4h (180) (NetWare4) invalid segment number
            B5h (181) (MS-DOS 7.0) a valid eject request failed
            B5h (181) (DOS 5.0-6.0,NetWare4) invalid call gate
            B6h (182) (DOS 5.0+,NetWare4) invalid ordinal
            B7h (183) (DOS 5.0+,NetWare4) shared segment already exists
            B8h (184) (DOS 5.0+,NetWare4) no child process to wait for
            B9h (185) (DOS 5.0+,NetWare4) NoWait specified and child still running
            BAh (186) (DOS 5.0+,NetWare4) invalid flag number
            BBh (187) (DOS 5.0+,NetWare4) semaphore does not exist
            BCh (188) (DOS 5.0+,NetWare4) invalid starting code segment
            BDh (189) (DOS 5.0+,NetWare4) invalid stack segment
            BEh (190) (DOS 5.0+,NetWare4) invalid module type (DLL can not be used as application)
            BFh (191) (DOS 5.0+,NetWare4) invalid EXE signature
            C0h (192) (DOS 5.0+,NetWare4) EXE marked invalid
            C1h (193) (DOS 5.0+,NetWare4) bad EXE format (e.g. DOS-mode program)
            C2h (194) (DOS 5.0+,NetWare4) iterated data exceeds 64K
            C3h (195) (DOS 5.0+,NetWare4) invalid minimum allocation size
            C4h (196) (DOS 5.0+,NetWare4) dynamic link from invalid Ring
            C5h (197) (DOS 5.0+,NetWare4) IOPL not enabled
            C6h (198) (DOS 5.0+,NetWare4) invalid segment descriptor privilege level
            C7h (199) (DOS 5.0+,NetWare4) automatic data segment exceeds 64K
            C8h (200) (DOS 5.0+,NetWare4) Ring2 segment must be moveable
            C9h (201) (DOS 5.0+,NetWare4) relocation chain exceeds segment limit
            CAh (202) (DOS 5.0+,NetWare4) infinite loop in relocation chain
            CBh (203) (NetWare4) environment variable not found
            CCh (204) (NetWare4) not current country
            CDh (205) (NetWare4) no signal sent
            CEh (206) (NetWare4) file name not 8.3
            CFh (207) (NetWare4) Ring2 stack in use
            D0h (208) (NetWare4) meta expansion is too long
            D1h (209) (NetWare4) invalid signal number
            D2h (210) (NetWare4) inactive thread
            D3h (211) (NetWare4) file system information not available
            D4h (212) (NetWare4) locked error
            D5h (213) (NetWare4) attempted to execute non-family API call in DOS mode
            D6h (214) (NetWare4) too many modules
            D7h (215) (NetWare4) nesting not allowed
            E6h (230) (NetWare4) non-existent pipe, or bad operation
            E7h (231) (NetWare4) pipe is busy
            E8h (232) (NetWare4) no data available for nonblocking read
            E9h (233) (NetWare4) pipe disconnected by server
            EAh (234) (NetWare4) more data available
            FFh (255) (NetWare4) invalid drive
        */
};

// DOS 3.x+
static const char dos_error_classes[16][MAX_ERR_STRING_LEN] = {
        "00  no error class ",
        "01  out of resource (storage space or I / O channels) ",
        "02  temporary situation (file or record lock) ",
        "03  authorization / permission problem (denied access) ",
        "04  internal system error(system software bug) ",
        "05  hardware failure, serious problem related to hardware ",
        "06  system failure(configuration file missing or incorrect) ",
        "07  application error, inconsistent request ",
        "08  not found, file/item not found ",
        "09  bad format, file/item in invalid format ",
        "0A  locked, file/item interlocked ",
        "0B  media failure, ECC/CRC error, wrong or bad disk ",
        "0C  already exists, collision with existing item ",
        "0D  unknown, classification doesn't exist or is inappropriate ",
        "0E  cannot (undocumented) ",
        "0F  time (undocumented) "
};

// DOS 3.x+
static const char dos_error_actions[8][MAX_ERR_STRING_LEN] = {
        "00  no action ",
        "01  retry, attempt a few more times and re-prompt ",
        "02  delay retry, retry a few more times after a pause ",
        "03  re-enter input, prompt user to re-enter input ",
        "04  abort with cleanup, orderly abort and shutdown ",
        "05  immediate abort, exit immediately without cleanup ",
        "06  ignore error ",
        "07  user intervention, retry after user fixes the problem "
};

// DOS 3.x+
static const char dos_error_locus[6][MAX_ERR_STRING_LEN] = {
        "00  no locus ",
        "01  unknown or not appropriate ",
        "02  block device (disk error) ",
        "03  network related ",
        "04  serial device (timeout) ",
        "05  memory related "
};

#endif
