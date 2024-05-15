/*
    Copyright (C) 2017 eSOL Co.,Ltd. Tokyo, Japan

    This software is protected by the law and the agreement concerning
    a Japanese country copyright method, an international agreement,
    and other intellectual property right and may be used and copied
    only in accordance with the terms of such license and with the inclusion
    of the above copyright notice.

    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by eSOL Co.,Ltd.
*/
/****************************************************************************
[ ulfs.c ] - User-mode fs management: implementing interface for process
****************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mcos/uring.h>
#include <mcos/mcos_ds.h>
#include <limits.h>
//#include <ulfs.h>
#include "ulfs_impl.h"

#define SVCID_LFS_OPEN              (SVCID_LFS | 0x00)  /* open()               */
#define SVCID_LFS_CLOSE             (SVCID_LFS | 0x01)  /* close()              */
#define SVCID_LFS_READ              (SVCID_LFS | 0x02)  /* read()               */
#define SVCID_LFS_WRITE             (SVCID_LFS | 0x03)  /* write()              */
#define SVCID_LFS_UNLINK            (SVCID_LFS | 0x04)  /* unlink()             */
#define SVCID_LFS_LSEEK             (SVCID_LFS | 0x05)  /* lseek()              */
#define SVCID_LFS_ISATTY            (SVCID_LFS | 0x06)  /* isatty()             */
#define SVCID_LFS_FSTAT             (SVCID_LFS | 0x07)  /* fstat()              */
#define SVCID_LFS_FTRUNCATE         (SVCID_LFS | 0x08)  /* ftruncate()          */
#define SVCID_LFS_ACCESS            (SVCID_LFS | 0x09)  /* access()             */
#define SVCID_LFS_CHDIR             (SVCID_LFS | 0x0a)  /* chdir()              */
#define SVCID_LFS_CHMOD             (SVCID_LFS | 0x0b)  /* chmod()              */
#define SVCID_LFS_CHOWN             (SVCID_LFS | 0x0c)  /* chown()              */
#define SVCID_LFS_DUP               (SVCID_LFS | 0x0d)  /* dup()                */
#define SVCID_LFS_DUP2              (SVCID_LFS | 0x0e)  /* dup2()               */
#define SVCID_LFS_FCHDIR            (SVCID_LFS | 0x0f)  /* fchdir()             */
#define SVCID_LFS_FCHMOD            (SVCID_LFS | 0x10)  /* fchmod()             */
#define SVCID_LFS_FCHOWN            (SVCID_LFS | 0x11)  /* fchown()             */
#define SVCID_LFS_FCNTL             (SVCID_LFS | 0x12)  /* fcntl()              */
#define SVCID_LFS_FDATASYNC         (SVCID_LFS | 0x13)  /* fdatasync()          */
#define SVCID_LFS_FSTATVFS          (SVCID_LFS | 0x14)  /* fstatvfs()           */
#define SVCID_LFS_FSYNC             (SVCID_LFS | 0x15)  /* fsync()              */
#define SVCID_LFS_GETCWD            (SVCID_LFS | 0x16)  /* getcwd()             */
#define SVCID_LFS_IOCTL             (SVCID_LFS | 0x17)  /* ioctl()              */
#define SVCID_LFS_LINK              (SVCID_LFS | 0x18)  /* link()               */
#define SVCID_LFS_STAT              (SVCID_LFS | 0x19)  /* stat()               */
#define SVCID_LFS_STATVFS           (SVCID_LFS | 0x1a)  /* statvfs()            */
#define SVCID_LFS_TRUNCATE          (SVCID_LFS | 0x1b)  /* truncate()           */
#define SVCID_LFS_REALPATH          (SVCID_LFS | 0x1c)  /* realpath()           */
#define SVCID_LFS_UMASK             (SVCID_LFS | 0x1d)  /* umask()              */
#define SVCID_LFS_UTIME             (SVCID_LFS | 0x1e)  /* utime()              */
#define SVCID_LFS_PREAD             (SVCID_LFS | 0x1f)  /* pread()              */
#define SVCID_LFS_PWRITE            (SVCID_LFS | 0x20)  /* pwrite()             */
#define SVCID_LFS_READV             (SVCID_LFS | 0x21)  /* readv()              */
#define SVCID_LFS_WRITEV            (SVCID_LFS | 0x22)  /* writev()             */
#define SVCID_LFS_RMDIR             (SVCID_LFS | 0x23)  /* rmdir()              */
#define SVCID_LFS_SELECT            (SVCID_LFS | 0x24)  /* select()             */
#define SVCID_LFS_PSELECT           (SVCID_LFS | 0x25)  /* pselect()            */
#define SVCID_LFS_READDIR           (SVCID_LFS | 0x26)  /* __readdir()          */
#define SVCID_LFS_MKDIR             (SVCID_LFS | 0x27)  /* mkdir()              */
#define SVCID_LFS_RENAME            (SVCID_LFS | 0x28)  /* rename()             */
#define SVCID_LFS_SYNC              ((int32_t)((uint32_t)SVCID_LFS | 0x29U))    /* sync()               */
#define SVCID_LFS_POSIX_FALLOCATE   ((int32_t)((uint32_t)SVCID_LFS | 0x2aU))    /* posix_fallocate()    */
#define SVCID_LFS_DEVCTL            (SVCID_LFS | 0x2b)  /* devctl()             */
#define SVCID_LFS_UTIMENS           (SVCID_LFS | 0x2c)  /* utimensat()/utimes() */
#define SVCID_LFS_FUTIMENS          (SVCID_LFS | 0x2d)  /* futimens()/futimes() */
#define SVCID_LFS_MKFS              (SVCID_LFS | 0x2e)  /* __mkfs()  */
#define SVCID_LFS_RMFS              (SVCID_LFS | 0x2f)  /* __rmfs() */

#ifndef _in_asm_source_

/*
 * SVC Parameter Packet
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <utime.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <devctl.h>
//#include <ulfs.h>
/*
 *  Type Definitions
 */
typedef union lfs       lfs_t;              /* LFS request identifier       */

#include <sys/lfs_common.h>

#ifndef LFS_NAME_MAX
#define LFS_NAME_MAX _POSIX_NAME_MAX
#endif

#if MCOS_CFG_LFS_EXTENDED
/*
 * Permission check flags
 * X_OK R_OK W_OK F_OK are used from unistd.h
 */
#define D_OK            0x008               /* delete/rename required - check SVTX & owner */
#define S_OK            0x010               /* do not return ENOLIK if file is symlink     */
#if D_OK <= (F_OK | X_OK | R_OK | W_OK)
#error Please check D_OK
#endif

/* Public utility routine to be called by PFS to check access permissions.
   file_mode = X_OK | R_OK | W_OK | D_OK | S_OK and file type
   parent_mode is to check SVTX on file delete/rename.
   Do not return ENOLINK if S_OK and file_mode indicates a symlink.
   Returns zero or POSIX error code */
int32_t lfs_check_permission(int32_t req_acc, uid_t req_uid, gid_t req_gid, pid_t req_pid,
                             uid_t file_uid, gid_t file_gid, mode_t file_mode, mode_t parent_mode);

#endif /* MCOS_CFG_LFS_EXTENDED */

struct  lfs_generic
{
    int32_t         code;                   /* Command code                 */
    int32_t         pflags;                 /* PFS flags & index            */
};

/** PFS_OPEN
 * Open a file and return a corresponding file description handle.
 * @param in code   PFS_OPEN
 * @param in pflags Filesystem mount flags
 * @param in name   Full path to the file to be opened, starting from system root
 * @param in flags  Open flags for this open request (e.g. O_EXCL, O_CREAT)
 * @param in mode   Mode flags for this open request (e.g. 0666) only if flags includes O_CREAT)
 * @param in uid    User ID of the caller thread/process
 * @param in gid    Group ID of the caller thread/process
 * @param in pid    Process ID of the caller thread/process (to check supplementary groups)
 * @param out fid   File description handle
 * @retval 0        No Error
 * @retval !=0      a POSIX error code to be placed in errno
 */
struct  lfs_open
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
    int             flags;                  /* Open flags                   */
    mode_t          mode;                   /* Open mode                    */
    uid_t           uid;                    /* User ID of caller            */
    gid_t           gid;                    /* Group ID of caller           */
    pid_t           pid;                    /* Process ID of the caller     */
    fid_t*          fid;                    /* Pointer to return fid        */
};

/** PFS_CLOSE
 * Release a file description handle
 * @param in code   PFS_CLOSE
 * @param in pflags Filesystem mount flags
 * @param in fid    File description to release
 * @param in flags  Flags passed at open time for this handle
 * @retval 0        No Error
 * @retval !=0      a POSIX error code to be placed in errno
 */
struct  lfs_close
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int             flags;                  /* Open flags                   */
};

/** PFS_READ
 * Read data from a given file description at a given offset.
 * @param in code   PFS_READ
 * @param in pflags Filesystem mount flags
 * @param in fid    File description to read from
 * @param in flags  Flags passed at open time for this handle
 * @param out buf   Memory location where to place the data that was read
 * @param inout len Number of bytes to read, Number of bytes actually read
 * @param inout off Offset to start reading from, New file offset after reading
 * @param inout ret Same as off
 * @retval 0        No Error
 * @retval !=0      a POSIX error code to be placed in errno
 */
struct  lfs_read
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int             flags;                  /* Open flags                   */
    void*           buf;                    /* User buffer                  */
    size_t*         len;                    /* Read and return size ptr     */
    off_t*          off;                    /* File offset ptr              */
    off_t*          ret;                    /* Return offset ptr            */
};

/** PFS_WRITE
 * Write data from a given file description at a given offset.
 * @param in code   PFS_WRITE
 * @param in pflags Filesystem mount flags
 * @param in fid    File description to write to
 * @param in flags  Flags passed at open time for this handle
 * @param in buf    Memory location where to find the data to write
 * @param inout len Number of bytes to write, Number of bytes actually written
 * @param inout off Offset to start writing from, New file offset after writing
 * @param inout ret Same as off
 * @retval 0        No Error
 * @retval !=0      a POSIX error code to be placed in errno
 */
struct  lfs_write
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int             flags;                  /* Open flags                   */
    const void*     buf;                    /* User buffer                  */
    size_t*         len;                    /* Write and return size ptr    */
    off_t*          off;                    /* File offset ptr              */
    off_t*          ret;                    /* Return offset ptr            */
};

/** PFS_IOCTL
 * Do a filesystem-specific I/O Control operation, similar to posix_devctl/devctl/ioctl
 * @param in code   PFS_IOCTL
 * @param in pflags Filesystem mount flags
 * @param in fid    File description to process
 * @param in flags  Flags passed at open time for this handle
 * @param in dcmd   Command to process
 * @param inout data Data to process related to dcmd
 * @param in size   Number of bytes of data
 * @param out info  Additional information to return
 * @retval 0        No Error
 * @retval !=0      a POSIX error code to be placed in errno
 */
struct  lfs_ioctl
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int             flags;                  /* Open flags                   */
    int             dcmd;                   /* Command to device            */
    void*           data;                   /* Ptr to in/out data           */
    size_t          size;                   /* Data size                    */
    int*            info;                   /* Ptr to return device info    */
};

