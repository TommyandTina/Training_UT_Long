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
* Description :  Clock and Time Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <sys/syspage.h>
#include <sys/neutrino.h>
#include <inttypes.h>
#include <pthread.h>

#include "target/qnx/r_osal_os_type.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private data types
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_clock_info_t
 * @brief structure to clock timer information.
 **********************************************************************************************************************/
typedef struct
{
    struct timespec         initial_monotonic_time;
    uint64_t                cycles_per_sec;
    uint64_t                initial_cycle;
}st_osal_clock_info_t;

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * @var gs_osal_clock_info
 * @brief Initial clock timer information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_clock_info_t gs_osal_clock_info =
{
    {0, 0}, 0, 0
};

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_CLK_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ClockTimeInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ClockTimeInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec abstime;
    struct timespec res_time = {0, 0};

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0488,0314,0316 1 # It points to a function macro in the system header and cannot be changed. */
    gs_osal_clock_info.cycles_per_sec = SYSPAGE_ENTRY(qtime)->cycles_per_sec;

    os_ret = clock_gettime(CLOCK_MONOTONIC, &abstime);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if ((abstime.tv_nsec % OSAL_MSECTONSEC) != 0L)
        {
            /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
            abstime.tv_nsec = (long)(((abstime.tv_nsec / OSAL_MSECTONSEC) + 1) * OSAL_MSECTONSEC);
            if (abstime.tv_nsec > OSAL_MAX_TV_NSEC)
            {
                abstime.tv_sec++;
                abstime.tv_nsec = abstime.tv_nsec - (OSAL_MAX_TV_NSEC + 1);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        os_ret = clock_getres(CLOCK_MONOTONIC, &res_time);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            abstime.tv_nsec += res_time.tv_nsec;
            if (OSAL_MAX_TV_NSEC < abstime.tv_nsec)
            {
                /* PRQA S 4393 1 # QAC false indication (CCT change). */
                abstime.tv_sec  += (time_t)(abstime.tv_nsec / (OSAL_MAX_TV_NSEC + 1));
                abstime.tv_nsec  = abstime.tv_nsec % (OSAL_MAX_TV_NSEC + 1);
            }
            else
            {
                /* Do nothing */
            }
        }

        /* PRQA S 5209,2855 1 # System is requesting an int, so it cannot be changed. */
        os_ret = clock_nanosleep(CLOCK_MONOTONIC, (int)TIMER_ABSTIME, &abstime, NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 1021,1006 2 # It points to a function macro in the system header and cannot be changed. */
            /* PRQA S 0602 1 # QAC false indication (CCT change). */
            gs_osal_clock_info.initial_cycle = ClockCycles();
            os_ret = clock_gettime(CLOCK_MONOTONIC, &gs_osal_clock_info.initial_monotonic_time);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if (0 != os_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ClockTimeInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_CLK_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_CLK_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(st_osal_time_t *p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* output time_stamp */
                p_time_stamp->tv_sec  = time_stamp.tv_sec;
                p_time_stamp->tv_nsec = (int32_t)time_stamp.tv_nsec;
            }
        }
    }   /* end of if (NULL == p_time_stamp). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_CLK_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_CLK_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ClockTimeGetHighResoTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ClockTimeGetHighResoTimeStamp(st_osal_time_t *p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret = OSAL_RETURN_OK;
    uint64_t         current_cycle;
    uint64_t         diff_cycle;
    uint64_t         remain_diff_cycle;
    uint64_t         diff_tv_sec;
    uint64_t         diff_tv_nsec;
    struct timespec  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 1021,1006 2 # It points to a function macro in the system header and cannot be changed. */
        /* PRQA S 0602 1 # QAC false indication (CCT change). */
        current_cycle = ClockCycles();
        diff_cycle    = current_cycle - gs_osal_clock_info.initial_cycle;
        if (0U == gs_osal_clock_info.cycles_per_sec)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Separate into sec and nsec */
            diff_tv_sec       = diff_cycle / gs_osal_clock_info.cycles_per_sec;
            remain_diff_cycle = diff_cycle % gs_osal_clock_info.cycles_per_sec;
            diff_tv_nsec      = (remain_diff_cycle * (uint64_t)OSAL_SECTONSEC) / gs_osal_clock_info.cycles_per_sec;

            /* convert to time_stamp */
            time_stamp.tv_sec  = gs_osal_clock_info.initial_monotonic_time.tv_sec + (time_t)diff_tv_sec;
            /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
            time_stamp.tv_nsec = gs_osal_clock_info.initial_monotonic_time.tv_nsec + (long)diff_tv_nsec;

            if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
            {
                /* round up */
                /* PRQA S 4393 1 # QAC false indication (CCT change). */
                time_stamp.tv_sec  += (time_t)(time_stamp.tv_nsec / (OSAL_MAX_TV_NSEC + 1));
                time_stamp.tv_nsec  = time_stamp.tv_nsec % (OSAL_MAX_TV_NSEC + 1);
            }
            else
            {
                /* Do nothing */
            }
            p_time_stamp->tv_sec  = time_stamp.tv_sec;
            p_time_stamp->tv_nsec = (int32_t)time_stamp.tv_nsec;
        }   /* end of if(0 == gs_osal_clock_info.cycles_per_sec). */
    }   /* end of if (NULL == p_time_stamp). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ClockTimeGetHighResoTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_CLK_007]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
