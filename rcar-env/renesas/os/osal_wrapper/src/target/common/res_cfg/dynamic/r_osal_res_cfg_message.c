/***********************************************************************************************************************
* Copyright [2022-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_res_cfg_message.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configuration Functions for Message Manager
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "target/common/res_cfg/dynamic/r_osal_res_cfg.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Definition
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef e_osal_rcfg_mq_search_array_t
 * @brief   Purpose of handle array search.
 **********************************************************************************************************************/
typedef enum
{
    OSAL_SEARCH_MQ_CREATE_HANDLE,   /*!< Search to create a handle */
    OSAL_SEARCH_MQ_REFER_HANDLE     /*!< Search to refer a handle  */
} e_osal_rcfg_mq_search_array_t;

/***********************************************************************************************************************
 * Global variable
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @var     gs_mq_init_state
 * @brief   Status of message manager.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_mq_init_state = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * @var     gs_inner_mq_mtx
 * @brief   Mutex of message manager.
***********************************************************************************************************************/
OSAL_STATIC osal_os_mutex_t gs_inner_mq_mtx;

/***********************************************************************************************************************
 * @var     gsp_mq_array
 * @brief   Handle array.
***********************************************************************************************************************/
OSAL_STATIC osal_mq_handle_t* gsp_mq_array = NULL;

/***********************************************************************************************************************
 * @var     gs_mq_array_size
 * @brief   Size of handle array.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_mq_array_size = 0;

/***********************************************************************************************************************
 * @var     gsp_mq_last_compare_addr
 * @brief   Last comparison address of bsearch.
***********************************************************************************************************************/
OSAL_STATIC void const* gsp_mq_last_compare_addr;

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Search the handle from the management array.
 * @param[in]        search_type    Handle array search.
 * @param[in]        check_id       Unique identifier of a message queue.
 * @param[in,out]    p_index        A pointer to the index of message queue.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_search_array(e_osal_rcfg_mq_search_array_t search_type, osal_mq_id_t check_id, size_t* p_index);