struct  lfs_devopen
{
    int             code;                   /* Command code                 */
    int             device;                 /* Device flags & unit          */
    int             flags;                  /* Open flags                   */
    int             nopen;                  /* Number of opens              */
};

struct  lfs_devclose
{
    int             code;                   /* Command code                 */
    int             device;                 /* Device flags & unit          */
    int             flags;                  /* Open flags                   */
    int             nopen;                  /* Number of opens              */
};

struct  lfs_devread
{
    int             code;                   /* Command code                 */
    int             device;                 /* Device flags & unit          */
    int             flags;                  /* Open flags                   */
    void*           buf;                    /* User buffer                  */
    int*            len;                    /* Read size and return size    */
    off_t*          off;                    /* File offset ptr              */
    off_t*          ret;                    /* Return offset ptr            */
};

struct  lfs_devwrite
{
    int             code;                   /* Command code                 */
    int             device;                 /* Device flags & unit          */
    int             flags;                  /* Open flags                   */
    const void*     buf;                    /* User buffer                  */
    int*            len;                    /* Write size and return size   */
    off_t*          off;                    /* File offset ptr              */
    off_t*          ret;                    /* Return offset ptr            */
};

struct  lfs_devctl
{
    int             code;                   /* Command code                 */
    int             device;                 /* Device flags & unit          */
    int             flags;                  /* Open flags                   */
    int             dcmd;                   /* Command to device            */
    void*           data;                   /* Ptr to in/out data           */
    size_t          size;                   /* Data size                    */
    int*            info;                   /* Ptr to return device info    */
};

struct  lfs_devgetdone
{
    int             code;                   /* Command code                 */
    int             type;                   /* Blocking/nonblocking         */
    lfs_t**         ptr;                    /* Ptr to return finished lfs_t */
};

 /* For eMCOS status field should be changed
    only at call with lfs_select.flags=0 */
struct  selreq
{
    struct srlist
    {
        struct srlist* next;
        struct srlist* prev;
    }               list;                   /* For use by PFS/dev for lists */
    unsigned char   status;                 /* Select status (r | w | e)    */
    unsigned char   request;                /* Select request (r | w | e)   */
    short           fildes;                 /* File descriptor              */
    unsigned int    thrid;                  /* Thread identifier to wakeup  */
};

struct  lfs_devselect
{
    int             code;                   /* Command code                 */
    int             device;                 /* Device flags & unit          */
    int             flags;                  /* Open flags                   */
    struct selreq*  selreq;                 /* Select request block         */
};

/** PFS_LINK
 * Create a hardlink from oldn to newn.
 * @param in code   PFS_LINK
 * @param in oldn   File to link
 * @param in newn   Location of new link to same file
 * @retval 0        No Error
 * @retval !=0      a POSIX error code to be placed in errno
 */
struct  lfs_link
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     oldn;                   /* Old file name                */
    const char*     newn;                   /* New file name                */
};

/** PFS_FSYNC
 * Flush metadata to disk
 * @param in code   PFS_FSYNC
 * @param in pflags Filesystem mount flags
 * @param in fid    File description to sync
 * @param in type   TYPE_FSYNC (fsync, metadata) or TYPE_FDATASYNC (fdatasync, data)
 * @param in flags  Flags passed at open time for this handle
 * @retval 0        No Error
 * @retval !=0      a POSIX error code to be placed in errno
 */
struct  lfs_fsync
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int             type;                   /* Fsync or fdatasync           */
    int             flags;                  /* Open flags                   */
};

/** lfs_fdsync - unused */
struct  lfs_fdsync
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
};

/** PFS_ACCESS
 * Check is file is accessible using the provided UNIX-style access mode
 * @param in code   PFS_ACCESS
 * @param in pflags Filesystem mount flags
 * @param in name   Path to check access for, starting from system root /
 * @param in amode  Access mode, Unix-style (0777). For MSDOS-style access R_OK/W_OK. For existence check F_OK.
 * @param in uid    User ID of the caller thread/process
 * @param in gid    Group ID of the caller thread/process
 * @param in pid    Process ID of the caller thread/process (to check supplementary groups)
 */
struct  lfs_access
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
    int             amode;                  /* Access mode                  */
    uid_t           uid;                    /* User ID of caller            */
    gid_t           gid;                    /* Group ID of caller           */
    pid_t           pid;                    /* Process ID of the caller     */
};

/** PFS_FSTAT
 * Provide information about file from file description
 * @param in code   PFS_FSTAT
 * @param in pflags Filesystem mount flags
 * @param in fid    File description to fstat from
 * @param out ptr   Pointer to stat structure to fill with information
 */
struct  lfs_fstat
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    struct stat*    ptr;                    /* Pointer to stat structure    */
};

/** PFS_STAT
 * Provide information about file from file name
 * @param in code   PFS_STAT
 * @param in pflags Filesystem mount flags
 * @param in name   File path starting from system root /
 * @param out ptr   Pointer to stat structure to fill with information
 */
struct  lfs_stat
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
    struct stat*    ptr;                    /* Pointer to stat structure    */
};

/** PFS_CHMOD
 * Modify access rights for a file from its name
 * @param in code   PFS_CHMOD
 * @param in pflags Filesystem mount flags
 * @param in name   File path starting from system root /
 * @param in mode   Requested new access rights
 */
struct  lfs_chmod
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
    mode_t          mode;                   /* File mode                    */
};

/** PFS_FCHMOD
 * Modify access rights for a file from a file description
 * @param in code   PFS_FCHMOD
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in mode   Requested new access rights
 */
struct  lfs_fchmod
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    mode_t          mode;                   /* File mode                    */
};

/** PFS_CHOWN
 * Modify owner of file from file path
 * @param in code   PFS_CHOWN
 * @param in pflags Filesystem mount flags
 * @param in name   File path starting from system root /
 * @param in owner  New uid requested as owning user
 * @param in group  New gid requested as owning group
 */
struct  lfs_chown
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
    uid_t           owner;                  /* Owner ID                     */
    gid_t           group;                  /* Group ID                     */
};

/** PFS_FCHOWN
 * Modify owner of file from file description
 * @param in code   PFS_FCHOWN
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in owner  New uid requested as owning user
 * @param in group  New gid requested as owning group
 */
struct  lfs_fchown
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    uid_t           owner;                  /* Owner ID                     */
    gid_t           group;                  /* Group ID                     */
};

/** PFS_UTIME
 * Modify access and modification time for file
 * @param in code   PFS_UTIME
 * @param in pflags Filesystem mount flags
 * @param in name   File path starting from system root /
 * @param in timb   Requested atime and mtime for file
 */
struct  lfs_utime
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
    const struct utimbuf* timb;             /* Pointer to utimbuf structure */
};

/** PFS_TRUNC
 * Truncate (or extend) file
 * @param in code   PFS_TRUNC
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in len    New size for file
 */
struct  lfs_trunc
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    off_t           len;                    /* Target length                */
};

/** PFS_UNLINK
 * Remove reference to file from file path
 * @param in code   PFS_UNLINK
 * @param in pflags Filesystem mount flags
 * @param in name   File path starting from system root /
 */
struct  lfs_unlink
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
};

/** PFS_RENAME
 * Rename file
 * @param in code   PFS_RENAME
 * @param in pflags Filesystem mount flags
 * @param in oldn   Current name to modify
 * @param in newn   New file name
 */
struct  lfs_rename
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     oldn;                   /* Old file name                */
    const char*     newn;                   /* New file name                */
};

/** PFS_RMDIR
 * Remove directory from name
 * @param in code   PFS_RMDIR
 * @param in pflags Filesystem mount flags
 * @param in name   File path starting from system root /
 */
struct  lfs_rmdir
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
};

/** PFS_MKDIR
 * Create directory at name
 * @param in code   PFS_MKDIR
 * @param in pflags Filesystem mount flags
 * @param in name   File path starting from system root /
 * @param in mode   Unix-style access mode
 */
struct  lfs_mkdir
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
    mode_t          mode;                   /* File mode                    */
};

/** PFS_CHDIR
 * Check directory is OK to make current directory for requesting process from path
 * @param in code   PFS_CHDIR
 * @param in pflags Filesystem mount flags
 * @param in name   File path starting from system root /
 * @retval 0        OK
 * @retval ENOTDIR  fid does not point to a directory
 * @retval EACCES   Unauthorized access
 */
struct  lfs_chdir
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
};

/** PFS_FCHDIR
 * Check directory is OK to make current directory for requesting process from file description
 * @param in code   PFS_FCHDIR
 * @param in pflags Filesystem mount flags
 * @param in fid    File description for directory
 * @param out buf   NULL-terminated path of fid
 * @param in len    Size of buf in bytes
 * @retval 0        OK
 * @retval ENOTDIR  fid does not point to a directory
 * @retval EACCES   Unauthorized access
 */
struct  lfs_fchdir
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    char*           buf;                    /* Name buffer                  */
    int             len;                    /* Buffer size                  */
};

/** PFS_READDIR
 * Enumerate files/directories in directory
 * @param in code   PFS_READDIR
 * @param in pflags Filesystem mount flags
 * @param in fid    File description for directory
 * @param in flags  Flags of open file description
 * @param out ptr   Directory entries to fill
 * @param inout len Size of ptr buffer in bytes at entry, Effective read in bytes at exit
 * @param inout off New directory/file offset before and after request
 * @param out ret   Same as off
 * @retval 0        Some entries were provided if len > 0, none if len == 0
 * @retval EBADF    Not a directory
 */
struct  lfs_readdir
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int             flags;                  /* Open flags                   */
    struct dirent*  ptr;                    /* Directory entry              */
    size_t*         len;                    /* Read and return size ptr     */
    off_t*          off;                    /* File offset ptr              */
    off_t*          ret;                    /* Return offset ptr            */
};

/** PFS_STATVFS
 * Get statistics from filesystem using file path
 * @param in code   PFS_STATVFS
 * @param in pflags Filesystem mount flags
 * @param in name   Valid file path within filesystem, using full path from system root /
 * @param out ptr   statvfs structure to fill
 */
struct  lfs_statvfs
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    const char*     name;                   /* File name                    */
    struct statvfs* ptr;                    /* Pointer to statvfs structure */
};

/** PFS_FSTATVFS
 * Get statistics from filesystem using file description
 * @param in code   PFS_FSTATVFS
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param out ptr   statvfs structure to fill
 */
struct  lfs_fstatvfs
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    struct statvfs* ptr;                    /* Pointer to statvfs structure */
};

/** PFS_SELECT
 * Wait for a notification on a file description change.
 * If flags is 0, then cancel waiting on fid.
 * else setup wait on selreq.request events (LFS_SELECT_*)
 * This call is typically done two times:
 * - one time to setup the wait. Returning 0 means that wait is accepted.
 * - another time to complete or cancel the wait (with flags as 0)
 * @param in code   PFS_SELECT
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in flags  Open flags for the file, or 0 if cancel request
 * @param inout selreq Wait request structure
 * @retval 0        Request successful
 * @retval ENOSYS   Unsupported request for this filesystem
 * @retval EBADF    Not a valid file description
 */
