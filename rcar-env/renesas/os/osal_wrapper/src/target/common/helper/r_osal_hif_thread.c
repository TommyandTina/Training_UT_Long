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
* File Name :    r_osal_hif_thread.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Thread Related Helper Functions
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
 * XOS3_OSAL_NOS_CD_CD_TH_031
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_HIF_ThreadSleepForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_ThreadSleepForTimePeriod(osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_ThreadSleep(&time_stamp);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_HIF_ThreadSleepForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_TH_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_TH_032
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_HIF_ThreadSleepUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_ThreadSleepUntilTimeStamp(const st_osal_time_t* const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    R_OSAL_HIF_ClockTimeRoundupByTimeStamp(p_time_stamp, &time_stamp);
    osal_ret = R_OSAL_OS_ThreadSleep(&time_stamp);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_HIF_ThreadSleepUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_TH_013]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
