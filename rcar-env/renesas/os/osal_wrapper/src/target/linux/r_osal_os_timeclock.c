/***********************************************************************************************************************
* Copyright [2022-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_os_timeclock.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for Clock and Time
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <errno.h>
#include <pthread.h>
#include "target/linux/r_osal_os_types.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_CLK_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(st_osal_time_t* p_time_stamp)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    struct timespec abstime;

    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = clock_gettime(CLOCK_REALTIME, &abstime);
        if ((0 != linux_ret) || (OSAL_MAX_TV_NSEC < abstime.tv_nsec))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_time_stamp->tv_sec  = abstime.tv_sec;
        p_time_stamp->tv_nsec = (int32_t)abstime.tv_nsec;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_CLK_005]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
