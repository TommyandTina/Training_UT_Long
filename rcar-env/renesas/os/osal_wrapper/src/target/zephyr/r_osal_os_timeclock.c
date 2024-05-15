/***********************************************************************************************************************
* Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Clock and Time Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <device.h>
#include <devicetree.h>

#include "target/zephyr/r_osal_os_type.h"

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
    struct timespec         initial_time;
    uint32_t                cycles_per_sec;
    uint32_t                initial_cycle;
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
 * XOS3_OSAL_ZEPHYR_CD_CD_CLK_001
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

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* Get the clock cycle per sec from the device tree information. */
    gs_osal_clock_info.cycles_per_sec = DT_PROP(DT_NODELABEL(cmt0), clock_frequency);
    
    gs_osal_clock_info.initial_cycle = k_cycle_get_32();
    
    os_ret = clock_gettime(CLOCK_REALTIME, &gs_osal_clock_info.initial_time);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }
    
    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ClockTimeInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_CLK_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_CLK_002
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
        os_ret = clock_gettime(CLOCK_REALTIME, &time_stamp);
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
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_CLK_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_CLK_003
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
    uint32_t         current_cycle;
    uint32_t         diff_cycle;
    uint32_t         remain_diff_cycle;
    uint32_t         diff_tv_sec;
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
        current_cycle = k_cycle_get_32();
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
            time_stamp.tv_sec  = gs_osal_clock_info.initial_time.tv_sec + (time_t)diff_tv_sec;
            time_stamp.tv_nsec = gs_osal_clock_info.initial_time.tv_nsec + (long)diff_tv_nsec; /* PRQA S 5209 # System is requesting a long, so it cannot be changed. */

            if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
            {
                /* round up */
                /* PRQA S 4393 1 # QAC false indication (CCT change). */
                time_stamp.tv_sec  += (time_t)time_stamp.tv_nsec / (time_t)(OSAL_MAX_TV_NSEC + 1);
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
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_CLK_007]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