struct  lfs_select
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int             flags;                  /* Open flags                   */
    struct selreq*  selreq;                 /* Select request block         */
};

/** PFS_GETDONE
 * Complete a pending request.
 * When a previous call returned POSIX_EIOINCOMPLETE, the thread will become pending
 * until the request completes. When the thread wakes up, it will call this request
 * to complete the transfer.
 * @param in code   PFS_GETDONE
 * @param in type   GETDONE_BLOCK or GETDONE_NONBLOCK
 * @param inout ptr LFS request to be completed
 */
struct  lfs_getdone
{
    int             code;                   /* Command code                 */
    int             type;                   /* Blocking/nonblocking         */
    lfs_t**         ptr;                    /* Ptr to return finished lfs_t */
};

#if (MCOS_CFG_GLB_HWCL_MAX > 1)
struct  lfs_findsrv
{
    int             code;                   /* Command code                 */
    const char*     name;                   /* File name                    */
    pfs_t*          pfsid;                  /* To return PFS ident          */
    int*            pflags;                 /* To return PFS flags          */
    dev_t*          pfile;                  /* To return DEV ident or 0     */
    int*            dflags;                 /* To return DEV flags or 0     */
    int*            srvid;                  /* Server id LFS/PFS/DEV        */
};
#endif /* (MCOS_CFG_GLB_HWCL_MAX > 1) */

/** PFS_SYNC
 * Flush filesystem data
 * @param in code   PFS_SYNC
 * @param in pflags Filesystem mount flags
 */
struct  lfs_sync
{
    int32_t         code;                   /* Command code                 */
    int32_t         pflags;                 /* PFS flags & index            */
};

/** PFS_POSIX_FALLOCATE
 * Reserve space for file: ensure that at least len bytes of storage are guaranteed
 * to be available starting at offset off.
 * @param in code   PFS_POSIX_FALLOCATE
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in flags  File open flags
 * @param in off    Starting point to consider requested length
 * @param in len    Requested length starting from off
 */
struct  lfs_posix_fallocate
{
    int32_t         code;                   /* Command code                 */
    int32_t         pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int32_t         flags;                  /* Open flags                   */
    off_t           off;                    /* Offset                       */
    off_t           len;                    /* length                       */
};

/** PFS_MMAP
 * Request authorization for making memory map of file.
 * Filesystem may correct/restrict the access rights in prot, and may propose
 * an existing filesystem-local memory range to use as physical backing memory
 * for mapping in user buffer.
 * @param in code   PFS_MMAP
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in vaddr  User process destination address
 * @param in flags  mmap flags (MAP_*)
 * @param in off    offset in file to map
 * @param in len    length of requested mapping
 * @param inout prot requested mapping rights (PROT_*), allowed mapping rights upon return
 * @param out addr  Filesystem-local proposed address for memory location of mapping
 */
struct  lfs_mmap
{
    int32_t         code;                   /* Command code                 */
    int32_t         pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    void           *vaddr;                  /* user process destination address */
    int32_t         flags;                  /* mmap flags */
    off_t           off;                    /* file/device offset */
    size_t          len;                    /* requested mmap length */
    int32_t         prot /* INOUT */;       /* mmap prot */
    void           *addr /* OUT */;         /* in-FS address, usually kernel address */
};

/** PFS_MUNMAP
 * Notify unmap of memory range to filesystem.
 * After this call, the range is no longer used in the context of this fid and backing
 * memory may be freed.
 * @param in code   PFS_MUNMAP
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in off    Start offset to be unmapped in file
 * @param in len    Length to be unmapped starting at off
 */
struct  lfs_munmap
{
    int32_t         code;                   /* Command code                 */
    int32_t         pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    off_t           off;                    /* file/device offset           */
    size_t          len;                    /* requested munmap length      */
};

/** PFS_MSYNC
 * Request flushing memory changes to disk.
 * After this call, all modifications in memory from the range shall be visible to
 * other readers. Only called in case of MAP_SHARED mappings.
 * @param in code   PFS_MSYNC
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in off    Start offset to flush
 * @param in flags  msync() option flags
 * @param in len    Length of range to flush starting at off
 */
struct  lfs_msync
{
    int32_t         code;                   /* Command code                 */
    int32_t         pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    off_t           off;                    /* file/device offset           */
    int32_t         flags;                  /* msync flags                  */
    size_t          len;                    /* length to sync               */
};

/** PFD_DUP
 * Increase reference counting of file description.
 * This may be because of explicit dup*() calls, or fork()/posix_spawn(), or mmap().
 * @param in code   PFS_DUP
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in flags  File open flags
 */
struct  lfs_dup
{
    int32_t         code;                   /* Command code                 */
    int32_t         pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    int32_t         flags;                  /* original flags               */
};

/** PFS_SEEK
 * Change current file description offset for reading and writing.
 * To allow correct handling inside lfs_fdesc_req_start/finish, off and st must
 * be located after the end of an equivalent struct lfs_fstat.
 * @param in code   PFS_SEEK
 * @param in pflags Filesystem mount flags
 * @param in fid    File description
 * @param in whence Seek action (SEEK_SET, SEEK_CUR, SEEK_END)
 * @param inout off Request offset at entry, New offset at exit
 */
struct  lfs_seek
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           fid;                    /* PFS file ID                  */
    uintptr_t       whence;                 /* meaning of offset            */
    off_t*          off /* INOUT */;        /* file/device offset           */
    struct stat     st;                     /* used internally              */
};

struct  lfs_utimens
{
    int                     code;           /* Command code                             */
    int                     pflags;         /* PFS flags & index                        */
    const char *            name;           /* File name                                */
    const struct timespec * atime;          /* Pointer to atime (null means UTIME_OMIT) */
    const struct timespec * mtime;          /* Pointer to atime (null means UTIME_OMIT) */
};

struct  lfs_futimens
{
    int                     code;           /* Command code                             */
    int                     pflags;         /* PFS flags & index                        */
    fid_t                   fid;            /* PFS file ID                              */
    const struct timespec * atime;          /* Pointer to atime (null means UTIME_OMIT) */
    const struct timespec * mtime;          /* Pointer to atime (null means UTIME_OMIT) */
};

struct  lfs_filepath
{
    int                     code;           /* Command code                             */
    int                     pflags;         /* PFS flags & index                        */
    fid_t                   fid;            /* PFS file ID                              */
    char *                  buf;            /* Buffer storing a file path               */
    size_t *                buflen;         /* Buffer size and length of the file path
                                             * containing a null terminator.            */
#if MCOS_CFG_LFS_EXTENDED
    uid_t                   uid;            /* User ID of caller                        */
    uid_t                   gid;            /* Group ID of caller                       */
    pid_t                   pid;            /* Process ID of caller                     */
    int                     flags;          /* O_DIRECTORY | O_SEARCH allowed
                                               uid/gid needed ony if both flags given   */
#endif
};

#if MCOS_CFG_LFS_EXTENDED

/** PFS_OPENEX
 * Open a file and return a corresponding file description handle.
 * As a special flag in mode, S_IFLNK may be specified to create a symbolic link.
 * O_SEARCH and O_CREAT require PFS to open the last component symlink as if by O_RDONLY
 * (this is to support operations on symlink itself). S_IFLNK is required here as well.
 * If relpath==NULL, and reffid!=0 EINVAL shall be returned.
 * If relpath==NULL and reffid==0 then PFS root directory shall be opened
 * If reffid points to a file ENOTDIR shall be returned.
 * If reffid points to a directory, and relpath is not NULL, then file may be
 *  opened or created at the path name relative to the path of directory reffid.
 * Note: ENOLINK is returned if component is symlink unless it is last and O_SEARCH|O_CREAT are set.
 *  If ENOLINK and symlink is last component file stat is returned via ptr.
 * @param in code   PFS_OPEN
 * @param in pflags Filesystem mount flags
 * @param in reffid File description to a directory used as reference, e.g. this FS root directory
 * @param in name   Path to the file to be opened, relative to reffid or absolute
 * @param in flags  Open flags for this open request (e.g. O_EXCL, O_CREAT)
 * @param in mode   Mode flags for this open request (e.g. 0666) only if flags includes O_CREAT)
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param out ptr   To return file stat (may be NULL)
 * @param out fid   File description handle
 * @retval 0        No Error
 * @retval !=0      a POSIX error code to be placed in errno
 */
struct  lfs_openex
{
    int             code;                   /* Command code                 */
    int             pflags;                 /* PFS flags & index            */
    fid_t           reffid;                 /* PFS directory ID             */
    const char*     relpath;                /* Relative file pathname       */
    int             flags;                  /* Open flags                   */
    mode_t          mode;                   /* Open mode & file type        */
    uid_t           uid;                    /* User ID of caller            */
    gid_t           gid;                    /* Group ID of caller           */
    pid_t           pid;                    /* Process ID of caller         */
    struct stat*    ptr;                    /* Pointer to return file stat  */
    fid_t*          fid;                    /* Pointer to return fid        */
};

/** PFS_FSTATEX
 * Provide information about path relative to file description.
 * If relpath is NULL, then action is done on reffid directly.
 * If reffid points to a file, and relpath is non-NULL, ENOTDIR shall be returned.
 * If reffid points to a directory, and relpath is not NULL,
 *  then action shall be done on the file/directory relative to the reffid directory.
 * To realize the same action as previous PFS_STAT, first open the file/directory.
 * To realize the same action as previous PFS_FSTAT, use NULL path.
 * @param in code   PFS_FSTAT
 * @param in pflags Filesystem mount flags
 * @param in reffid File description to fstat from
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param in relpath Path to the file/directory relative to reffid
 * @param out ptr   Pointer to stat structure to fill with information
 */
struct lfs_fstatex
{
    int             code;                   /* Command code                  */
    int             pflags;                 /* PFS flags & index             */
    fid_t           reffid;                 /* PFS directory ID              */
    uid_t           uid;                    /* Caller uid                    */
    gid_t           gid;                    /* Caller gid                    */
    pid_t           pid;                    /* Caller pid                   */
    const char*     relpath;                /* Relative pathname             */
    struct stat*    ptr;                    /* Pointer to stat structure     */
};

/** PFS_MKDIREX
 * Creeate a directory
 * If relpath is NULL, then EINVAL shall be returned
 * If reffid points to a file, and relpath is non-NULL, ENOTDIR shall be returned.
 * @param in code   PFS_MKDIREX
 * @param in pflags Filesystem mount flags
 * @param in reffid File description to mkdir from
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param in mode   Mode flags to set for the directory
 * @param in relpath Path to the directory relative to reffid
 */
struct  lfs_mkdirex
{
    int             code;                   /* Command code                  */
    int             pflags;                 /* PFS flags & index             */
    fid_t           reffid;                 /* PFS directory ID              */
    uid_t           uid;                    /* Caller uid                    */
    gid_t           gid;                    /* Caller gid                    */
    pid_t           pid;                    /* Caller pid                   */
    mode_t          mode;                   /* Directory mode                */
    const char*     relpath;                /* Relative pathname             */
};

