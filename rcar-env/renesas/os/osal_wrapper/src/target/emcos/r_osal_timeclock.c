/***********************************************************************************************************************
* Copyright [2021-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper TimeClock Manager for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <stdint.h>

#include "target/emcos/r_osal_common.h"


/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_clocktime Clock & Time Manager Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_clock_is_initialized
 * Initialize flag
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_clock_is_initialized = false;

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_ClockTimeGetTimeStamp()
* XOS3_OSAL_EMCOS_CD_CD_CLK_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_009]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ClockTimeGetTimeStamp(e_osal_clock_type_t clock_type,
                                             st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret  = OSAL_RETURN_OK;
    e_osal_return_t      api_ret;
    struct timespec      time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_clock_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((OSAL_CLOCK_TYPE_HIGH_RESOLUTION != clock_type) && (OSAL_CLOCK_TYPE_HIGH_PRECISION != clock_type))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        api_ret = R_OSAL_Internal_GetTimestamp(&time_stamp);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* output timestamp */
            p_time_stamp->tv_sec  = time_stamp.tv_sec;
            p_time_stamp->tv_nsec = (int32_t)time_stamp.tv_nsec;
        }
    }   /* end of if(false == gs_osal_clock_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ClockTimeGetTimeStamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_ClockTimeCalculateTimeDifference()
* XOS3_OSAL_EMCOS_CD_CD_CLK_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_005]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_006]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_007]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_008]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_ClockTimeCalculateTimeDifference(const st_osal_time_t *const p_time2,
                                                        const st_osal_time_t *const p_time1,
                                                        osal_nano_sec_t *const p_time_difference)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_nano_sec_t nsec1;
    osal_nano_sec_t nsec2;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_clock_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((p_time2 == NULL) || (p_time1 == NULL) || (p_time_difference == NULL))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    /* PRQA S 2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    else if ((p_time1->tv_sec < (time_t)0) || (p_time1->tv_nsec < 0) || (p_time1->tv_nsec > OSAL_MAX_TV_NSEC))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    /* PRQA S 2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    else if ((p_time2->tv_sec < (time_t)0) || (p_time2->tv_nsec < 0) || (p_time2->tv_nsec > OSAL_MAX_TV_NSEC))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        nsec1 = ((osal_nano_sec_t)p_time1->tv_sec * (osal_nano_sec_t)OSAL_SECTONSEC) + p_time1->tv_nsec;
        nsec2 = ((osal_nano_sec_t)p_time2->tv_sec * (osal_nano_sec_t)OSAL_SECTONSEC) + p_time2->tv_nsec;
        *p_time_difference = nsec2 - nsec1;
    }   /* end of if(false == gs_osal_clock_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_ClockTimeCalculateTimeDifference()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_clock_time_initialize()
* XOS3_OSAL_EMCOS_CD_CD_CLK_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_003]
***********************************************************************************************************************/
e_osal_return_t osal_clock_time_initialize(void)
{

    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true == gs_osal_clock_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_clock_is_initialized = true;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_clock_time_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_clock_time_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_CLK_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_001]
***********************************************************************************************************************/
e_osal_return_t osal_clock_time_deinitialize(void)
{

    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret      = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_clock_is_initialized = false;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_clock_time_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_clock_time_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_CLK_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_002]
***********************************************************************************************************************/
e_osal_return_t osal_clock_time_deinitialize_pre(void)
{

    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_clock_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_clock_is_initialized = false;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_clock_time_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_clock_time_set_is_init()
* XOS3_OSAL_EMCOS_CD_CD_CLK_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_CLK_004]
***********************************************************************************************************************/
void osal_clock_time_set_is_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_clock_is_initialized = true;

    return;
}
/***********************************************************************************************************************
* End of function osal_clock_time_set_is_init()
***********************************************************************************************************************/

