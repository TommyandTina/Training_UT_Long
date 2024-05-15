/*
    Copyright (C) 2016 By eSOL Co.,Ltd. Tokyo, Japan

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
[lfs_svc.c] - SVC interface library (POSIX LFS)
****************************************************************************/

#include <stdarg.h>
#include <svc/svc_if.h>

#include "stub.h"

#define open    __open__    /* for variadic argument */
#define openat  __openat__
#define fcntl   __fcntl__
#define ioctl   __ioctl__

/* Switch name of the lfs command type for user */
//#include <svc/svc_if_lfs.h>
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
#define pfs_handle_t uintptr_t

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

#undef open                 /* for variadic argument */
#undef openat
#undef fcntl
#undef ioctl
#include <errno.h>
#include <mcos/mcos.h>
#include <mcos/mcos_ds.h>

//#define DEBUG_LFS_SVC_C
#ifdef DEBUG_LFS_SVC_C
#define dprintf(...) do { \
        mcos_debug_printf(__VA_ARGS__); \
    } while (0)
#else
#define dprintf(...) do {  } while (0)
#endif

int32_t open(const char *path, int32_t oflag,...)
{
    svc_packet_open_t   p;
START_TIMER
    va_list ap;
    va_start(ap, oflag);
    mode_t mode = va_arg(ap, mode_t);
    p.id        = SVCID_LFS_OPEN;
    p.dirfd     = AT_FDCWD;
    p.path      = path;
    p.oflag     = oflag;
    p.mode      = mode;
    int32_t ret = (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
STOP_TIMER
    return ret;
}

int32_t close(int32_t fildes)
{
    svc_packet_close_t  p;
START_TIMER
    p.id        = SVCID_LFS_CLOSE;
    p.fildes    = fildes;
    int32_t ret = (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
STOP_TIMER
    return ret;
}

ssize_t read(int32_t fildes, void *buf, size_t nbyte)
{
    svc_packet_read_t   p;
    size_t  len = nbyte; /* Requested/Returned length */

    p.id        = SVCID_LFS_READ;
    p.fildes    = fildes;
    p.buf       = buf;
    p.nbyte     = &len;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return (ssize_t)len;
}

ssize_t write(int32_t fildes, const void *buf, size_t nbyte)
{
    svc_packet_write_t  p;
    size_t  len = nbyte; /* Requested/Returned length */

    p.id        = SVCID_LFS_WRITE;
    p.fildes    = fildes;
    p.buf       = buf;
    p.nbyte     = &len;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return (ssize_t)len;
}

int32_t unlink(const char *path)
{
    svc_packet_unlink_t p;

    p.id        = SVCID_LFS_UNLINK;
    p.dirfd     = AT_FDCWD;
    p.path      = path;
    p.flags     = 0;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

off_t lseek(int32_t fildes, off_t offset, int32_t whence)
{
    svc_packet_lseek_t  p;

    p.id        = SVCID_LFS_LSEEK;
    p.fildes    = fildes;
    p.offset    = offset;
    p.whence    = whence;
    return (off_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int isatty(int fildes)
{
    svc_packet_isatty_t p;

    p.id        = SVCID_LFS_ISATTY;
    p.fildes    = fildes;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fstat(int32_t fildes, struct stat *buf)
{
    svc_packet_fstat_t      p;

    p.id        = SVCID_LFS_FSTAT;
    p.fildes    = fildes;
    p.buf       = buf;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t ftruncate(int32_t fildes, off_t length)
{
    svc_packet_ftruncate_t      p;

    p.id        = SVCID_LFS_FTRUNCATE;
    p.fildes    = fildes;
    p.length    = length;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t access(const char *path, int32_t amode)
{
    svc_packet_access_t     p;

    p.id        = SVCID_LFS_ACCESS;
    p.dirfd     = AT_FDCWD;
    p.path      = path;
    p.amode     = amode;
    p.flags     = 0;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t chdir(const char *path)
{
    svc_packet_chdir_t      p;

    p.id        = SVCID_LFS_CHDIR;
    p.path      = path;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t chmod(const char *path, mode_t mode)
{
    svc_packet_chmod_t      p;

    p.id        = SVCID_LFS_CHMOD;
    p.dirfd     = AT_FDCWD;
    p.path      = path;
    p.mode      = mode;
    p.flags     = 0;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t chown(const char *path, uid_t owner, gid_t group)
{
    svc_packet_chown_t      p;

    p.id        = SVCID_LFS_CHOWN;
    p.dirfd     = AT_FDCWD;
    p.path      = path;
    p.owner     = owner;
    p.group     = group;
    p.flags     = 0;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int dup(int fildes)
{
    svc_packet_dup_t        p;

    p.id        = SVCID_LFS_DUP;
    p.fildes    = fildes;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int dup2(int fildes, int fildes2)
{
    svc_packet_dup2_t       p;

    p.id        = SVCID_LFS_DUP2;
    p.fildes    = fildes;
    p.fildes2   = fildes2;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fchdir(int32_t fildes)
{
    svc_packet_fchdir_t     p;

    p.id        = SVCID_LFS_FCHDIR;
    p.fildes    = fildes;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fchmod(int32_t fildes, mode_t mode)
{
    svc_packet_fchmod_t     p;

    p.id        = SVCID_LFS_FCHMOD;
    p.fildes    = fildes;
    p.mode      = mode;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fchown(int32_t fildes, uid_t owner, gid_t group)
{
    svc_packet_fchown_t     p;

    p.id        = SVCID_LFS_FCHOWN;
    p.fildes    = fildes;
    p.owner     = owner;
    p.group     = group;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int fcntl(int fildes, int cmd,...)
{
    svc_packet_fcntl_t      p;
    va_list ap;
    va_start(ap, cmd);
    int arg = va_arg(ap, int);

    p.id        = SVCID_LFS_FCNTL;
    p.fildes    = fildes;
    p.cmd       = cmd;
    p.arg       = arg;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fdatasync(int32_t fildes)
{
    svc_packet_fdatasync_t      p;

    p.id        = SVCID_LFS_FDATASYNC;
    p.fildes    = fildes;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fstatvfs(int32_t fildes, struct statvfs *buf)
{
    svc_packet_fstatvfs_t       p;

    p.id        = SVCID_LFS_FSTATVFS;
    p.fildes    = fildes;
    p.buf       = buf;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fsync(int32_t fildes)
{
    svc_packet_fsync_t      p;

    p.id        = SVCID_LFS_FSYNC;
    p.fildes    = fildes;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

char* getcwd(char *buf, size_t size)
{
    svc_packet_getcwd_t     p;
    char    *cwd;

    p.id        = SVCID_LFS_GETCWD;
    p.buf       = buf;
    p.size      = size;
    p.cwd       = &cwd;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return *p.cwd;
}

int32_t ioctl(int32_t fildes, int32_t request,...)
{
    svc_packet_ioctl_t      p;
    va_list ap;
    va_start(ap, request);
    void* arg = va_arg(ap, void*);

    p.id        = SVCID_LFS_IOCTL;
    p.fildes    = fildes;
    p.request   = request;
    p.arg       = arg;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t link(const char *path1, const char *path2)
{
    svc_packet_link_t       p;

    p.id        = SVCID_LFS_LINK;
    p.oldfd     = AT_FDCWD;
    p.path1     = path1;
    p.newfd     = AT_FDCWD;
    p.path2     = path2;
    p.flags     = 0;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t stat(const char *path, struct stat *buf)
{
    svc_packet_stat_t       p;

    p.id        = SVCID_LFS_STAT;
    p.dirfd     = AT_FDCWD;
    p.path      = path;
    p.buf       = buf;
    p.flags     = 0;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t statvfs(const char *path, struct statvfs *buf)
{
    svc_packet_statvfs_t        p;

    p.id        = SVCID_LFS_STATVFS;
    p.path      = path;
    p.buf       = buf;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int truncate(const char *path, off_t length)
{
    svc_packet_truncate_t       p;

    p.id        = SVCID_LFS_TRUNCATE;
    p.path      = path;
    p.length    = length;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

char* realpath(const char *file_name, char *resolved_name)
{
    svc_packet_realpath_t       p;
    char    *name;
    char    *temp = NULL;

    /* Allocate memory for resolved path */
    if (resolved_name == NULL)
    {
        temp = malloc(PATH_MAX);
        resolved_name = temp;
    }

    p.id            = SVCID_LFS_REALPATH;
    p.file_name     = file_name;
    p.resolved_name = resolved_name;
    p.name          = &name;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());

    /* Release memory on failure */
    if ((name == NULL) && (temp != NULL))
    {
        free(temp);
    }

    return *p.name;
}

mode_t umask(mode_t cmask)
{
    svc_packet_umask_t      p;

    p.id        = SVCID_LFS_UMASK;
    p.cmask     = cmask;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t utime(const char *path, const struct utimbuf *times)
{
    svc_packet_utime_t      p;

    p.id        = SVCID_LFS_UTIME;
    p.path      = path;
    p.times     = times;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

ssize_t pread(int32_t fildes, void *buf, size_t nbyte, off_t offset)
{
    svc_packet_pread_t  p;
    size_t  len = nbyte; /* Requested/Returned length */

    p.id        = SVCID_LFS_PREAD;
    p.fildes    = fildes;
    p.buf       = buf;
    p.nbyte     = &len;
    p.offset    = offset;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return (ssize_t)len;
}

ssize_t pwrite(int32_t fildes, const void *buf, size_t nbyte, off_t offset)
{
    svc_packet_pwrite_t p;
    size_t  len = nbyte; /* Requested/Returned length */

    p.id        = SVCID_LFS_PWRITE;
    p.fildes    = fildes;
    p.buf       = buf;
    p.nbyte     = &len;
    p.offset    = offset;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return (ssize_t)len;
}

ssize_t readv(int32_t fildes, const struct iovec *iov, int32_t iovcnt)
{
    svc_packet_readv_t  p;
    ssize_t len = 0; /* Returned length */

    p.id        = SVCID_LFS_READV;
    p.fildes    = fildes;
    p.iov       = iov;
    p.iovcnt    = iovcnt;
    p.rbyte     = &len;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return (ssize_t)len;
}

ssize_t writev(int32_t fildes, const struct iovec *iov, int32_t iovcnt)
{
    svc_packet_writev_t p;
    ssize_t len = 0; /* Returned length */

    p.id        = SVCID_LFS_WRITEV;
    p.fildes    = fildes;
    p.iov       = iov;
    p.iovcnt    = iovcnt;
    p.rbyte     = &len;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return (ssize_t)len;
}

int32_t rmdir(const char *path)
{
    svc_packet_rmdir_t      p;

    p.id        = SVCID_LFS_RMDIR;
    p.path      = path;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
{
    svc_packet_select_t     p;

    p.id        = SVCID_LFS_SELECT;
    p.nfds      = nfds;
    p.readfds   = readfds;
    p.writefds  = writefds;
    p.errorfds  = errorfds;
    p.timeout   = timeout;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int pselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, const struct timespec *timeout, const sigset_t *sigmask)
{
    svc_packet_pselect_t        p;

    p.id        = SVCID_LFS_PSELECT;
    p.nfds      = nfds;
    p.readfds   = readfds;
    p.writefds  = writefds;
    p.errorfds  = errorfds;
    p.timeout   = timeout;
    p.sigmask   = sigmask;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

struct dirent* __readdir(DIR* dir, struct dirent* ent)
{
    svc_packet_readdir_t        p;
    struct dirent   *dirent;

    p.id        = SVCID_LFS_READDIR;
    p.dir       = dir;
    p.ent       = ent;
    p.dirent    = &dirent;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return *p.dirent;
}

int32_t mkdir(const char *path, mode_t mode)
{
    svc_packet_mkdir_t  p;

    p.id        = SVCID_LFS_MKDIR;
    p.dirfd     = AT_FDCWD;
    p.path      = path;
    p.mode      = mode;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t rename(const char* old, const char* new)
{
    svc_packet_rename_t p;

    p.id        = SVCID_LFS_RENAME;
    p.oldfd     = AT_FDCWD;
    p.old       = old;
    p.newfd     = AT_FDCWD;
    p.new       = new;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t openat(int32_t dirfd, const char *path, int32_t oflag,...)
{
    svc_packet_open_t   p;
    va_list ap;
    va_start(ap, oflag);
    mode_t mode = va_arg(ap, mode_t);

    p.id        = SVCID_LFS_OPEN;
    p.dirfd     = dirfd;
    p.path      = path;
    p.oflag     = oflag;
    p.mode      = mode;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t mkdirat(int32_t dirfd, const char *path, mode_t mode)
{
    svc_packet_mkdir_t  p;

    p.id        = SVCID_LFS_MKDIR;
    p.dirfd     = dirfd;
    p.path      = path;
    p.mode      = mode;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t linkat(int32_t oldfd, const char *path1, int32_t newfd, const char *path2, int32_t flags)
{
    svc_packet_link_t       p;

    p.id        = SVCID_LFS_LINK;
    p.oldfd     = oldfd;
    p.path1     = path1;
    p.newfd     = newfd;
    p.path2     = path2;
    p.flags     = flags;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t unlinkat(int32_t dirfd, const char *path, int32_t flags)
{
    svc_packet_unlink_t p;

    p.id        = SVCID_LFS_UNLINK;
    p.dirfd     = dirfd;
    p.path      = path;
    p.flags     = flags;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t renameat(int32_t oldfd, const char* old, int32_t newfd, const char* new)
{
    svc_packet_rename_t p;

    p.id        = SVCID_LFS_RENAME;
    p.oldfd     = oldfd;
    p.old       = old;
    p.newfd     = newfd;
    p.new       = new;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fchmodat(int32_t dirfd, const char *path, mode_t mode, int32_t flags)
{
    svc_packet_chmod_t      p;

    p.id        = SVCID_LFS_CHMOD;
    p.dirfd     = dirfd;
    p.path      = path;
    p.mode      = mode;
    p.flags     = flags;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fchownat(int32_t dirfd, const char *path, uid_t owner, gid_t group, int32_t flags)
{
    svc_packet_chown_t      p;

    p.id        = SVCID_LFS_CHOWN;
    p.dirfd     = dirfd;
    p.path      = path;
    p.owner     = owner;
    p.group     = group;
    p.flags     = flags;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t faccessat(int32_t dirfd, const char *path, int32_t amode, int32_t flags)
{
    svc_packet_access_t     p;

    p.id        = SVCID_LFS_ACCESS;
    p.dirfd     = dirfd;
    p.path      = path;
    p.amode     = amode;
    p.flags     = flags;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t fstatat(int32_t dirfd, const char *path, struct stat *buf, int32_t flags)
{
    svc_packet_stat_t       p;

    p.id        = SVCID_LFS_STAT;
    p.dirfd     = dirfd;
    p.path      = path;
    p.buf       = buf;
    p.flags     = flags;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

void sync(void)
{
    svc_packet_sync_t   p;

    p.id        = SVCID_LFS_SYNC;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t posix_fallocate(int32_t fd, off_t offset, off_t len)
{
    svc_packet_posix_fallocate_t    p;

    p.id        = SVCID_LFS_POSIX_FALLOCATE;
    p.fildes    = fd;
    p.offset    = offset;
    p.length    = len;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t posix_devctl(int32_t fno, int32_t dcmd, void* data, size_t size, int32_t* info)
{
    svc_packet_devctl_t p;
START_TIMER
    p.id        = SVCID_LFS_DEVCTL;
    p.fildes    = fno;
    p.dcmd      = dcmd;
    p.data      = data;
    p.size      = size;
    p.info      = info;
    int32_t ret = (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
STOP_TIMER
    return ret;
}

int32_t devctl(int32_t fno, int32_t dcmd, void* data, size_t size, int32_t* info)
{
    return posix_devctl(fno, dcmd, data, size, info);
}

int32_t utimes(const char * path, const struct timeval tv[2])
{
    struct timespec * tsp = NULL_PNTR;
    struct timespec ts[2];
    if (tv != NULL_PNTR)
    {
        if (((uintptr_t)tv & (sizeof(tv[0].tv_usec)-1)) != 0)
        {
         /* Kernel will check alignment */
            tsp = (struct timespec*)tv;
        }
        else
        {
            ts[0].tv_nsec = (long)tv[0].tv_usec;
            if ((tv[0].tv_usec != UTIME_OMIT) && (tv[0].tv_usec != UTIME_NOW))
            {
                TIMEVAL_TO_TIMESPEC(&tv[0], &ts[0]);
            }
            ts[1].tv_nsec = (long)tv[1].tv_usec;
            if ((tv[1].tv_usec != UTIME_OMIT) && (tv[1].tv_usec != UTIME_NOW))
            {
                TIMEVAL_TO_TIMESPEC(&tv[1], &ts[1]);
            }
            tsp = ts;
        }
    }

    return utimensat(AT_FDCWD, path, tsp, 0);
}

int32_t utimensat(int32_t dirfd, const char * path, const struct timespec tv[2], int32_t flags)
{
    return (int32_t) call_svc(
        &(svc_packet_utimens_t) { .id = SVCID_LFS_UTIMENS, .dirfd = dirfd, .path = path, .tv = tv, .flags = flags, },
        GET_CALLER_ADDRESS());
}

int32_t futimes(int32_t fd, const struct timeval tv[2])
{
    struct timespec * tsp = NULL_PNTR;
    struct timespec ts[2];
    if (tv != NULL_PNTR)
    {
        if (((uintptr_t)tv & (sizeof(tv[0].tv_usec)-1)) != 0)
        {
         /* Kernel will check alignment */
            tsp = (struct timespec*)tv;
        }
        else
        {
            ts[0].tv_nsec = (long)tv[0].tv_usec;
            if ((tv[0].tv_usec != UTIME_OMIT) && (tv[0].tv_usec != UTIME_NOW))
            {
                TIMEVAL_TO_TIMESPEC(&tv[0], &ts[0]);
            }
            ts[1].tv_nsec = (long)tv[1].tv_usec;
            if ((tv[1].tv_usec != UTIME_OMIT) && (tv[1].tv_usec != UTIME_NOW))
            {
                TIMEVAL_TO_TIMESPEC(&tv[1], &ts[1]);
            }
            tsp = ts;
        }
    }

    return futimens(fd, tsp);
}

int32_t futimens(int32_t fd, const struct timespec tv[2])
{
    return (int32_t) call_svc(
        &(svc_packet_futimens_t) { .id = SVCID_LFS_FUTIMENS, .fildes = fd, .tv = tv, },
        GET_CALLER_ADDRESS());
}

int32_t call_svc_mkfs(const char *name, int32_t pflags, struct lfs_cmd_channel *uchannel)
{
    svc_packet_mkfs_t p;

    p.id = SVCID_LFS_MKFS;
    p.name = name;
    p.pflags = pflags;
    p.uchannel = uchannel;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t call_svc_rmfs(const char *name)
{
    svc_packet_rmfs_t p;

    p.id = SVCID_LFS_RMFS;
    p.name = name;
    return call_svc((void*)&p, GET_CALLER_ADDRESS());
}
