/*
 * $QNXLicenseC:
 * Copyright 2019, QNX Software Systems. All Rights Reserved.
 * Copyright 2022, Renesas Electronics Corporation
 *
 * You must obtain a written license from and pay applicable
 * license fees to QNX Software Systems before you may reproduce,
 * modify or distribute this software, or any work that includes
 * all or part of this software.   Free development licenses are
 * available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email
 * licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review
 * this entire file for other proprietary rights or license notices,
 * as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#ifndef _LOG_H
#define _LOG_H

#define LOGGING_MAX_VERBOSITY   (7)
#define LOGGING_MIN_VERBOSITY   (0)

#if defined(__QNXNTO__)

#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <stdio.h>

/**
 * @defgroup Log Log
 * @brief Various logging functions
 */

/**
 * @addtogroup Log
 * @{
 */

/** @brief The name of the program, which must be initialized before the logging macros are used */
extern char *prog_name;
extern int prog_name_len;
/**
 * @brief The verbosity level for the logger, which must be initialized before the logging macros are used
 * @details The verbosity levels match the severity levels in slogger. Setting the verbosity level will disable logging messages at higher levels.
 */
extern int log_verbosity;

/** @brief The slogger code for rcar cfd driver */
#define _SLOGC_RCAR_CANFD_MGR _SLOG_SETCODE(_SLOGC_PRIVATE_START + 555, 1)
#define LOG_COLON_SPACE ": "
#define LOG_COLON_SPACE_LEN 2

/**
 * @brief Generic logging function that writes messages to slogger
 * @param[in] severity The slogger severity level
 * @param[in] msg A printf format string
 * @param[in] vars A variable list of arguments for the format string
 */
#define LOGF(severity, msg, vars...) slogf(_SLOGC_RCAR_CANFD_MGR, severity, "%s: "msg, prog_name, ##vars )

/**
 * @brief Generic logging function that writes messages to slogger with a variable argument list
 * @details prog_name is assumed to be embedded in the va_list or the message string itself
 * @param[in] severity The slogger severity level
 * @param[in] msg A printf format string
 * @param[in] vars A variable list of arguments, of type va_list, for the format string
 */
#define VLOGF(severity, msg, var_list) \
    do { \
        if (log_verbosity >= severity) { \
            char new_msg[prog_name_len + LOG_COLON_SPACE_LEN + strlen(message) + 1]; \
            sprintf(new_msg, "%s%s%s", prog_name, LOG_COLON_SPACE, message); \
            vslogf(_SLOGC_RCAR_CANFD_MGR, severity, new_msg, var_list ); \
        } \
     } while (0)

/** @brief Log messages at the CRITICAL severity level */
#define LOG_CRITICAL(msg, vars...) do {if(log_verbosity >= _SLOG_CRITICAL) LOGF(_SLOG_CRITICAL, msg, ##vars);}while(0)
/** @brief Log messages at the ERROR severity level */
#define LOG_ERROR(msg, vars...) do {if(log_verbosity >= _SLOG_ERROR) LOGF(_SLOG_ERROR, msg, ##vars);}while(0)
/** @brief Log messages at the WARNING severity level */
#define LOG_WARNING(msg, vars...) do {if(log_verbosity >= _SLOG_WARNING) LOGF(_SLOG_WARNING, msg, ##vars);}while(0)
/** @brief Log messages at the NOTICE severity level */
#define LOG_NOTICE(msg, vars...) do {if(log_verbosity >= _SLOG_NOTICE) LOGF(_SLOG_NOTICE, msg, ##vars);}while(0)
/** @brief Log messages at the INFO severity level */
#define LOG_INFO(msg, vars...) do {if(log_verbosity >= _SLOG_INFO) LOGF(_SLOG_INFO, msg, ##vars);}while(0)
/** @brief Log messages at the DEBUG1 severity level */
#define LOG_DEBUG1(msg, vars...) do {if(log_verbosity >= _SLOG_DEBUG1) LOGF(_SLOG_DEBUG1, msg, ##vars);}while(0)
/** @brief Log messages at the DEBUG2 severity level */
#define LOG_DEBUG2(msg, vars...) do {if(log_verbosity >= _SLOG_DEBUG2) LOGF(_SLOG_DEBUG2, msg, ##vars);}while(0)

/** @} */

#else

#error "Logging not supported on this platform"

#endif

#endif /* _LOG_H */


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/can/rcar_cfd/canfd_log.h $ $Rev: 895758 $")
#endif

