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
* File Name :    r_osal_hif_message.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Message Related Helper Functions
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
 * XOS3_OSAL_NOS_CD_CD_MSG_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_HIF_MqSendForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MqSendForTimePeriod(osal_os_mq_t mq,
                                               osal_milli_sec_t time_period,
                                               const void* p_buffer,
                                               size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;
    bool            is_isr; 
    
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (true == is_isr) /* Is ISR Context */
        {
            osal_ret = R_OSAL_OS_MqSend(mq, p_buffer, buffer_size);
        }
        else  /* Is Normal Context */
        {
            osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_OS_MqTimedSend(mq, &time_stamp, p_buffer, buffer_size);
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        /* Do nothing */
    }  /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_HIF_MqSendForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MSG_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MSG_017
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_HIF_MqSendUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MqSendUntilTimeStamp(osal_os_mq_t mq,
                                                const st_osal_time_t* const p_time_stamp,
                                                const void* p_buffer,
                                                size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    bool            is_isr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (true == is_isr) /* Is ISR Context */
        {
            osal_ret = R_OSAL_OS_MqSend(mq, p_buffer, buffer_size);
        }
        else  /* Is Normal Context */
        {
            osal_ret = R_OSAL_OS_MqTimedSend(mq, p_time_stamp, p_buffer, buffer_size);
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_HIF_MqSendUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MSG_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MSG_018
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_HIF_MqReceiveForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MqReceiveForTimePeriod (osal_os_mq_t mq,
                                                   osal_milli_sec_t time_period,
                                                   void* p_buffer,
                                                   size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;
    bool            is_isr; 
    
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (true == is_isr)    /* Is ISR Context */
        {
            osal_ret = R_OSAL_OS_MqReceive(mq, p_buffer, buffer_size);
        }
        else
        {
            osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_OS_MqTimedReceive(mq, &time_stamp, p_buffer, buffer_size);
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_HIF_MqReceiveForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MSG_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_MSG_019
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_HIF_MqReceiveUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MqReceiveUntilTimeStamp(osal_os_mq_t mq,
                                                   const st_osal_time_t* const p_time_stamp,
                                                   void* p_buffer,
                                                   size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    bool            is_isr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (true == is_isr) /* Is ISR Context */
        {
            osal_ret = R_OSAL_OS_MqReceive(mq, p_buffer, buffer_size);
        }
        else /* Is Normal Context */
        {
            osal_ret = R_OSAL_OS_MqTimedReceive(mq, p_time_stamp, p_buffer, buffer_size);
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_HIF_MqReceiveUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_MSG_019]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
