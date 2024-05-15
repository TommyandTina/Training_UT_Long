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
* File Name :    r_osal_timeclock.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper TimeClock Manager
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include "target/common/r_osal_manager.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_CLK_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_ClockTimeGetTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ClockTimeGetTimeStamp(e_osal_clock_type_t clock_type,
                                             st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    e_osal_init_status_t    init_status;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    init_status = R_OSAL_RCFG_ClockTimeGetInitStatus();
    if (OSAL_MANAGER_STATE_INITIALIZED != init_status)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((OSAL_CLOCK_TYPE_HIGH_RESOLUTION != clock_type) && (OSAL_CLOCK_TYPE_HIGH_PRECISION != clock_type))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = R_OSAL_RCFG_ClockTimeGetTimeStamp(clock_type, p_time_stamp);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ClockTimeGetTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_CLK_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_CLK_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_ClockTimeCalculateTimeDifference()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ClockTimeCalculateTimeDifference(const st_osal_time_t *const p_time2,
                                                        const st_osal_time_t *const p_time1,
                                                        osal_nano_sec_t *const p_time_difference)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    e_osal_init_status_t    init_status;
    st_osal_time_t          diff_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    init_status = R_OSAL_RCFG_ClockTimeGetInitStatus();
    if (OSAL_MANAGER_STATE_INITIALIZED != init_status)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_time2) || (NULL == p_time1) || (NULL == p_time_difference))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    /* PRQA S 2996 1 # There is no problem because the time_t type is signed. */
    else if (((time_t)0L > p_time1->tv_sec) ||
             (0L > p_time1->tv_nsec) || (OSAL_MAX_TV_NSEC < p_time1->tv_nsec))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    /* PRQA S 2996 1 # There is no problem because the time_t type is signed. */
    else if (((time_t)0L > p_time2->tv_sec) ||
             (0L > p_time2->tv_nsec) || (OSAL_MAX_TV_NSEC < p_time2->tv_nsec))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        diff_time.tv_sec  = p_time2->tv_sec  - p_time1->tv_sec;
        diff_time.tv_nsec = p_time2->tv_nsec - p_time1->tv_nsec;
        *p_time_difference = ((osal_nano_sec_t)diff_time.tv_sec * (osal_nano_sec_t)OSAL_SECTONSEC) + diff_time.tv_nsec;
    }   /* end of if (OSAL_MANAGER_STATE_INITIALIZED != init_status) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ClockTimeCalculateTimeDifference()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_CLK_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_CLK_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_ClockTimeInitialize()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ClockTimeInitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    e_osal_init_status_t    init_status;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    init_status = R_OSAL_RCFG_ClockTimeGetInitStatus();
    if (OSAL_MANAGER_STATE_UNINITIALIZED != init_status)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        R_OSAL_RCFG_ClockTimeSetInitStatus(OSAL_MANAGER_STATE_INITIALIZED);
        osal_ret = R_OSAL_RCFG_ClockTimeControlInit();
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ClockTimeInitialize()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_CLK_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_CLK_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_ClockTimeDeinitialize()
***********************************************************************************************************************/
void R_OSAL_ClockTimeDeinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    R_OSAL_RCFG_ClockTimeSetInitStatus(OSAL_MANAGER_STATE_UNINITIALIZED);

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_ClockTimeDeinitialize()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_CLK_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_CLK_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_ClockTimeCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ClockTimeCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    e_osal_init_status_t    init_status;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    init_status = R_OSAL_RCFG_ClockTimeGetInitStatus();
    if (OSAL_MANAGER_STATE_INITIALIZED != init_status)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        R_OSAL_RCFG_ClockTimeSetInitStatus(OSAL_MANAGER_STATE_UNINITIALIZED);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ClockTimeCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_CLK_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_CLK_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_ClockTimeSetInit()
***********************************************************************************************************************/
void R_OSAL_ClockTimeSetInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    R_OSAL_RCFG_ClockTimeSetInitStatus(OSAL_MANAGER_STATE_INITIALIZED);

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_ClockTimeSetInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_CLK_006]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
