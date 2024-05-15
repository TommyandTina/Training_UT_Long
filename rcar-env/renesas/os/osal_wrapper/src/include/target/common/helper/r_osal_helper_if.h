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
* File Name :    r_osal_helper_if.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Helper IF Public Header
***********************************************************************************************************************/

#ifndef OSAL_HELPER_IF_H
#define OSAL_HELPER_IF_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "target/common/r_osal_common.h"
#include "target/common/os_if/r_osal_os_if.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Public global functions
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            OS interface for current thread sleep by time period.
* @param[in]        time_period       To set the value for sleep time by msec. (valid values are >=0)
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_ThreadSleepForTimePeriod
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_ThreadSleepForTimePeriod (osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief            OS interface for current thread sleep by time stamp.
* @param[in]        p_time_stamp      The pointer addresses of st_osal_time_t that holds.
*                                     the timestamp on which the thread will continue. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_ThreadSleepUntilTimeStamp
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_ThreadSleepUntilTimeStamp(const st_osal_time_t* const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            OS interface for mutex lock by time period.
* @param[in]        mutex        Address of OS mutex type.
* @param[in]        time_period  Sets the value of time to wait for the mutex is unlocked, in msec. (valid values are >= 0)
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MutexLockForTimePeriod
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MutexLockForTimePeriod(osal_os_mutex_t mutex,
                                                  osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief            OS interface for mutex lock by time stamp.
* @param[in]        mutex         Address of OS mutex type.
* @param[in]        p_time_stamp  Set the pointer address of st_osal_time_t as timestamp that wait until mutex is unlocked. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MutexLockUntilTimeStamp
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MutexLockUntilTimeStamp(osal_os_mutex_t mutex,
                                                   const st_osal_time_t* const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            OS interface for condition wait by time period.
* @param[in]        cond          Address of OS condition type.
* @param[in]        mutex         Address of OS mutex type.
* @param[in]        time_period   To set the value for sleep time by msec. (valid values are >=0)
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_CondWaitForTimePeriod
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_CondWaitForTimePeriod(osal_os_cond_t cond,
                                                 osal_os_mutex_t mutex,
                                                 osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief            OS interface for condition wait by time period.
* @param[in]        cond          Address of OS condition type.
* @param[in]        mutex         Address of OS mutex type.
* @param[in]        p_time_stamp  To set the pointer address of st_osal_time_t for timeout by time stamp. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_FAIL
* @note             This function works on the assumption that the mutex is locked before it is called.
*                   The minimum unit of time is 1 milli second. Digits less than 1 milli second is rounded up to
*                   1 milli second. If the time stamp is past time, use the current time stamp.
*                   The waiting time may be longer than specified due to the influence of other threads.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_CondWaitUntilTimeStamp
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_CondWaitUntilTimeStamp(osal_os_cond_t cond,
                                                  osal_os_mutex_t mutex,
                                                  const st_osal_time_t *const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Round up to the nearest ms and stored into the output variable.
* @param[in]        p_time_stamp_in  The address of input st_osal_time_t.
* @param[in,out]    p_time_stamp_out The address of output st_osal_time_t. 
* @return           void
* @note             None.
* @see              Refer to following the functions in this specification
*                   R_OSAL_HIF_ThreadSleepUntilTimeStamp
*                   R_OSAL_HIF_MutexLockUntilTimeStamp
*                   R_OSAL_HIF_CondWaitUntilTimeStamp
***********************************************************************************************************************/
void R_OSAL_HIF_ClockTimeRoundupByTimeStamp(const st_osal_time_t* const p_time_stamp_in,
                                            st_osal_time_t * p_time_stamp_out);

/*******************************************************************************************************************//**
* @brief            Get time out value by time period.
* @param[in]        time_period      To set the value for sleep time by msec. (valid values are >=0)
* @param[in,out]    p_time_stamp     The address of output st_osal_time_t. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_HIF_ThreadSleepForTimePeriod
*                   R_OSAL_HIF_MutexLockForTimePeriod
*                   R_OSAL_HIF_CondWaitForTimePeriod
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(osal_milli_sec_t time_period,
                                                           st_osal_time_t * p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Send a message to message queue. \n
*                   In case of the message queue is full, wait for the time specified by time_period.
* @param[in]        mq          The handle acquired at R_OSAL_OS_MqCreate().
* @param[in]        time_period Sets the value of time to wait for the queue be able to send message,
*                               in msec. (valid values are >= 0)
* @param[in]        p_buffer    The address of void* for the send data.
* @param[in]        buffer_size The p_buffer size by Byte. Should be equal msg_size in #st_osal_mq_config_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_TIME
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MqSendForTimePeriod(osal_os_mq_t mq,
                                               osal_milli_sec_t time_period,
                                               const void* p_buffer,
                                               size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Send a message to message queue. \n
*                   In case of the message queue is full, wait until the time specified by the time stamp is reached.
* @param[in]        mq            The handle acquired at R_OSAL_OS_MqCreate().
* @param[in]        p_time_stamp  Set the pointer address of #st_osal_time_t as timestamp that wait until
*                                 the queue be able to send message.
* @param[in]        p_buffer      The address of void* for the send data.
* @param[in]        buffer_size   The p_buffer size by Byte. Should be equal msg_size in #st_osal_mq_config_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_TIME
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MqSendUntilTimeStamp(osal_os_mq_t mq,
                                                const st_osal_time_t* const p_time_stamp,
                                                const void* p_buffer,
                                                size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Receive a message from the message queue. \n
*                   In case of the message queue is empty, wait for the time specified by time_period.
* @param[in]        mq           The handle acquired at R_OSAL_OS_MqCreate().
* @param[in]        time_period  Sets the value of time to wait for the queue be able to receive message,
*                                in msec. (valid values are >= 0)
* @param[in]        p_buffer     The address of void* for the receives data.
* @param[in]        buffer_size  The p_buffer size by Byte. Should be equal msg_size in #st_osal_mq_config_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_TIME
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MqReceiveForTimePeriod(osal_os_mq_t mq,
                                                  osal_milli_sec_t time_period,
                                                  void* p_buffer,
                                                  size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Receive a message from the message queue. \n
*                   In case of the message queue is empty, wait until the time that is specified by the time stamp is reached.
* @param[in]        mq            The handle acquired at R_OSAL_OS_MqCreate().
* @param[in]        p_time_stamp  Set the pointer address of #st_osal_time_t as timestamp that wait until
*                                 the queue be able to send message.
* @param[in]        p_buffer      The address of void* for the receive data.
* @param[in]        buffer_size   The p_buffer size by Byte. Should be equal msg_size in #st_osal_mq_config_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_TIME
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_HIF_MqReceiveUntilTimeStamp(osal_os_mq_t mq,
                                                   const st_osal_time_t* const p_time_stamp,
                                                   void* p_buffer,
                                                   size_t buffer_size);

/** @} OSAL_WRAPPER */

#endif /* OSAL_HELPER_IF_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