/** PFS_UNLINKEX
 * Remove a file or directory
 * If relpath is NULL, then EINVAL shall be returned
 * If reffid points to a file, and relpath is non-NULL, ENOTDIR shall be returned.
 * @param in code   PFS_UNLINKEX
 * @param in pflags Filesystem mount flags
 * @param in reffid File description to mkdir from
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param in flags  O_DIRECTORY or 0
 * @param in relpath Path to the file/directory relative to reffid
 */
struct  lfs_unlinkex
{
    int             code;                   /* Command code                  */
    int             pflags;                 /* PFS flags & index             */
    fid_t           reffid;                 /* PFS directory ID              */
    uid_t           uid;                    /* Caller uid                    */
    gid_t           gid;                    /* Caller gid                    */
    pid_t           pid;                    /* Caller pid                   */
    int             flags;                  /* O_DIRECTORY or 0              */
    const char*     relpath;                /* Relative pathname             */
};

/** PFS_CHMODEX
 * Change file mode
 * If relpath is NULL, then action is done on reffid directly.
 * If reffid points to a file, and relpath is non-NULL, ENOTDIR shall be returned.
 * If reffid points to a directory, and relpath is not NULL,
 *  then action shall be done on the file/directory relative to the reffid directory.
 * @param in code   PFS_CHMODEX
 * @param in pflags Filesystem mount flags
 * @param in reffid File description to chmod from
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param in mode   Mode flags to set for the file
 * @param in relpath Path to the file/directory relative to reffid
 */
struct  lfs_chmodex
{
    int             code;                   /* Command code                  */
    int             pflags;                 /* PFS flags & index             */
    fid_t           reffid;                 /* PFS directory ID              */
    uid_t           uid;                    /* Caller uid                    */
    gid_t           gid;                    /* Caller gid                    */
    pid_t           pid;                    /* Caller pid                   */
    mode_t          mode;                   /* File mode                     */
    const char*     relpath;                /* Relative pathname             */
};

/** PFS_CHOWNEX
 * Change file owner/group
 * If relpath is NULL, then action is done on reffid directly.
 * If reffid points to a file, and relpath is non-NULL, ENOTDIR shall be returned.
 * If reffid points to a directory, and relpath is not NULL,
 *  then action shall be done on the file/directory relative to the reffid directory.
 * @param in code   PFS_CHOWNEX
 * @param in pflags Filesystem mount flags
 * @param in reffid File description to chown from
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param in owner  User id to change file to (-1 ignore)
 * @param in group  Group id to change file to (-1 ignore)
 * @param in relpath Path to the file/directory relative to reffid
 */
struct  lfs_chownex
{
    int             code;                   /* Command code                  */
    int             pflags;                 /* PFS flags & index             */
    fid_t           reffid;                 /* PFS directory ID              */
    uid_t           uid;                    /* Caller uid                    */
    gid_t           gid;                    /* Caller gid                    */
    pid_t           pid;                    /* Caller pid                   */
    uid_t           owner;                  /* Owner to change to            */
    uid_t           group;                  /* Group to change to            */
    const char*     relpath;                /* Relative pathname             */
};

/** PFS_LINKEX
 * Make hardlink to a file
 * If orelpath is NULL, then EINVAL shall be returned
 * If nrelpath is NULL, then EINVAL shall be returned
 * If oreffid points to a file, and orelpath is non-NULL, ENOTDIR shall be returned.
 * If nreffid points to a file, and nrelpath is non-NULL, ENOTDIR shall be returned.
 * @param in code   PFS_LINKEX
 * @param in pflags Filesystem mount flags
 * @param in oreffid File description to link from
 * @param in nreffid File description to link to
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param in orelpath Path to the file relative to oreffid
 * @param in nrelpath Path to the file relative to nreffid
 */
struct  lfs_linkex
{
    int             code;                   /* Command code                  */
    int             pflags;                 /* PFS flags & index             */
    fid_t           oreffid;                /* PFS old directory ID          */
    fid_t           nreffid;                /* PFS nre directory ID          */
    uid_t           uid;                    /* Caller uid                    */
    gid_t           gid;                    /* Caller gid                    */
    pid_t           pid;                    /* Caller pid                   */
    const char*     orelpath;               /* Relative old pathname         */
    const char*     nrelpath;               /* Relative new pathname         */
};

/** PFS_RENAMEEX
 * Rename file/directory
 * If orelpath is NULL, then EINVAL shall be returned
 * If nrelpath is NULL, then EINVAL shall be returned
 * If oreffid points to a file, and orelpath is non-NULL, ENOTDIR shall be returned.
 * If nreffid points to a file, and nrelpath is non-NULL, ENOTDIR shall be returned.
 * @param in code   PFS_LINKEX
 * @param in pflags Filesystem mount flags
 * @param in oreffid File description to rename from
 * @param in nreffid File description to rename to
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param in orelpath Path to the file relative to oreffid
 * @param in nrelpath Path to the file relative to nreffid
 */
struct  lfs_renameex
{
    int             code;                   /* Command code                  */
    int             pflags;                 /* PFS flags & index             */
    fid_t           oreffid;                /* PFS old directory ID          */
    fid_t           nreffid;                /* PFS nre directory ID          */
    uid_t           uid;                    /* Caller uid                    */
    gid_t           gid;                    /* Caller gid                    */
    pid_t           pid;                    /* Caller pid                   */
    const char*     orelpath;               /* Relative old pathname         */
    const char*     nrelpath;               /* Relative new pathname         */
};

/** PFS_UTIMENSEX
 * Change file times
 * If relpath is NULL, then action is done on reffid directly.
 * If reffid points to a file, and relpath is non-NULL, ENOTDIR shall be returned.
 * If reffid points to a directory, and relpath is not NULL,
 *  then action shall be done on the file/directory relative to the reffid directory.
 * @param in code   PFS_UTIMENSEX
 * @param in pflags Filesystem mount flags
 * @param in reffid File description to chown from
 * @param in uid    User id of caller
 * @param in gid    Group id of caller
 * @param in pid    Process id of caller
 * @param in relpath Path to the file/directory relative to reffid
 * @param in atime  Pointer to atime or NULL
 * @param in atime  Pointer to atime or NULL
 */
struct  lfs_utimensex
{
    int             code;                   /* Command code                  */
    int             pflags;                 /* PFS flags & index             */
    fid_t           reffid;                 /* PFS directory ID              */
    uid_t           uid;                    /* Caller uid                    */
    gid_t           gid;                    /* Caller gid                    */
    pid_t           pid;                    /* Caller pid                   */
    const char*     relpath;                /* Relative pathname             */
    const struct timespec * atime;          /* Pointer to atime (null means UTIME_OMIT) */
    const struct timespec * mtime;          /* Pointer to atime (null means UTIME_OMIT) */
};

#endif /* MCOS_CFG_LFS_EXTENDED */

/*
 *  Request union
 */
union   lfs
{
    int                         r_code;
    struct  lfs_generic         r_gen;
    struct  lfs_open            r_open;
    struct  lfs_close           r_close;
    struct  lfs_read            r_read;
    struct  lfs_write           r_write;
    struct  lfs_ioctl           r_ioctl;
    struct  lfs_link            r_link;
    struct  lfs_fsync           r_fsync;
    struct  lfs_access          r_access;
    struct  lfs_fstat           r_fstat;
    struct  lfs_stat            r_stat;
    struct  lfs_chmod           r_chmod;
    struct  lfs_fchmod          r_fchmod;
    struct  lfs_chown           r_chown;
    struct  lfs_fchown          r_fchown;
    struct  lfs_utime           r_utime;
    struct  lfs_trunc           r_trunc;
    struct  lfs_unlink          r_unlink;
    struct  lfs_rename          r_rename;
    struct  lfs_rmdir           r_rmdir;
    struct  lfs_mkdir           r_mkdir;
    struct  lfs_chdir           r_chdir;
    struct  lfs_fchdir          r_fchdir;
    struct  lfs_readdir         r_readdir;
    struct  lfs_statvfs         r_statvfs;
    struct  lfs_fstatvfs        r_fstatvfs;
    struct  lfs_select          r_select;
    struct  lfs_devopen         r_devopen;
    struct  lfs_devclose        r_devclose;
    struct  lfs_devread         r_devread;
    struct  lfs_devwrite        r_devwrite;
    struct  lfs_devctl          r_devctl;
    struct  lfs_devselect       r_devselect;
    struct  lfs_getdone         r_getdone;
    struct  lfs_devgetdone      r_devgetdone;
#if MCOS_CFG_GLB_HWCL_MAX > 1
    struct  lfs_findsrv         r_findsrv;
#endif
    struct  lfs_sync            r_sync;
    struct  lfs_posix_fallocate r_posix_fallocate;
    struct  lfs_mmap            r_mmap;
    struct  lfs_munmap          r_munmap;
    struct  lfs_msync           r_msync;
    struct  lfs_dup             r_dup;
    struct  lfs_seek            r_seek;
    struct  lfs_utimens         r_utimens;
    struct  lfs_futimens        r_futimens;
    struct  lfs_filepath        r_filepath;
#if MCOS_CFG_LFS_EXTENDED
    struct  lfs_openex          r_openex;
    struct  lfs_fstatex         r_fstatex;
    struct  lfs_mkdirex         r_mkdirex;
    struct  lfs_unlinkex        r_unlinkex;
    struct  lfs_chmodex         r_chmodex;
    struct  lfs_chownex         r_chownex;
    struct  lfs_linkex          r_linkex;
    struct  lfs_renameex        r_renameex;
    struct  lfs_utimensex       r_utimensex;
#endif
};

/*
 *  LFS extended information (OS dependent feature)
 */
/* For supporting abort feature */
struct lfs_abort
{
    void    (*brkfn)(int);      /* Break function       */
    void    (*stafn)(int);      /* Startup function     */
    void    (*clnfn)(int);      /* Cleanup function     */
    int     prio;               /* Priority of PFS(DEV) */
};
typedef struct lfs_abort lfs_abort_t;

typedef int32_t (*lfs_cmd_func_t)(lfs_t *req);
typedef int32_t (*lfs_dev_func_t)(lfs_t *req, uintptr_t ctx);

#include <stdint.h>
/* Common message processing routines. Return error code.
   Application PFS and DEV server threads should call corresponding
   routine and terminate when it returns (DEV/PFS removed) */
int lfs_pfs_csm_entry(uint32_t bufs, int (*pfsfunc)(lfs_t* req));
int lfs_dev_csm_entry(uint32_t bufs, int (*devfunc)(lfs_t* req));
/* Report PFS/DEV request done to driver thread.
   Note: to be called from driver/server thread context */
void lfs_pfs_reqdone(void);
void lfs_dev_reqdone(void);
int dev_devreqdone_msgproc(int ret, lfs_t* req, void *info);
typedef struct lfs_dev_reqdone_info {
    uintptr_t param;
    void* ptr;
} lfs_dev_reqdone_info_t;

/* file description to be used inside PFS service */
typedef struct lfs_fdesc {
    off_t     offset;     /* file offset, updated after read/write/lseek */
    uintptr_t fid;        /* PFS-internal identifier */
    uint16_t  refc;       /* refcount */
    uint8_t   paction;    /* if current request is pread or pwrite, 1 */
} lfs_fdesc_t;

