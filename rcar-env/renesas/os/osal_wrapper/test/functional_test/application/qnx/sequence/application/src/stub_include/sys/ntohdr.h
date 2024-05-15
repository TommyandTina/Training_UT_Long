/*
 * $QNXLicenseC:
 * Copyright 2019, QNX Software Systems. All Rights Reserved.
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

/*
 * This file is only used to include NTO headers in host projects
 */

#ifndef __NTOHDR_H_INCLUDED
#define __NTOHDR_H_INCLUDED

#ifndef _NTO_HDR_PIECE_
#define _NTO_HDR_PIECE_(x) x
#endif

/*<STAN_MACRO1,STAN_MACRO5>*/
#if defined(__X86__)
    #define _NTO_CPU_HDR_DIR_(h)   x86/h
#elif defined(__ARM__)
    #define _NTO_CPU_HDR_DIR_(h)   arm/h
#elif defined(__X86_64__)
	#define _NTO_CPU_HDR_DIR_(h)   x86_64/h
#elif defined(__aarch64__)
	#define _NTO_CPU_HDR_DIR_(h)   aarch64/h

/* New CPU types go here */

#elif defined(__QNXNTO__)
    #error not configured for CPU
#else
    /* partial support only - cross hosted development targeting qnx6 */
    #define _NTO_CPU_HDR_DIR_(h)   unknown_cpu/h
#endif
/*</STAN_MACRO1,STAN_MACRO5>*/

#ifndef _NTO_HDR_
    #ifdef _NTO_HDR_DIR_
        #define _NTO_HDR_(hdr)  <_NTO_HDR_PIECE_(_NTO_HDR_DIR_)_NTO_HDR_PIECE_(hdr)>
    #else
        #define _NTO_HDR_(hdr)  <hdr>
    #endif
#endif

#ifndef _NTO_CPU_HDR_
    #ifdef _NTO_HDR_DIR_
		#define _NTO_CPU_HDR_(hdr)  <_NTO_HDR_PIECE_(_NTO_HDR_DIR_)_NTO_CPU_HDR_DIR_(hdr)>
    #else
		#define _NTO_CPU_HDR_(hdr)  <_NTO_CPU_HDR_DIR_(hdr)>
    #endif
#endif

#ifndef _NTO_DIR_CPU_HDR_
    #ifdef _NTO_HDR_DIR_
       #define _NTO_DIR_CPU_HDR_(dir, hdr)  <_NTO_HDR_PIECE_(_NTO_HDR_DIR_)dir/_NTO_CPU_HDR_DIR_(hdr)>
    #else
       #define _NTO_DIR_CPU_HDR_(dir, hdr)  <dir/_NTO_CPU_HDR_DIR_(hdr)>
    #endif
#endif

#endif // __NTOHDR_H_INCLUDED

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/c/public/sys/ntohdr.h $ $Rev: 897782 $")
#endif