/***********************************************************************************************************************
 * @brief            Compare message queue ID for R_OSAL_OS_BinarySearch().
 * @param[in]        p_array1    A pointer to the created message queue.
 * @param[in]        p_array2    A pointer to the created message queue.
 * @return           #int
 * @retval           0
 * @retval           1
 * @retval           -1
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_mq_compare_id(const void* p_array1, const void* p_array2);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_MqSendForTimePeriod() and R_OSAL_RCFG_MqSendUntilTimeStamp() on the not ISR context.
 * @param[in,out]    handle          A handle of created message queue.
 * @param[in]        p_time_stamp    Time to wait until the queue is available.
 * @param[in]        p_buffer        A pointer to the message to send.
 * @param[in]        buffer_size     Size of the message to send.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_TIME
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_send_not_isr(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp, const void* p_buffer, size_t buffer_size);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_MqSendForTimePeriod() and R_OSAL_RCFG_MqSendUntilTimeStamp() on the ISR context.
 * @param[in]        handle          A handle of created message queue.
 * @param[in]        p_buffer        A pointer to the message to send.
 * @param[in]        buffer_size     Size of the message to send.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_send_isr(osal_mq_handle_t handle, const void* p_buffer, size_t buffer_size);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_MqReceiveForTimePeriod() and R_OSAL_RCFG_MqReceiveUntilTimeStamp() on the not ISR context.
 * @param[in,out]    handle          A handle of created message queue.
 * @param[in]        p_time_stamp    Time to wait until the queue is available.
 * @param[in]        p_buffer        A pointer to the message to send.
 * @param[in]        buffer_size     Size of the message to send.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_TIME
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_receive_not_isr(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp, void* p_buffer, size_t buffer_size);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_MqReceiveForTimePeriod() and R_OSAL_RCFG_MqReceiveUntilTimeStamp() on the ISR context.
 * @param[in]        handle         A handle of created message queue.
 * @param[in]        p_buffer       A pointer to the message to send.
 * @param[in]        buffer_size    Size of the message to send.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_receive_isr(osal_mq_handle_t handle, void* p_buffer, size_t buffer_size);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_MqCreate().
 * @param[in]        p_config    A pointer to the configuration of message queue.
 * @param[in]        mq_id       Unique identifier of a message queue.
 * @param[in]        index       The index of message queue.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_create(st_osal_mq_config_t const* p_config, osal_mq_id_t mq_id, size_t index);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_MqDelete().
 * @param[in]        handle    A handle of created message queue.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_delete(const osal_mq_handle_t handle);

/***********************************************************************************************************************
 * @brief            Delete the handle.
 * @param[in]        index    The index of message queue array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_delete_handle(size_t index);

/***********************************************************************************************************************
 * @brief            Check the handle of send and receive.
 * @param[in]        handle         A handle of created message queue.
 * @param[in]        buffer_size    Size of the message to send.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_check_send_receive_handle(const osal_mq_handle_t handle, size_t buffer_size);

/***********************************************************************************************************************
 * @brief            Common function for destroy the mutex.
 * @param[in]        p_mutex    A pointer to the mutex of message manager.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_mutex_destroy(osal_os_mutex_t* p_mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex for time period.
 * @param[in]        mutex    A handle of mutex of message manager.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_mutex_lock_for_time_period(osal_os_mutex_t mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex until timestamp.
 * @param[in]        mutex            A handle of mutex of message manager.
 * @param[in]        p_abs_timeout    Wait on a condition variable with timeout until time stamp..
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_MqGetInitStatus(void)
{
    return gs_mq_init_state;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_RCFG_MqSetInitStatus(e_osal_init_status_t status)
{
    gs_mq_init_state = status;
    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqControlInit(void)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    osal_os_mutexattr_t mutex_attr = NULL;

    /* Initialize the global variables */
    osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &gs_inner_mq_mtx);
    }

    if (NULL != mutex_attr)
    {
        mutex_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_mq_array                = NULL;
        gs_mq_array_size            = 0;
        gsp_mq_last_compare_addr    = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqControlDeinit(void)
{
    e_osal_return_t osal_ret;

    /* Destroy the mutex */
    osal_ret = R_OSAL_OS_MutexTryLock(gs_inner_mq_mtx);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        (void)R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);

        osal_ret = r_osal_rcfg_mq_mutex_destroy(&gs_inner_mq_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    /* Initialize the global variables */
    gs_mq_array_size            = 0;
    gsp_mq_last_compare_addr    = NULL;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqCheckBusy(void)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* If exist the created handle, return BUSY */
        if (NULL != gsp_mq_array)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqCreate(const st_osal_mq_config_t* const p_config, osal_mq_id_t mq_Id, osal_mq_handle_t* const p_handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    size_t          index = 0;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the mq_Id does not created */
        if (NULL != gsp_mq_array)
        {
            osal_ret = r_osal_rcfg_mq_search_array(OSAL_SEARCH_MQ_CREATE_HANDLE, mq_Id, &index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mq_create(p_config, mq_Id, index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2822 1 # The pointer and size are checked in the r_osal_rcfg_mq_create function. */
            *p_handle = gsp_mq_array[index];
            gs_mq_array_size++;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqDelete()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqDelete(osal_mq_handle_t handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_delete(handle);

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqDelete()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqSendForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqSendForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, const void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret;
    bool            is_isr;
    st_osal_time_t  time_stamp;

    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (false == is_isr)
        {
            osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_mq_send_not_isr(handle, &time_stamp, p_buffer, buffer_size);
            }
        }
        else
        {
            osal_ret = r_osal_rcfg_mq_send_isr(handle, p_buffer, buffer_size);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqSendForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqSendUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqSendUntilTimeStamp(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp, const void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret;
    bool            is_isr;

    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (false == is_isr)
        {
            osal_ret = r_osal_rcfg_mq_send_not_isr(handle, p_time_stamp, p_buffer, buffer_size);
        }
        else
        {
            osal_ret = r_osal_rcfg_mq_send_isr(handle, p_buffer, buffer_size);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqSendUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqReceiveForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqReceiveForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret;
    bool            is_isr;
    st_osal_time_t  time_stamp;

    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (false == is_isr)
        {
            osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_mq_receive_not_isr(handle, &time_stamp, p_buffer, buffer_size);
            }
        }
        else
        {
            osal_ret = r_osal_rcfg_mq_receive_isr(handle, p_buffer, buffer_size);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqReceiveForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqReceiveUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqReceiveUntilTimeStamp(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp, void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret;
    bool            is_isr;

    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (false == is_isr)
        {
            osal_ret = r_osal_rcfg_mq_receive_not_isr(handle, p_time_stamp, p_buffer, buffer_size);
        }
        else
        {
            osal_ret = r_osal_rcfg_mq_receive_isr(handle, p_buffer, buffer_size);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqReceiveUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqIsFull()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_MqIsFull(osal_mq_handle_t handle, bool* const p_result)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle is created */
        if (NULL == gsp_mq_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mq_search_array(OSAL_SEARCH_MQ_REFER_HANDLE, handle->mq_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MqIsFull(handle->mq, p_result);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqIsFull()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqIsEmpty()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_MqIsEmpty(osal_mq_handle_t handle, bool* const p_result)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle is created */
        if (NULL == gsp_mq_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mq_search_array(OSAL_SEARCH_MQ_REFER_HANDLE, handle->mq_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MqIsEmpty(handle->mq, p_result);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqIsEmpty()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqGetConfig()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_MqGetConfig(osal_mq_handle_t handle, st_osal_mq_config_t *const p_config)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle is created */
        if (NULL == gsp_mq_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mq_search_array(OSAL_SEARCH_MQ_REFER_HANDLE, handle->mq_id, NULL);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Return the config */
            *p_config = handle->osal_config;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqGetConfig()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqReset()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_MqReset(osal_mq_handle_t handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle is created */
        if (NULL == gsp_mq_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mq_search_array(OSAL_SEARCH_MQ_REFER_HANDLE, handle->mq_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MqReset(handle->mq);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqReset()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_search_array()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_search_array(e_osal_rcfg_mq_search_array_t search_type, osal_mq_id_t check_id, size_t* p_index)
{
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    st_osal_mq_control_t const* res;
    st_osal_mq_control_t        key_control;
    st_osal_mq_control_t const* key_handle = &key_control;
    size_t                      last_compare_index;
    st_osal_mq_control_t const* last_compare_handle;

    key_control.mq_id = check_id;
    gsp_mq_last_compare_addr = NULL;

    /* PRQA S 0316 2 # Required cast on system, not modifiable. */
    /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
    res = (st_osal_mq_control_t const *)R_OSAL_OS_BinarySearch((const void *)&key_handle, (const void *)gsp_mq_array, gs_mq_array_size, sizeof(gsp_mq_array[0]), r_osal_rcfg_mq_compare_id);
    if (NULL == gsp_mq_last_compare_addr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_SEARCH_MQ_CREATE_HANDLE == search_type)
        {
            if (NULL != res)
            {
                /* mq_id already exist */
                osal_ret = OSAL_RETURN_BUSY;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* Calc the index for insertion */
                /* PRQA S 0326, 0306 1 # uintptr_t is a type that represents a pointer type as an unsigned integer. */
                last_compare_index  = ((size_t)((uintptr_t)gsp_mq_last_compare_addr - (uintptr_t)&gsp_mq_array[0])) / sizeof(gsp_mq_array[0]);

                /* PRQA S 0316 1 # This casting is always compatible to the osal_mq_handle_t*. */
                last_compare_handle = *((const osal_mq_handle_t *)gsp_mq_last_compare_addr);

                /* Insert index is last compared index or last compared index + 1 in the binary search */
                if (check_id < last_compare_handle->mq_id)
                {
                    *p_index = last_compare_index;
                }
                else
                {
                    *p_index = last_compare_index + 1U;
                }
            }
        }
        else /* OSAL_SEARCH_MQ_REFER_HANDLE */
        {
            if (NULL == res)
            {
                /* mq_id does not exist */
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_index))
            {
                /* Calc the index for reference */
                /* PRQA S 0306 1 # uintptr_t is a type that represents a pointer type as an unsigned integer. */
                *p_index = ((size_t)((uintptr_t)res - (uintptr_t)&gsp_mq_array[0])) / sizeof(gsp_mq_array[0]);
            }
        } /* end of if (OSAL_SEARCH_MQ_CREATE_HANDLE == search_type) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_search_array()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_017
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_compare_id()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_mq_compare_id(const void* p_array1, const void* p_array2)
{
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int ret;

    /* PRQA S 0316 2 # This casting is always compatible to the osal_mq_handle_t*. */
    st_osal_mq_control_t const* p_handle1 = *((const osal_mq_handle_t *)p_array1);
    st_osal_mq_control_t const* p_handle2 = *((const osal_mq_handle_t *)p_array2);

    gsp_mq_last_compare_addr = (void const *)p_array2;

    if (p_handle1->mq_id < p_handle2->mq_id)
    {
        ret = -1;
    }
    else if (p_handle2->mq_id < p_handle1->mq_id)
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_compare_id()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_018
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_send_not_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_send_not_isr(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp, const void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_until_time_stamp(gs_inner_mq_mtx, p_time_stamp);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_check_send_receive_handle(handle, buffer_size);

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MqSendUntilTimeStamp(handle->mq, p_time_stamp, p_buffer, buffer_size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_send_not_isr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_019
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_send_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_send_isr(osal_mq_handle_t handle, const void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_check_send_receive_handle(handle, buffer_size);

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MqSendUntilTimeStamp(handle->mq, NULL, p_buffer, buffer_size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_send_isr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_020
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_receive_not_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_receive_not_isr(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp, void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_until_time_stamp(gs_inner_mq_mtx, p_time_stamp);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_check_send_receive_handle(handle, buffer_size);

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MqReceiveUntilTimeStamp(handle->mq, p_time_stamp, p_buffer, buffer_size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_receive_not_isr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_021
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_receive_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_receive_isr(osal_mq_handle_t handle, void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_mq_mutex_lock_for_time_period(gs_inner_mq_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_check_send_receive_handle(handle, buffer_size);

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MqReceiveUntilTimeStamp(handle->mq, NULL, p_buffer, buffer_size);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_receive_isr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_022
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_create()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_create(st_osal_mq_config_t const* p_config, osal_mq_id_t mq_id, size_t index)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    e_osal_return_t     mq_ret;
    osal_mq_handle_t*   p_handle_array;
    osal_os_mqattr_t    mq_attr = NULL;

    /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
    /* PRQA S 5118,2707 1 # Freed in r_osal_rcfg_mq_delete_handle() */
    p_handle_array = (osal_mq_handle_t *)realloc((void *)gsp_mq_array, (gs_mq_array_size + 1U) * sizeof(osal_mq_handle_t));
    if (NULL == p_handle_array)
    {
        osal_ret = OSAL_RETURN_MEM;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Move element for insertion */
        if (index < gs_mq_array_size)
        {
            /* PRQA S 1496 2 # Caller of this API should guarantee size of buffer */
            /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memmove((void *)&p_handle_array[index + 1U], (void *)&p_handle_array[index], (size_t)(gs_mq_array_size - index) * sizeof(p_handle_array[0]));
        }

        gsp_mq_array = p_handle_array;
        gsp_mq_array[index] = NULL;

        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        gsp_mq_array[index] = (st_osal_mq_control_t *)R_OSAL_OS_Malloc(sizeof(st_osal_mq_control_t));
        if (NULL == gsp_mq_array[index])
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_mq_array[index]->mq_id          = mq_id;
        gsp_mq_array[index]->osal_config    = *p_config;

        osal_ret = R_OSAL_OS_MqInitAttr(&mq_attr);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MqCreate(&gsp_mq_array[index]->osal_config, mq_attr, &gsp_mq_array[index]->mq);
    }

    if (NULL != mq_attr)
    {
        mq_ret = R_OSAL_OS_MqDeinitAttr(mq_attr);
        if ((OSAL_RETURN_OK != mq_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mq_ret;
        }
    }

    if ((OSAL_RETURN_OK != osal_ret) &&
        (NULL != gsp_mq_array[index]))
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_mq_array[index]);
        gsp_mq_array[index] = NULL;
    }

    return osal_ret;
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_create()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_023
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_delete()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_delete(const osal_mq_handle_t handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          index;

    /* Check the handle is created */
    if (NULL == gsp_mq_array)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_search_array(OSAL_SEARCH_MQ_REFER_HANDLE, handle->mq_id, &index);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MqCheckBusy(handle->mq);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MqDelete(handle->mq);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_delete_handle(index);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_delete()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_024
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_delete_handle()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_delete_handle(size_t index)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    osal_mq_handle_t*   p_handle_array = NULL;

    gs_mq_array_size--;

    if (NULL != gsp_mq_array[index])
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_mq_array[index]);
        gsp_mq_array[index] = NULL;

        /* Move element for deletion */
        if (index < gs_mq_array_size)
        {
            /* PRQA S 1496 2 # Caller of this API should guarantee size of buffer */
            /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memmove((void *)&gsp_mq_array[index], (void *)&gsp_mq_array[index + 1U], (size_t)((gs_mq_array_size - index) * sizeof(gsp_mq_array[0])));
        }
    }

    if (0U != gs_mq_array_size)
    {
        /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
        /* PRQA S 5118,2706 1 # Freed in r_osal_rcfg_mq_delete_handle() */
        p_handle_array = (osal_mq_handle_t *)realloc((void *)gsp_mq_array, gs_mq_array_size * sizeof(osal_mq_handle_t));
        if (NULL == p_handle_array)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }
    else
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_mq_array);
        gsp_mq_array = NULL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_mq_array = p_handle_array;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_delete_handle()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_025
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_check_send_receive_handle()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_check_send_receive_handle(const osal_mq_handle_t handle, size_t buffer_size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /* Check the handle is created */
    if (NULL == gsp_mq_array)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_search_array(OSAL_SEARCH_MQ_REFER_HANDLE, handle->mq_id, NULL);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (handle->osal_config.msg_size != buffer_size)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_check_send_receive_handle()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_026
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_mutex_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_mutex_destroy(osal_os_mutex_t* p_mutex)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_MutexDestroy(*p_mutex);
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_mutex = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_mutex_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_041]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_027
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_mutex_lock_for_time_period()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_mutex_lock_for_time_period(osal_os_mutex_t mutex)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(mutex, OSAL_INNER_TIMEOUT);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_mutex_lock_for_time_period()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_042]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_028
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(mutex, p_abs_timeout);
    if ((OSAL_RETURN_OK != osal_ret) &&
        (OSAL_RETURN_TIME != osal_ret))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_043]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
