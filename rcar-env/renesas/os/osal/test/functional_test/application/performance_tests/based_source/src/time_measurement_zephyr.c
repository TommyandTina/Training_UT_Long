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
* File Name :    time_measurement_zephyr.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Time measurement function for Zephyr OS
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     05.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Global parameter */
uint32_t g_timepoint_begin;
uint32_t g_timepoint_end;

void get_time_begin()
{
    __asm__ volatile("mrc   p15, 0, %0, c9, c13, 0" : "=r" (g_timepoint_begin));
}

void get_time_end()
{
    __asm__ volatile("mrc   p15, 0, %0, c9, c13, 0" : "=r" (g_timepoint_end));
}

void get_time_diff()
{
    g_diff_time = ((uint64_t)(g_timepoint_end - g_timepoint_begin) * SEC_TO_NSEC / CPU_CLOCK);
}

void get_time_initialize()
{
    uint32_t reg;
    reg = PMCNTENSET_C;
    __asm__ volatile("mcr   p15, 0, %0, c9, c12, 1" :: "r" (reg));
    reg = 0;
    __asm__ volatile("mrc   p15, 0, %0, c9, c12, 0" : "=r" (reg));
    reg |= (PMCR_C | PMCR_E);
    __asm__ volatile("mcr   p15, 0, %0, c9, c12, 0" :: "r" (reg));
    __asm__ volatile("mrc   p15, 0, %0, c9, c13, 0" : "=r" (reg));
}