typedef struct lfs_fdeschead {
    lfs_fdesc_t *head;
    struct lfs_fdeschead *next;
    uint32_t blocksize;   /* If 0, the code will assume LFS_FDESC_BLOCKSIZE_DEFAULT */
} lfs_fdeschead_t;

/* Allocate a new fdesc */
lfs_fdesc_t *lfs_fdesc_alloc(lfs_fdeschead_t *fhead, uint32_t *idx);
/* Find a fdesc from an arraylist */
lfs_fdesc_t *lfs_fdesc_get_desc(lfs_fdeschead_t *fhead, uint32_t idx);
/* Find a fdesc by fid */
int32_t lfs_fdesc_find_by_fid(lfs_fdeschead_t *fhead, uintptr_t fid, lfs_fdesc_t **fdesc, uint32_t *idx);
/* Increase refc atomically and return previous value */
int32_t lfs_fdesc_refc_inc(lfs_fdesc_t *fdesc);
/* Decrease refc atomically and return previous value.
 * If refc goes to 0, also release the entry */
uint32_t lfs_fdesc_refc_dec(lfs_fdesc_t *fdesc);

/* To be called by a filesystem before processing its local request */
_Bool lfs_fdesc_req_start(lfs_fdeschead_t *head, lfs_t *req, lfs_fdesc_t **fd, int32_t *ret);
/* To be called by a filesystem after processing its local request */
void lfs_fdesc_req_finish(lfs_fdesc_t *fdesc, lfs_t *req, int32_t sta);

/*
 * Serialized version of "union lfs" used when forwarding PFS request to process
 */

/* Assuming size of uring buffer header <= 128 bytes so that uring data
 * fits to one page
 */
#define LFS_SERIALIZED_SIZE (4096UL - 256)

/*
 *  Type Definitions
 */

struct  lfs_open_serialized
{
    struct lfs_open r_open;
    const char      name_copy[PATH_MAX];    /* User-land store              */
    fid_t           fid_copy;
    uint32_t        r_ret; /* I/O error */
};

struct  lfs_close_serialized
{
    struct lfs_close r_close;
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_read_serialized
{
    struct lfs_read r_read;
    size_t          len_copy;
    off_t           off_copy;
    off_t           ret_copy;
    uint32_t        r_ret; /* I/O error */

    /* Non-NULL "buf" means buffer is large and it points to original
     * buf mapped in server's logical address space.  NULL "buf" means
     * buffer is small and copy of original buf is appended to the end
     * of this structure.
     */
    /* char buf_copy[0]; */
};

/* The max size of the "buf" contents field in the uring packet. Note
 * that uring packet size is LFS_SERIALIZED_SIZE and "buf" contents can
 * follow the struct contents in the uring packet. */
#define LFS_SERIALIZED_READ_SIZE ((LFS_SERIALIZED_SIZE) - sizeof(struct lfs_read_serialized))

struct  lfs_write_serialized
{
    struct lfs_write r_write;
    size_t          len_copy;
    off_t           off_copy;
    off_t           ret_copy;
    uint32_t        r_ret; /* I/O error */

    /* Non-NULL "buf" means buffer is large and it points to original
     * buf mapped in server's logical address space.  NULL "buf" means
     * buffer is small and copy of original buf is appended to the end
     * of this structure.
     */
    /* char buf_copy[0]; */
};

#define LFS_SERIALIZED_WRITE_SIZE ((LFS_SERIALIZED_SIZE) - sizeof(struct lfs_write_serialized))

struct  lfs_ioctl_serialized
{
    struct lfs_ioctl r_ioctl;
    int              info_copy;
    uint32_t         r_ret; /* I/O error */

