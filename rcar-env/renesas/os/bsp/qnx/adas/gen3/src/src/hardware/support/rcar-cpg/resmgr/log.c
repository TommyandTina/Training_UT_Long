/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#include <stdio.h>
#include <stdarg.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include "proto.h"

void cpg_slogf(const int vlevel, const char* const fmt, ...)
{
    int      status = 0;
    va_list  arg;

    if( vlevel <= verbose)
    {
        va_start(arg, fmt);
        status = vslogf(_SLOG_SETCODE(_SLOGC_TEST, 0), vlevel, fmt, arg);
        va_end(arg);

        /* If slogger not running, send output to stderr */
        if( -1 == status)
        {
            va_start(arg, fmt);
            status = vfprintf(stderr, fmt, arg);
            va_end(arg);
        }
    }
}
