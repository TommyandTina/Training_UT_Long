/*
============================================================================
DESCRIPTION
PRR low level driver API
============================================================================
                            C O P Y R I G H T
============================================================================
                      Copyright (c) 2019 - 2021
                                  by
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: to be used as sample S/W

DISCLAIMER
This software is supplied by Renesas Electronics Corporation and is only
intended for use with Renesas products. No other uses are authorized. This
software is owned by Renesas Electronics Corporation and is protected under
all applicable laws, including copyright laws.
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
Renesas reserves the right, without notice, to make changes to this software
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the
following link:
http://www.renesas.com/disclaimer *
Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*/

#include <stdarg.h>
#include "rcar-xos/osal/r_osal.h"

#define PRINT_RESOURCE_ID_BSP      0x8000U
#define MUTEX_PRINT       (PRINT_RESOURCE_ID_BSP   + 98U)
#define LOC_PRINT_MUTEX_TIMEOUT   (3000U)


#define R_USE_LOG 0 /* Set here 1 when use log */

#if (R_USE_LOG)
static osal_mutex_handle_t loc_printfMutex = NULL;
#endif

void R_PRINT_Log(const char *fmt, ...)
{
#if (R_USE_LOG)
    va_list args;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == loc_printfMutex)
    {
        osal_ret = R_OSAL_ThsyncMutexCreate(MUTEX_PRINT, &loc_printfMutex);
        if (OSAL_RETURN_OK != osal_ret)
        {
            printf("R_PRINT_Log MutexCreate Error osal_ret = %d\n", osal_ret);
        }
    }
    va_start(args, fmt);
    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(loc_printfMutex, LOC_PRINT_MUTEX_TIMEOUT);
    if (OSAL_RETURN_OK == osal_ret)
    {
        vprintf(fmt, args);
        osal_ret = R_OSAL_ThsyncMutexUnlock(loc_printfMutex);
        if (OSAL_RETURN_OK != osal_ret)
        {
            printf("R_PRINT_Log MutexUnlock Error osal_ret = %d\n", osal_ret);
        }
    }
    else
    {
        printf("R_PRINT_Log MutexLock Error osal_ret = %d\n", osal_ret);
    }
    va_end(args);
#endif
}
