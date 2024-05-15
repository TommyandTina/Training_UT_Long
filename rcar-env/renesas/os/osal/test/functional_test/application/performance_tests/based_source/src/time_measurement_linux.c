/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    time_measurement_linux.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Time measurement function for Linux OS
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     05.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"
#include <time.h>

/* Global parameter */
struct timespec g_timepoint_begin;
struct timespec g_timepoint_end;

void get_time_begin()
{
    if (0 != clock_gettime(CLOCK_MONOTONIC, &g_timepoint_begin))
    {
        LOG_ERROR("Get begin timepoint failed\n");
    }
}

void get_time_end()
{
    if (0 != clock_gettime(CLOCK_MONOTONIC, &g_timepoint_end))
    {
        LOG_ERROR("Get end timepoint failed\n");
    }
}

void get_time_diff()
{
    g_diff_time = (g_timepoint_end.tv_sec - g_timepoint_begin.tv_sec) * SEC_TO_NSEC;
    g_diff_time = (g_diff_time + (g_timepoint_end.tv_nsec - g_timepoint_begin.tv_nsec));
}

void get_time_initialize()
{
    /* Do nothing */
}