    /* Non-NULL "data" means buffer is large and it points to original
     * data mapped in server's logical address space.  NULL "data" means
     * buffer is small and copy of original data is appended to the end
     * of this structure.
     */
    /* char        data_copy[0]; */
};

#define LFS_SERIALIZED_IOCTL_SIZE ((LFS_SERIALIZED_SIZE) - sizeof(struct lfs_ioctl_serialized))

struct  lfs_seek_serialized
{
    struct lfs_seek r_seek;
    off_t           off_copy;
    uint32_t        r_ret; /* I/O error */
};

struct  lfs_link_serialized
{
    struct lfs_link  r_link;
    const char       oldn_copy[PATH_MAX];
    const char       newn_copy[PATH_MAX];
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_fsync_serialized
{
    struct  lfs_fsync r_fsync;
    uint32_t          r_ret; /* I/O error */
};

struct  lfs_access_serialized
{
    struct lfs_access r_access;
    const char        name_copy[PATH_MAX];
    uint32_t          r_ret; /* I/O error */
};

struct  lfs_fstat_serialized
{
    struct lfs_fstat r_fstat;
    struct stat     ptr_copy;
    uint32_t        r_ret; /* I/O error */
};

struct  lfs_stat_serialized
{
    struct lfs_stat r_stat;
    const char      name_copy[PATH_MAX];
    struct stat     ptr_copy;
    uint32_t        r_ret; /* I/O error */
};

struct  lfs_chmod_serialized
{
    struct lfs_chmod r_chmod;
    const char       name_copy[PATH_MAX];
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_fchmod_serialized
{
    struct lfs_fchmod r_chmod;
    uint32_t          r_ret; /* I/O error */
};

struct  lfs_chown_serialized
{
    struct lfs_chown r_chown;
    const char       name_copy[PATH_MAX];
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_fchown_serialized
{
    struct lfs_fchown r_chown;
    uint32_t          r_ret; /* I/O error */
};

struct  lfs_utime_serialized
{
    struct lfs_utime r_utime;
    const char       name_copy[PATH_MAX];
    struct utimbuf   timb_copy;
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_trunc_serialized
{
    struct lfs_trunc r_trunc;
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_unlink_serialized
{
    struct lfs_unlink r_unlink;
    const char       name_copy[PATH_MAX];
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_rename_serialized
{
    struct lfs_rename r_rename;
    const char        oldn_copy[PATH_MAX];
    const char        newn_copy[PATH_MAX];
    uint32_t          r_ret; /* I/O error */
};

struct  lfs_rmdir_serialized
{
    struct lfs_rmdir r_rmdir;
    const char       name_copy[PATH_MAX];
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_mkdir_serialized
{
    struct lfs_mkdir r_mkdir;
    const char       name_copy[PATH_MAX];
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_chdir_serialized
{
    struct lfs_chdir r_chdir;
    const char       name_copy[PATH_MAX];
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_fchdir_serialized
{
    struct lfs_fchdir r_fchdir;
    char              buf_copy[PATH_MAX];
    uint32_t          r_ret; /* I/O error */
};

struct  lfs_readdir_serialized
{
    struct lfs_readdir r_readdir;
    struct dirent   ptr_copy;
    size_t          len_copy;
    off_t           off_copy;
    off_t           ret_copy;
    uint32_t        r_ret; /* I/O error */
};

#define LFS_SERIALIZED_READDIR_SIZE ((LFS_SERIALIZED_SIZE) - sizeof(struct lfs_readdir_serialized))

struct  lfs_statvfs_serialized
{
    struct lfs_statvfs r_statvfs;
    const char         name_copy[PATH_MAX];
    struct statvfs     ptr_copy;
    uint32_t           r_ret; /* I/O error */
};

struct  lfs_fstatvfs_serialized
{
    struct lfs_fstatvfs r_fstatvfs;
    struct statvfs      ptr_copy;
    uint32_t            r_ret; /* I/O error */
};

struct lfs_select_serialized
{
    struct lfs_select r_select;
    struct selreq     selreq_copy;
    uint32_t          r_ret; /* I/O error */
};

/* lfs_getdone is not serialized because I/O done is notified to
 * dedicated user-mode thread */

/* Ask HW-cluster leader PFS for remote process hosting the name */
#if (MCOS_CFG_GLB_HWCL_MAX > 1)
struct  lfs_findsrv_serialized
{
    struct lfs_findsrv r_findsrv;
    const char     name_copy[PATH_MAX];    /* File name                    */

    pfs_t          pfsid_copy;
    int            pflags_copy;
    dev_t          pfile_copy;
    int            dflags_copy;
    int            srvid_copy;
    uint32_t       r_ret; /* I/O error */
};
#endif /* (MCOS_CFG_GLB_HWCL_MAX > 1) */

struct  lfs_sync_serialized
{
    struct lfs_sync r_sync;
    uint32_t        r_ret; /* I/O error */
};

struct  lfs_posix_fallocate_serialized
{
    struct lfs_posix_fallocate r_posix_fallocate;
    uint32_t                   r_ret; /* I/O error */
};

struct  lfs_mmap_serialized
{
    struct lfs_mmap r_mmap;
    uint32_t        r_ret; /* I/O error */
};

struct  lfs_munmap_serialized
{
    struct lfs_munmap r_munmap;
    uint32_t          r_ret; /* I/O error */
};

struct  lfs_msync_serialized
{
    struct lfs_msync r_msync;
    uint32_t         r_ret; /* I/O error */
};

struct  lfs_dup_serialized
{
    struct lfs_dup r_dup;
    uint32_t       r_ret; /* I/O error */
};

struct  lfs_utimens_serialized
{
    struct lfs_utimens r_utimens;
    char               name_copy[PATH_MAX];
    struct timespec    atime_copy;
    struct timespec    mtime_copy;
    uint32_t           r_ret; /* I/O error */
};

struct  lfs_futimens_serialized
{
    struct lfs_futimens r_futimens;
    struct timespec     atime_copy;
    struct timespec     mtime_copy;
    uint32_t            r_ret; /* I/O error */
};

struct  lfs_filepath_serialized
{
    struct lfs_filepath r_filepath;
    char                buf_copy[PATH_MAX];
    size_t              buflen_copy;
    uint32_t            r_ret; /* I/O error */
};

typedef union
{
    int32_t                         r_code;
    struct lfs_open_serialized             r_open;
    struct lfs_close_serialized            r_close;
    struct lfs_read_serialized             r_read;
    struct lfs_write_serialized            r_write;
    struct lfs_ioctl_serialized            r_ioctl;
    struct lfs_seek_serialized             r_seek;
    struct lfs_link_serialized             r_link;
    struct lfs_fsync_serialized            r_fsync;
    struct lfs_access_serialized           r_access;
    struct lfs_fstat_serialized            r_fstat;
    struct lfs_stat_serialized             r_stat;
    struct lfs_chmod_serialized            r_chmod;
    struct lfs_fchmod_serialized           r_fchmod;
    struct lfs_chown_serialized            r_chown;
    struct lfs_fchown_serialized           r_fchown;
    struct lfs_utime_serialized            r_utime;
    struct lfs_trunc_serialized            r_trunc;
    struct lfs_unlink_serialized           r_unlink;
    struct lfs_rename_serialized           r_rename;
    struct lfs_rmdir_serialized            r_rmdir;
    struct lfs_mkdir_serialized            r_mkdir;
    struct lfs_chdir_serialized            r_chdir;
    struct lfs_fchdir_serialized           r_fchdir;
    struct lfs_readdir_serialized          r_readdir;
    struct lfs_statvfs_serialized          r_statvfs;
    struct lfs_fstatvfs_serialized         r_fstatvfs;
    struct lfs_select_serialized           r_select;
#if (MCOS_CFG_GLB_HWCL_MAX > 1)
    struct lfs_findsrv_serialized          r_findsrv;
#endif
    struct lfs_sync_serialized             r_sync;
    struct lfs_posix_fallocate_serialized  r_posix_fallocate;
    struct lfs_mmap_serialized             r_mmap;
    struct lfs_munmap_serialized           r_munmap;
    struct lfs_msync_serialized            r_msync;
    struct lfs_dup_serialized              r_dup;
    struct lfs_utimens_serialized          r_utimens;
    struct lfs_futimens_serialized         r_futimens;
    struct lfs_filepath_serialized         r_filepath;
} lfs_serialized_t;

/*
 * Max length of internal named object name
 * (synchronize with minimum acceptable value in POSIX)
 */
#define LFS_NAME_MAX _POSIX_NAME_MAX

/* Channel for sending lfs command to process */
struct lfs_cmd_channel {
    uring_t ring_to_server;
    uring_t ring_to_client;
    uint32_t server_lsid; /* For mapping client buffer in server process' space. Note that the type is "UW" of TKSE. */
    pfs_t pfs_id;
    int32_t (*func)(lfs_t *req);
};

/* pfs in process view. Use it only with functions interacting with users. Use struct lfs_cmd_channel_t * for internal funcions.  */
#define pfs_handle_t uintptr_t

//#define ULFS_ENABLE_CROSS_LOGICAL_SPACE_MAP

 /* LFS internal only error code meaning rmfs() is called and
  * pfs_service should return
  */
#define ERMFS -1

/* open/openat() */
typedef struct {
    int         id;
    int         dirfd;
    const char* path;
    int         oflag;
    mode_t      mode;
} svc_packet_open_t;

/* close() */
typedef struct {
    int         id;
    int         fildes;
} svc_packet_close_t;

/* read() */
typedef struct {
    int         id;
    int         fildes;
    void        *buf;
    size_t      *nbyte;
} svc_packet_read_t;

/* write() */
typedef struct {
    int         id;
    int         fildes;
    const void  *buf;
    size_t      *nbyte;
} svc_packet_write_t;

/* unlink/unlinkat() */
typedef struct {
    int         id;
    int         dirfd;
    const char* path;
    int         flags;
} svc_packet_unlink_t;

/* lseek() */
typedef struct {
    int         id;
    int         fildes;
    off_t       offset;
    int         whence;
} svc_packet_lseek_t;

/* isatty() */
typedef struct {
    int         id;
    int         fildes;
} svc_packet_isatty_t;

/* fstat() */
typedef struct {
    int         id;
    int         fildes;
    struct stat *buf;
} svc_packet_fstat_t;

/* ftruncate() */
typedef struct {
    int         id;
    int         fildes;
    off_t       length;
} svc_packet_ftruncate_t;

/* access/faccessat() */
typedef struct {
    int         id;
    int         dirfd;
    const char  *path;
    int         amode;
    int         flags;
} svc_packet_access_t;

/* chdir() */
typedef struct {
    int         id;
    const char  *path;
} svc_packet_chdir_t;

/* chmod/fchmodat() */
typedef struct {
    int         id;
    int         dirfd;
    const char  *path;
    mode_t      mode;
    int         flags;
} svc_packet_chmod_t;

/* chown/fchownat() */
typedef struct {
    int         id;
    int         dirfd;
    const char  *path;
    uid_t       owner;
    gid_t       group;
    int         flags;
} svc_packet_chown_t;

/* dup() */
typedef struct {
    int         id;
    int         fildes;
} svc_packet_dup_t;

/* dup2() */
typedef struct {
    int         id;
    int         fildes;
    int         fildes2;
} svc_packet_dup2_t;

/* fchdir() */
typedef struct {
    int         id;
    int         fildes;
} svc_packet_fchdir_t;

/* fchmod() */
typedef struct {
    int         id;
    int         fildes;
    mode_t      mode;
} svc_packet_fchmod_t;

/* fchown() */
typedef struct {
    int         id;
    int         fildes;
    uid_t       owner;
    gid_t       group;
} svc_packet_fchown_t;

/* fcntl() */
typedef struct {
    int         id;
    int         fildes;
    int         cmd;
    int         arg;
} svc_packet_fcntl_t;

/* fdatasync() */
typedef struct {
    int         id;
    int         fildes;
} svc_packet_fdatasync_t;

/* fstatvfs() */
typedef struct {
    int             id;
    int             fildes;
    struct statvfs  *buf;
} svc_packet_fstatvfs_t;

/* fsync() */
typedef struct {
    int         id;
    int         fildes;
} svc_packet_fsync_t;

/* getcwd() */
typedef struct {
    int         id;
    char        *buf;
    size_t      size;
    char        **cwd;
} svc_packet_getcwd_t;

/* ioctl() */
typedef struct {
    int         id;
    int         fildes;
    int         request;
    void        *arg;
} svc_packet_ioctl_t;

/* link/linkat() */
typedef struct {
    int         id;
    int         flags;
    const char  *path1;
    const char  *path2;
    int         oldfd;
    int         newfd;
} svc_packet_link_t;

/* stat/fstatat() */
typedef struct {
    int         id;
    int         dirfd;
    const char  *path;
    struct stat *buf;
    int         flags;
} svc_packet_stat_t;

/* statvfs() */
typedef struct {
    int             id;
    const char      *path;
    struct statvfs  *buf;
} svc_packet_statvfs_t;

/* truncate() */
typedef struct {
    int         id;
    const char  *path;
    off_t       length;
} svc_packet_truncate_t;

/* realpath() */
typedef struct {
    int         id;
    const char  *file_name;
    char        *resolved_name;
    char        **name;
} svc_packet_realpath_t;

/* umask() */
typedef struct {
    int         id;
    mode_t      cmask;
} svc_packet_umask_t;

/* utime() */
typedef struct {
    int                  id;
    const char           *path;
    const struct utimbuf *times;
} svc_packet_utime_t;

/* pread() */
typedef struct {
    int         id;
    int         fildes;
    void        *buf;
    size_t      *nbyte;
    off_t       offset;
} svc_packet_pread_t;

/* pwrite() */
typedef struct {
    int         id;
    int         fildes;
    const void  *buf;
    size_t      *nbyte;
    off_t       offset;
} svc_packet_pwrite_t;

/* readv() */
typedef struct {
    int                 id;
    int                 fildes;
    const struct iovec  *iov;
    int                 iovcnt;
    ssize_t             *rbyte;
} svc_packet_readv_t;

/* writev() */
typedef struct {
    int                 id;
    int                 fildes;
    const struct iovec  *iov;
    int                 iovcnt;
    ssize_t             *rbyte;
} svc_packet_writev_t;

/* rmdir() */
typedef struct {
    int         id;
    const char  *path;
} svc_packet_rmdir_t;

/* select() */
typedef struct {
    int                     id;
    int                     nfds;
    fd_set                  *readfds;
    fd_set                  *writefds;
    fd_set                  *errorfds;
    struct timeval          *timeout;
} svc_packet_select_t;

/* pselect() */
typedef struct {
    int                     id;
    int                     nfds;
    fd_set                  *readfds;
    fd_set                  *writefds;
    fd_set                  *errorfds;
    const struct timespec   *timeout;
    const sigset_t          *sigmask;
} svc_packet_pselect_t;

/* __readdir() */
typedef struct {
    int             id;
    DIR             *dir;
    struct dirent   *ent;
    struct dirent   **dirent;
} svc_packet_readdir_t;

/* mkdir/mkdirat() */
typedef struct {
    int         id;
    int         dirfd;
    const char  *path;
    mode_t      mode;
} svc_packet_mkdir_t;

/* rename/renameat() */
typedef struct {
    int         id;
    int         dirfd;
    const char  *old;
    const char  *new;
    int         oldfd;
    int         newfd;
} svc_packet_rename_t;

/* sync() */
typedef struct {
    int32_t     id;
} svc_packet_sync_t;

/* posix_fallocate() */
typedef struct {
    int32_t     id;
    int32_t     fildes;
    off_t       offset;
    off_t       length;
} svc_packet_posix_fallocate_t;

/* devctl() */
typedef struct {
    int32_t     id;
    int32_t     fildes;
    int32_t     dcmd;
    void        *data;
    size_t      size;
    int32_t     *info;
} svc_packet_devctl_t;

/* utimensat()/utimes() */
typedef struct {
    int32_t                 id;
    int32_t                 dirfd;
    const char *            path;
    const struct timespec * tv;
    int32_t                 flags;
} svc_packet_utimens_t;

/* futimens()/futimes() */
typedef struct {
    int32_t                 id;
    int32_t                 fildes;
    const struct timespec * tv;
} svc_packet_futimens_t;

/* mkfs() */
typedef struct {
    int32_t                id;
    const char*            name;
    int32_t                pflags;
    struct lfs_cmd_channel *uchannel;
    pfs_handle_t           *handle; /* out */
} svc_packet_mkfs_t;

/* rmfs() */
typedef struct {
    int32_t id;
    const char *name;
} svc_packet_rmfs_t;


/* List of POSIX LFS APIs */
typedef union {
    int                             id;
    svc_packet_open_t               param_open_;
    svc_packet_close_t              param_close_;
    svc_packet_read_t               param_read_;
    svc_packet_write_t              param_write_;
    svc_packet_unlink_t             param_unlink_;
    svc_packet_lseek_t              param_lseek_;
    svc_packet_isatty_t             param_isatty_;
    svc_packet_fstat_t              param_fstat_;
    svc_packet_ftruncate_t          param_ftruncate_;
    svc_packet_access_t             param_access_;
    svc_packet_chdir_t              param_chdir_;
    svc_packet_chmod_t              param_chmod_;
    svc_packet_chown_t              param_chown_;
    svc_packet_dup_t                param_dup_;
    svc_packet_dup2_t               param_dup2_;
    svc_packet_fchdir_t             param_fchdir_;
    svc_packet_fchmod_t             param_fchmod_;
    svc_packet_fchown_t             param_fchown_;
    svc_packet_fcntl_t              param_fcntl_;
    svc_packet_fdatasync_t          param_fdatasync_;
    svc_packet_fstatvfs_t           param_fstatvfs_;
    svc_packet_fsync_t              param_fsync_;
    svc_packet_getcwd_t             param_getcwd_;
    svc_packet_ioctl_t              param_ioctl_;
    svc_packet_link_t               param_link_;
    svc_packet_stat_t               param_stat_;
    svc_packet_statvfs_t            param_statvfs_;
    svc_packet_truncate_t           param_truncate_;
    svc_packet_realpath_t           param_realpath_;
    svc_packet_umask_t              param_umask_;
    svc_packet_utime_t              param_utime_;
    svc_packet_pread_t              param_pread_;
    svc_packet_pwrite_t             param_pwrite_;
    svc_packet_readv_t              param_readv_;
    svc_packet_writev_t             param_writev_;
    svc_packet_rmdir_t              param_rmdir_;
    svc_packet_select_t             param_select_;
    svc_packet_pselect_t            param_pselect_;
    svc_packet_readdir_t            param_readdir_;
    svc_packet_mkdir_t              param_mkdir_;
    svc_packet_rename_t             param_rename_;
    svc_packet_sync_t               param_sync_;
    svc_packet_posix_fallocate_t    param_posix_fallocate_;
    svc_packet_devctl_t             param_devctl_;
    svc_packet_utimens_t            param_utimens_;
    svc_packet_futimens_t           param_futimens_;
    svc_packet_mkfs_t               param_mkfs_;
    svc_packet_rmfs_t               param_rmfs_;
} svc_packet_lfs_t;

#endif /* _in_asm_source_ */

//#define DEBUG_ULFS_C
#ifdef DEBUG_ULFS_C
#define dprintf(...) do { \
        mcos_debug_printf(__VA_ARGS__); \
    } while (0)
#else
#define dprintf(...) do {  } while (0)
#endif

#define ULFS_DESERIALIZE_USER_DEFS(command) \
    struct lfs_##command##_serialized *srep = &reply->r_##command;

static int32_t ulfs_deserialize_user(lfs_serialized_t *reply, int32_t ioret)
{
    int32_t ret = 0;

    /* Deserialize result */
    switch (reply->r_code)
    {
    case PFS_OPEN:
    {
        ULFS_DESERIALIZE_USER_DEFS(open);

        dprintf("%s: deserialize: open\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_CLOSE:
    {
        ULFS_DESERIALIZE_USER_DEFS(close);

        dprintf("%s: deserialize: close\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_READ:
    {
        ULFS_DESERIALIZE_USER_DEFS(read);

        dprintf("%s: deserialize: read\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_WRITE:
    {
        ULFS_DESERIALIZE_USER_DEFS(write);

        dprintf("%s: deserialize: write\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_IOCTL:
    {
        ULFS_DESERIALIZE_USER_DEFS(ioctl);

        dprintf("%s: deserialize: ioctl\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_LINK:
    {
        ULFS_DESERIALIZE_USER_DEFS(link);

        dprintf("%s: deserialize: link\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_FSYNC:
    {
        ULFS_DESERIALIZE_USER_DEFS(fsync);

        dprintf("%s: deserialize: fsync\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_CHMOD:
    {
        ULFS_DESERIALIZE_USER_DEFS(chmod);

        dprintf("%s: deserialize: chmod\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_FCHMOD:
    {
        ULFS_DESERIALIZE_USER_DEFS(fchmod);

        dprintf("%s: deserialize: fchmod\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_CHOWN:
    {
        ULFS_DESERIALIZE_USER_DEFS(chown);

        dprintf("%s: deserialize: chown\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_FCHOWN:
    {
        ULFS_DESERIALIZE_USER_DEFS(fchown);

        dprintf("%s: deserialize: fchown\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_UTIME:
    {
        ULFS_DESERIALIZE_USER_DEFS(utime);

        dprintf("%s: deserialize: utime\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_TRUNC:
    {
        ULFS_DESERIALIZE_USER_DEFS(trunc);

        dprintf("%s: deserialize: trunc\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_UNLINK:
    {
        ULFS_DESERIALIZE_USER_DEFS(unlink);

        dprintf("%s: deserialize: unlink\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_RENAME:
    {
        ULFS_DESERIALIZE_USER_DEFS(rename);

        dprintf("%s: deserialize: rename\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_RMDIR:
    {
        ULFS_DESERIALIZE_USER_DEFS(rmdir);

        dprintf("%s: deserialize: rmdir\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_MKDIR:
    {
        ULFS_DESERIALIZE_USER_DEFS(mkdir);

        dprintf("%s: deserialize: mkdir\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_CHDIR:
    {
        ULFS_DESERIALIZE_USER_DEFS(chdir);

        dprintf("%s: deserialize: chdir\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_SYNC:
    {
        ULFS_DESERIALIZE_USER_DEFS(sync);

        dprintf("%s: deserialize: sync\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_POSIX_FALLOCATE:
    {
        ULFS_DESERIALIZE_USER_DEFS(posix_fallocate);

        dprintf("%s: deserialize: posix_fallocate\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_MUNMAP:
    {
        ULFS_DESERIALIZE_USER_DEFS(munmap);

        dprintf("%s: deserialize: munmap\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_MSYNC:
    {
        ULFS_DESERIALIZE_USER_DEFS(msync);

        dprintf("%s: deserialize: msync\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_DUP:
    {
        ULFS_DESERIALIZE_USER_DEFS(dup);

        dprintf("%s: deserialize: dup\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_UTIMENS:
    {
        ULFS_DESERIALIZE_USER_DEFS(utimens);

        dprintf("%s: deserialize: utimens\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_FUTIMENS:
    {
        ULFS_DESERIALIZE_USER_DEFS(futimens);

        dprintf("%s: deserialize: futimens\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_FCHDIR:
    {
        ULFS_DESERIALIZE_USER_DEFS(fchdir);

        dprintf("%s: deserialize: fchdir\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_SEEK:
    {
        ULFS_DESERIALIZE_USER_DEFS(seek);

        dprintf("%s: deserialize: seek\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_FSTAT:
    {
        ULFS_DESERIALIZE_USER_DEFS(fstat);

        dprintf("%s: deserialize: fstat\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_STAT:
    {
        ULFS_DESERIALIZE_USER_DEFS(stat);

        dprintf("%s: deserialize: stat\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_READDIR:
    {
        ULFS_DESERIALIZE_USER_DEFS(readdir);

        dprintf("%s: deserialize: readdir\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_STATVFS:
    {
        ULFS_DESERIALIZE_USER_DEFS(statvfs);

        dprintf("%s: deserialize: statvfs\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_FSTATVFS:
    {
        ULFS_DESERIALIZE_USER_DEFS(fstatvfs);

        dprintf("%s: deserialize: fstatvfs\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_SELECT:
    {
        ULFS_DESERIALIZE_USER_DEFS(select);

        dprintf("%s: deserialize: select\n", __func__);
        srep->r_ret = ioret;
    }
    break;

#if (MCOS_CFG_GLB_HWCL_MAX > 1)
    case PFS_FINDSRV:
    {
        ULFS_DESERIALIZE_USER_DEFS(findsrv);

        dprintf("%s: deserialize: findsrv\n", __func__);
        srep->r_ret = ioret;
    }
    break;
#endif

    case PFS_MMAP:
    {
        ULFS_DESERIALIZE_USER_DEFS(mmap);

        dprintf("%s: deserialize: mmap\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    case PFS_FILEPATH:
    {
        ULFS_DESERIALIZE_USER_DEFS(filepath);

        dprintf("%s: deserialize: filepath\n", __func__);
        srep->r_ret = ioret;
    }
    break;

    default:
        break;
    }

    return ret;
}

/* Receive request from dev dispatcher and send result to dev
 * dispatcher via uring
 */
static int pfs_user_dispatch(struct lfs_cmd_channel *uchannel, lfs_t *req)
{
    int32_t ret = 0;

    /* Process request, compose reply in "req" */
    ret = uchannel->func(req);

    if (ret >= 0 || ret == ERMFS)
    {
        int32_t _ret;

        _ret = ulfs_deserialize_user((lfs_serialized_t *)req,
                                     ret);
        if (_ret)
        {
            dprintf("%s: ulfs_serialize_user with %d\n",
                    __func__, _ret);
        }
    }

    /* Send. Note that ERMFS is LFS internal only
       code meaning pfs_service() should return.
     */
    if (ret >= 0 || ret == ERMFS)
    {
        int _ret;
        
        _ret = uring_interrupt_notify(uchannel->ring_to_client,
                                     req,
                                     LFS_SERIALIZED_SIZE);
        if (_ret)
        {
            dprintf("%s: uring_interrupt_notify failed with %d\n",
                    __func__, ret);
            ret = -EIO;
        }
    }

    return ret;
}

#define ULFS_SERIALIZE_USER_DEFS(command) \
    struct lfs_##command##_serialized *sreq = &_lfsreq->r_##command; \
    struct lfs_##command *preq = (struct lfs_##command *)_lfsreq;

static int32_t ulfs_serialize_user(lfs_serialized_t *_lfsreq)
{
    int32_t ret = 0;

    switch (_lfsreq->r_code)
    {
    case PFS_OPEN:
        dprintf("%s: serialize: open\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(open);

            preq->name = sreq->name_copy;
            preq->fid = &sreq->fid_copy;
        }
        break;

    case PFS_CLOSE:
        dprintf("%s: serialize: close\n", __func__);
        break;

    case PFS_READ:
        dprintf("%s: serialize: read\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(read);

            preq->len = &sreq->len_copy;
            preq->off = &sreq->off_copy;
            preq->ret = &sreq->ret_copy;

            if (*preq->len > 0)
            {
#ifdef ULFS_ENABLE_CROSS_LOGICAL_SPACE_MAP
                if (*preq->len > LFS_SERIALIZED_READ_SIZE)
                {
                }
                else
#endif
                {
                    preq->buf = &sreq[1]; /* buf is copied to uring packet */;
                }
            }
        }
        break;

    case PFS_WRITE:
        dprintf("%s: serialize: write\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(write);

            preq->len = &sreq->len_copy;
            dprintf("%s: len: %lx\n", __func__, *preq->len);
            preq->off = &sreq->off_copy;
            preq->ret = &sreq->ret_copy;

            if (*preq->len > 0)
            {
#ifdef ULFS_ENABLE_CROSS_LOGICAL_SPACE_MAP
                if (*preq->len > LFS_SERIALIZED_WRITE_SIZE)
                {
                }
                else
#endif
                {
                    preq->buf = &sreq[1]; /* buf is copied to uring packet */;
                }
            }
        }
        break;

    case PFS_IOCTL:
        dprintf("%s: serialize: ioctl\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(ioctl);

            preq->info = &sreq->info_copy;

            if (preq->size > 0)
            {
#ifdef ULFS_ENABLE_CROSS_LOGICAL_SPACE_MAP
                if (preq->size > LFS_SERIALIZED_IOCTL_SIZE)
                {
                }
                else
#endif
                {
                    preq->data = &sreq[1]; /* data is copied to uring packet */;
                }
            }
        }
        break;

    case PFS_SEEK:
        dprintf("%s: serialize: seek\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(seek);

            preq->off = &sreq->off_copy;
        }
        break;

    case PFS_LINK:
        dprintf("%s: serialize: link\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(link);

            preq->oldn = sreq->oldn_copy;
            preq->newn = sreq->newn_copy;
        }
        break;

    case PFS_FSYNC:
        dprintf("%s: serialize: fsync\n", __func__);
        break;

    case PFS_ACCESS:
        dprintf("%s: serialize: access\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(access);

            preq->name = sreq->name_copy;
        }
        break;

    case PFS_FSTAT:
        dprintf("%s: serialize: fstat\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(fstat);

            preq->ptr = &sreq->ptr_copy;
        }
        break;

    case PFS_STAT:
        dprintf("%s: serialize: stat\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(stat);

            preq->name = sreq->name_copy;
            preq->ptr = &sreq->ptr_copy;
        }
        break;

    case PFS_CHMOD:
        dprintf("%s: serialize: chmod\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(chmod);

            preq->name = sreq->name_copy;
        }
        break;

    case PFS_FCHMOD:
        dprintf("%s: serialize: fchmod\n", __func__);
        break;

    case PFS_CHOWN:
        dprintf("%s: serialize: chown\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(chown);

            preq->name = sreq->name_copy;
        }
        break;

    case PFS_FCHOWN:
        dprintf("%s: serialize: fchown\n", __func__);
        break;

    case PFS_UTIME:
        dprintf("%s: serialize: utime\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(utime);

            preq->name = sreq->name_copy;
            preq->timb = &sreq->timb_copy;
        }
        break;

    case PFS_TRUNC:
        dprintf("%s: serialize: trunc\n", __func__);
        break;

    case PFS_UNLINK:
        dprintf("%s: serialize: unlink\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(unlink);

            preq->name = sreq->name_copy;
        }
        break;

    case PFS_RENAME:
        dprintf("%s: serialize: rename\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(rename);

            preq->oldn = sreq->oldn_copy;
            preq->newn = sreq->newn_copy;
        }
        break;

    case PFS_RMDIR:
        dprintf("%s: serialize: rmdir\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(rmdir);

            preq->name = sreq->name_copy;
        }
        break;

    case PFS_MKDIR:
        dprintf("%s: serialize: mkdir\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(mkdir);

            preq->name = sreq->name_copy;
        }
        break;

    case PFS_CHDIR:
        dprintf("%s: serialize: chdir\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(chdir);

            preq->name = sreq->name_copy;
        }
        break;

    case PFS_FCHDIR:
        dprintf("%s: serialize: fchdir\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(fchdir);

            preq->buf = sreq->buf_copy;
        }
        break;

    case PFS_READDIR:
        dprintf("%s: serialize: read\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(readdir);

            preq->ptr = &sreq->ptr_copy;
            preq->len = &sreq->len_copy;
            preq->off = &sreq->off_copy;
            preq->ret = &sreq->ret_copy;
        }
        break;

    case PFS_STATVFS:
        dprintf("%s: serialize: statvfs\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(statvfs);

            preq->name = sreq->name_copy;
            preq->ptr = &sreq->ptr_copy;
        }
        break;

    case PFS_FSTATVFS:
        dprintf("%s: serialize: fstatvfs\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(fstatvfs);

            preq->ptr = &sreq->ptr_copy;
        }
        break;

    case PFS_SELECT:
        dprintf("%s: serialize: select\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(select);

            preq->selreq = &sreq->selreq_copy;
        }
        break;

#if (MCOS_CFG_GLB_HWCL_MAX > 1)
    case PFS_FINDSRV:
        dprintf("%s: serialize: findsrv\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(findsrv);

            preq->name = sreq->name_copy;

            sreq->pfsid = &preq->pfsid_copy;
            sreq->pflags = &preq->pflags_copy;
            sreq->pfile = &preq->pfile_copy;
            sreq->dflags = &preq->dflags_copy;
            sreq->srvid = &preq->srvid_copy;
        }
        break;
#endif

    case PFS_SYNC:
        dprintf("%s: serialize: sync\n", __func__);
        break;

    case PFS_POSIX_FALLOCATE:
        dprintf("%s: serialize: posix_fallocate\n", __func__);
        break;

    case PFS_MMAP:
        dprintf("%s: serialize: mmap\n", __func__);
        break;

    case PFS_MUNMAP:
        dprintf("%s: serialize: munmap\n", __func__);
        break;

    case PFS_MSYNC:
        dprintf("%s: serialize: msync\n", __func__);
        break;

    case PFS_DUP:
        dprintf("%s: serialize: dup\n", __func__);
        break;

    case PFS_UTIMENS:
        dprintf("%s: serialize: utimens\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(utimens);

            preq->name = sreq->name_copy;
            preq->atime = &sreq->atime_copy;
            preq->mtime = &sreq->mtime_copy;
        }
        break;

    case PFS_FUTIMENS:
        dprintf("%s: serialize: futimens\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(futimens);

            preq->atime = &sreq->atime_copy;
            preq->mtime = &sreq->mtime_copy;
        }
        break;

    case PFS_FILEPATH:
        dprintf("%s: serialize: filepath\n", __func__);
        {
            ULFS_SERIALIZE_USER_DEFS(filepath);

            preq->buf = sreq->buf_copy;
            preq->buflen = &sreq->buflen_copy;
        }
        break;

    default:
        dprintf("%s: unknown command: r_code: %d\n", __func__, _lfsreq->r_code);
        ret = ENOSYS;
        break;
    }

    return ret;
}

int32_t pfs_service(pfs_handle_t handle)
{
    int32_t ret = 0;
    int32_t _ret;
    struct lfs_cmd_channel *uchannel = (struct lfs_cmd_channel *)handle;

    lfs_t *req = NULL;

    req = malloc(LFS_SERIALIZED_SIZE);
    if (req == NULL)
    {
        dprintf("%s: malloc failed\n", __func__);
        ret = -ENOMEM;
    }

    /* Note that negative value of ret means server should exit */
    if (ret >= 0)
    {
        while (1)
        {
            ret = uring_interrupt_receive(uchannel->ring_to_server,
                                          req,
                                          LFS_SERIALIZED_SIZE);
            if (ret)
            {
                dprintf("%s: uring_interrupt_receive failed with %d\n",
                        __func__, ret);
                break;
            }

            if (ret >= 0)
            {
                ret = ulfs_serialize_user((lfs_serialized_t *)req);
                if (ret)
                {
                    dprintf("%s: ulfs_serialize_user with %d\n",
                            __func__, ret);
                    break;
                }
            }

            if (ret >= 0)
            {
                ret = pfs_user_dispatch(uchannel, req);

                if (ret == ERMFS || ret < 0)
                {
                    /* Wait until reply semaphore is ready to get
                     * freed. Note that only record first internal
                     * error
                     */
                    _ret = uring_rr_receive_reply_sem_rel(uchannel->ring_to_server);
                    ret = (ret >= 0) ? ((_ret == 0) ? ret : -_ret) : ret;
                    if (_ret)
                    {
                        dprintf("%s: uring_rr_send_reply_sem_rel failed with %d\n",
                                __func__, _ret);
                    }
                }

                /* ERMFS means rmfs() is called */
                if (ret == ERMFS)
                {
                    dprintf("%s: rmfs() is called\n", __func__);
                    ret = 0;
                    break;
                }
                else if (ret < 0)
                {
                    dprintf("%s: pfs_user_dispatch failed with %d, pid: %d\n",
                            __func__, ret, getpid());
                    break;
                }
            }
        }
    }

    free(req);

    ret = uring_destroy(uchannel->ring_to_server);
    ret = (ret == MCOS_EOK) ? 0 : -EINVAL;
    if (ret)
    {
        dprintf("%s: uring_destroy failed with %d\n",
                __func__, ret);
    }

    ret = uring_destroy(uchannel->ring_to_client);
    ret = (ret == MCOS_EOK) ? 0 : -EINVAL;
    if (ret)
    {
        dprintf("%s: uring_destroy failed with %d\n",
                __func__, ret);
    }

    free(uchannel);

    dprintf("%s: exit, ret: %d\n", __func__, ret);
    return ret;
}

typedef int32_t (*function_t)(lfs_t *req);
pfs_handle_t mkfs(const char *name, int32_t pflags,...)
{
    int ret = 0;
    struct lfs_cmd_channel *uchannel = NULL;
    pfs_handle_t handle = NULL;
    uring_par_t param = {
        .qlen = 1,
        .n_sectors = 1,

        /* Single size is used because receiver side checks if size matches */
        .sector_bsz = {
            LFS_SERIALIZED_SIZE
        },

        .sector_bn = {
            1,
        },
        .opt = 0,
    };

    va_list ap;
    va_start(ap, pflags);
    function_t func = va_arg(ap, function_t);

    /* Create urings. Note that they should reside in process local space. */

    if (sizeof(param.sector_bsz) / sizeof(param.sector_bsz[0]) !=
        sizeof(param.sector_bn) / sizeof(param.sector_bn[0]))
    {
        dprintf("%s: bsz array-size or bn array-size < # of sectors\n",
                __func__);
        ret = EINVAL;
    }

    if (ret == 0)
    {
        
        if (func == NULL)
        {
            ret = EINVAL;
        }
    }

    if (ret == 0)
    {
        uchannel = malloc(sizeof(struct lfs_cmd_channel));
        if (uchannel == NULL)
        {
            dprintf("%s: malloc channel failed\n",
                    __func__);
            ret = ENOMEM;
        }
    }

    /* For pfs_user_dipatch to call uchannel->func */
    if (ret == 0)
    {
        uchannel->func = func;

        ret = uring_create(&uchannel->ring_to_server, &param);
        ret = (ret == MCOS_EOK) ? 0 : EINVAL;
        if (ret)
        {
            dprintf("%s: uring_create failed with %d\n",
                    __func__, ret);
            free(uchannel);
        }
    }

    if (ret == 0)
    {
        ret = uring_create(&uchannel->ring_to_client, &param);
        ret = (ret == MCOS_EOK) ? 0 : EINVAL;
        if (ret)
        {
            dprintf("%s: uring_create failed with %d\n",
                    __func__, ret);
            uring_destroy(uchannel->ring_to_server);
            free(uchannel);
        }
    }

    /* Call __mkfs */
    if (ret == 0)
    {
        /* Note that "name" must reside in process local space because
         * mkfs checks it by CheckBStrSpaceR().
         */
        ret = call_svc_mkfs(name, pflags, uchannel);
        if (ret)
        {
            int32_t _errno = errno;

            dprintf("%s: SVCID_LFS_MKFS failed, ret: %d, errno: %d\n",
                    __func__, ret, _errno);
            uring_destroy(uchannel->ring_to_server);
            uring_destroy(uchannel->ring_to_client);
            free(uchannel);
            ret = _errno;
        }
    }

    if (ret != 0)
    {
        errno = ret;
        uchannel = NULL;
    }

    return (pfs_handle_t)uchannel;
}

int32_t rmfs(const char *name)
{
    int32_t ret = 0;
    struct lfs_cmd_channel *uchannel = NULL;

    /* The handle is taken from pfs table because the caller is the
     * third-party process and the handle isn't shared with the
     * process and hence the handle can't be passed as an argument
     * of this function.
     * Note that kernel rmfs() checks if name is in user space.
     * Also note that first we make the file invisible and then
     * clean-up the handle contents to avoid use-after-free
     */
    if (ret == 0)
    {
        ret = call_svc_rmfs(name);
        if (ret)
        {
            int _errno = errno;

            dprintf("%s: __rmfs() failed, ret: %d, errno: %d\n",
                    __func__, ret, _errno);
        }
    }

    /* Note that uchannel->ring_to_server, uchannel->ring_to_client,
     * uchannel is freed when exiting event handling loop in
     * pfs_service
     */

    if (ret)
    {
        errno = ret;
        ret = -1;
    }

    return ret;
}
