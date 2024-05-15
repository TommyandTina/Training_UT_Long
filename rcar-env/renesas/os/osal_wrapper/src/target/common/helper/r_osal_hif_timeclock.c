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
* File Name :    r_osal_hif_timeclock.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Clock and Time Related Helper Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "target/common/helper/r_osal_helper_if.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_CLK_031
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_HIF_ClockTimeRoundupByTimeStamp()
***********************************************************************************************************************/
void R_OSAL_HIF_ClockTimeRoundupByTimeStamp(const st_osal_time_t* const p_time_stamp_in,
                                            st_osal_time_t * p_time_stamp_out)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_time_stamp_out->tv_sec  = p_time_stamp_in->tv_sec;
    p_time_stamp_out->tv_nsec = p_time_stamp_in->tv_nsec;

    if (0L != (p_time_stamp_out->tv_nsec % OSAL_MSECTONSEC))
    {
        p_time_stamp_out->tv_nsec = (int32_t)(((p_time_stamp_out->tv_nsec / OSAL_MSECTONSEC) + 1) * OSAL_MSECTONSEC);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_MAX_TV_NSEC < p_time_stamp_out->tv_nsec)
    {
        /* PRQA S 4393 1 # System is requesting a time_t, so it cannot be changed. */
        p_time_stamp_out->tv_sec  += (time_t)(p_time_stamp_out->tv_nsec / (OSAL_MAX_TV_NSEC + 1));
        p_time_stamp_out->tv_nsec  = (int32_t)(p_time_stamp_out->tv_nsec % (OSAL_MAX_TV_NSEC + 1));
    }
    else
    {
        /* Do nothing */
    }

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_HIF_ClockTimeRoundupByTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_CLK_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_CLK_032
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(osal_milli_sec_t time_period,
                                                           st_osal_time_t * p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(&time_stamp);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4393 1 # System is requesting a time_t, so it cannot be changed. */
        time_stamp.tv_sec  += (time_t)(time_period / OSAL_SECTOMSEC);
        time_stamp.tv_nsec += (int32_t)((time_period % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);
        if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
        {
            /* PRQA S 4393 1 # System is requesting a time_t, so it cannot be changed. */
            time_stamp.tv_sec  += (time_t)(time_stamp.tv_nsec / (OSAL_MAX_TV_NSEC + 1));
            time_stamp.tv_nsec  = (int32_t)(time_stamp.tv_nsec % (OSAL_MAX_TV_NSEC + 1));
        }
        else
        {
            /* Do nothing */
        }
        R_OSAL_HIF_ClockTimeRoundupByTimeStamp(&time_stamp, p_time_stamp);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_CLK_008]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
