/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.   Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

/**
 * @file        process.h
 *
 * @brief       Process spawning and related routines
 *
 * @important   Included from unistd.h so must be POSIX compliant
 */

#ifndef _SYS_PROCESS_H_INCLUDED
#define _SYS_PROCESS_H_INCLUDED

#ifndef __PLATFORM_H_INCLUDED
#include <sys/platform.h>
#endif

#if defined(__PID_T)
typedef __PID_T		pid_t;
#undef __PID_T
#endif

#if defined(__EXT_PCDOS)
#define P_WAIT      0
#define P_NOWAIT    1
#define P_OVERLAY   2
#define P_NOWAITO   3
#endif

__BEGIN_DECLS

#if defined(__EXT_POSIX1_198808)
// void             _exit( int __status ) __attribute__((__noreturn__));
// extern int execl( const char *__path, const char *__arg0, ... )  __attribute__((sentinel(0)));
// extern int execle( const char *__path, const char *__arg0, ... ) __attribute__((sentinel(1)));
// extern int execlp( const char *__file, const char *__arg0, ... ) __attribute__((sentinel(0)));
extern int execv( const char *__path, char * const __argv[] );
extern int execve( const char *__path, char * const __argv[], char * const __envp[] );
extern int execvp( const char *__file, char * const __argv[] );
extern pid_t    fork(void);
extern pid_t    getpgrp( void );
extern pid_t    getpid(void);
extern pid_t    getppid( void );
extern int      setpgid( pid_t __pid, pid_t __pgroupid );
#endif

#if defined(__EXT_XOPEN_EX) && !defined(vfork)
// vfork() is no longer supported.
// It is currently implmenented as a simple wrapper for fork(), and will be
// removed altogether in a future release.
// static inline pid_t __attribute__((deprecated))
vfork(void)
{
    return fork();
}
#endif

/*
 *  Prototypes for non-POSIX functions
 */

#if defined(__EXT_QNX)
// void             __exit( int __status ) __attribute__((__noreturn__));
// extern int execlpe( const char *__file, const char *__arg0, ... ) __attribute__((sentinel(1)));
extern int execvpe( const char *__file, char * const __argv[], char * const __envp[] );
extern int gettid(void);
extern char *_cmdname(char *__name);
#endif

#if defined(__EXT_PCDOS)
// extern int spawnl(int __mode, const char *__path, const char *__arg0,...)   __attribute__((sentinel(0)));
// extern int spawnle(int __mode, const char *__path, const char *__arg0,...)  __attribute__((sentinel(1)));
// extern int spawnlp(int __mode, const char *__path, const char *__arg0,...)  __attribute__((sentinel(0)));
// extern int spawnlpe(int __mode, const char *__path, const char *__arg0,...) __attribute__((sentinel(1)));
extern int spawnv(int __mode, const char *__path, char * const __argv[]);
extern int spawnve(int __mode, const char *__path, char * const __argv[], char * const __envp[]);
extern int spawnvp(int __mode, const char *__path, char * const __argv[]);
extern int spawnvpe(int __mode, const char *__path, char * const __argv[], char * const __envp[]);
#endif

__END_DECLS

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/sys/process.h $ $Rev: 912243 $")
#endif
