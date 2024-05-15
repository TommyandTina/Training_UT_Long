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
* File Name :    time_measurement_windows.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Time measurement function for Windows OS
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     05.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"
#include <profileapi.h>

/* Global parameter */
LARGE_INTEGER g_timepoint_begin;
LARGE_INTEGER g_timepoint_end;
LARGE_INTEGER frequency;

void get_time_begin()
{
    if (!QueryPerformanceCounter(&g_timepoint_begin))
    {
        LOG_ERROR("Get begin timepoint failed\n");
    }
}

void get_time_end()
{
    if (!QueryPerformanceCounter(&g_timepoint_end))
    {
        LOG_ERROR("Get end timepoint failed\n");
    }
}

void get_time_diff()
{
    g_diff_time = (uint64_t)(g_timepoint_end.QuadPart - g_timepoint_begin.QuadPart) * SEC_TO_NSEC / frequency.QuadPart;
}

void get_time_initialize()
{
    if(!QueryPerformanceFrequency(&frequency))
    {
        LOG_ERROR("Get frequency failed\n");
    }
}
