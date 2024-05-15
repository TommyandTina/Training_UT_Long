/*
    Copyright (C) 2019 eSOL Co.,Ltd. All rights reserved.

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
[ lfa_sysdepend.h ] - libmmap system dependance definition
****************************************************************************/

#ifndef LFA_SYSDEPEND_H
#define LFA_SYSDEPEND_H

#include <stdint.h>
#if defined(__linux)
#include <stdatomic.h>
#else
#include <mcos/atomic.h>
#endif

#define LFA_WORD_BITS       (8 * __SIZEOF_POINTER__)

/* should really use sysconf(_SC_PAGE_SIZE), but that's not currently available */
#define LFA_MMU_PAGE_SIZE   4096U

/* compiler attributes */
#define LFA_INLINE          static inline __attribute__((always_inline))
#define LFA_WEAK            __attribute__((weak))
#define LFA_PURE            __attribute__((pure))
#define LFA_CALIGN          __attribute__((aligned(64)))
#define LFA_ALIAS(lfa_fn)   __attribute__ ((alias (#lfa_fn)))

/* count leading zeros */
#if LFA_WORD_BITS == 32
    #define LFA_CLZ_(word)      __builtin_clzl(word)
#else
    #define LFA_CLZ_(word)      __builtin_clzll(word)
#endif
#define LFA_CLZ(word)           (uint_fast8_t)(((uintptr_t)(word) == 0U) ? LFA_WORD_BITS : LFA_CLZ_(word))

/* find first set */
#if LFA_WORD_BITS == 32
    #define LFA_FFS_(word)      __builtin_ffsl(word)
#else
    #define LFA_FFS_(word)      __builtin_ffsll(word)
#endif
#define LFA_FFS(word)           (uint_fast8_t)LFA_FFS_(word)

#endif /* LFA_SYSDEPEND_H */
