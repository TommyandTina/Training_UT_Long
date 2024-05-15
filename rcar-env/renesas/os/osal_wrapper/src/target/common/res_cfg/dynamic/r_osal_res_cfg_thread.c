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
* File Name :    r_osal_res_cfg_thread.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configuration Functions for Thread Manager
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
 * @typedef e_osal_rcfg_thread_search_array_t
 * @brief   Purpose of handle array search.
 **********************************************************************************************************************/
typedef enum
{
    OSAL_SEARCH_THREAD_CREATE_HANDLE,   /*!< Search to create a handle */
    OSAL_SEARCH_THREAD_REFER_HANDLE     /*!< Search to refer a handle  */
} e_osal_rcfg_thread_search_array_t;

/***********************************************************************************************************************
 * Global variable
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @var     gs_thread_init_state
 * @brief   Status of thread manager.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_thread_init_state = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * @var     gs_inner_thread_mtx
 * @brief   Mutex of thread manager.
***********************************************************************************************************************/
OSAL_STATIC osal_os_mutex_t gs_inner_thread_mtx;

/***********************************************************************************************************************
 * @var     gsp_thread_array
 * @brief   Handle array.
***********************************************************************************************************************/
OSAL_STATIC osal_thread_handle_t* gsp_thread_array = NULL;

/***********************************************************************************************************************
 * @var     gs_thread_array_size
 * @brief   Size of handle array.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_thread_array_size = 0;

/***********************************************************************************************************************
 * @var     gsp_thread_last_compare_addr
 * @brief   Last comparison address of bsearch.
***********************************************************************************************************************/
OSAL_STATIC void const* gsp_thread_last_compare_addr;

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Function called by the thread.
 * @param[in]        p_arg    A pointer to the function argument.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void* r_osal_rcfg_internal_thread(void* p_arg);

/***********************************************************************************************************************
 * @brief            Search the handle from the management array.
 * @param[in]        search_type    A type of handle to search for.
 * @param[in]        check_id       Unique identifier of ID for thread.
 * @param[in,out]    p_index        A pointer to the index of handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_search_array(e_osal_rcfg_thread_search_array_t search_type, osal_thread_id_t check_id, size_t* p_index);

/***********************************************************************************************************************
 * @brief            Compare thread ID for R_OSAL_OS_BinarySearch().
 * @param[in]        p_array1    A pointer to the handle of thread.
 * @param[in]        p_array2    A pointer to the handle of thread.
 * @return           #int
 * @retval           0
 * @retval           1
 * @retval           -1
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_thread_compare_id(const void* p_array1, const void* p_array2);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_ThreadCreate().
 * @param[in]        p_config     A pointer to the configuration of thread.
 * @param[in]        thread_id    Unique identifier of ID for thread.
 * @param[in]        index        The index of the handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_create(st_osal_thread_config_t const* p_config, osal_thread_id_t thread_id, size_t index);

/***********************************************************************************************************************
 * @brief            Check the input parameter of R_OSAL_RCFG_ThreadJoin().
 * @param[in,out]    handle     A handle of thread.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_join_pre(osal_thread_handle_t const handle);

/***********************************************************************************************************************
 * @brief            Post procedure after thread join.
 * @param[in,out]    handle      A handle of thread.
 * @param[in]        join_ret    OSAL return value.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_join_post(osal_thread_handle_t const handle, e_osal_return_t join_ret);

/***********************************************************************************************************************
 * @brief            Delete the handle.
 * @param[in]        index    The index of the handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_delete_handle(size_t index);

/***********************************************************************************************************************
 * @brief            Common function for destroy the mutex.
 * @param[in]        p_mutex    A pointer to the mutex of pthread.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_mutex_destroy(osal_os_mutex_t* p_mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex for time period.
 * @param[in]        mutex    A handle of mutex of pthread.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_mutex_lock(osal_os_mutex_t mutex);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_ThreadGetInitStatus(void)
{
    return gs_thread_init_state;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_RCFG_ThreadSetInitStatus(e_osal_init_status_t status)
{
    gs_thread_init_state = status;
    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadControlInit(void)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    osal_os_mutexattr_t mutex_attr = NULL;

    /* Initialize the global variables */
    osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &gs_inner_thread_mtx);
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
        gsp_thread_array                = NULL;
        gs_thread_array_size            = 0;
        gsp_thread_last_compare_addr    = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadControlDeinit(void)
{
    e_osal_return_t osal_ret;

    /* Destroy the mutex */
    osal_ret = R_OSAL_OS_MutexTryLock(gs_inner_thread_mtx);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        (void)R_OSAL_OS_MutexUnlock(gs_inner_thread_mtx);

        osal_ret = r_osal_rcfg_thread_mutex_destroy(&gs_inner_thread_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    /* Initialize the global variables */
    gs_thread_array_size            = 0;
    gsp_thread_last_compare_addr    = NULL;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadCheckBusy(void)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_thread_mutex_lock(gs_inner_thread_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* If exist the created handle, return BUSY */
        if (NULL != gsp_thread_array)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_thread_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadCreate(const st_osal_thread_config_t* const p_config, osal_thread_id_t thread_id, osal_thread_handle_t* const p_handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    size_t          index = 0;

    osal_ret = r_osal_rcfg_thread_mutex_lock(gs_inner_thread_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the thread_id does not created */
        if (NULL != gsp_thread_array)
        {
            osal_ret = r_osal_rcfg_thread_search_array(OSAL_SEARCH_THREAD_CREATE_HANDLE, thread_id, &index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_thread_create(p_config, thread_id, index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2822 1 # The pointer and size are checked in the r_osal_rcfg_mq_create function. */
            *p_handle = gsp_thread_array[index];
            gs_thread_array_size++;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_thread_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadSelf()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSelf(osal_thread_handle_t* const p_handle)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    osal_os_thread_t    self_id;
    size_t              thread_count;
    bool                thread_equal;

    osal_ret = r_osal_rcfg_thread_mutex_lock(gs_inner_thread_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the self_id is created */
        osal_ret = R_OSAL_OS_ThreadSelf(&self_id);

        for (thread_count = 0; (OSAL_RETURN_OK == osal_ret) && (thread_count < gs_thread_array_size); thread_count++)
        {
            if (NULL != gsp_thread_array[thread_count])
            {
                osal_ret = R_OSAL_OS_ThreadEqual(self_id, gsp_thread_array[thread_count]->thread, &thread_equal);

                if ((OSAL_RETURN_OK == osal_ret) && (true == thread_equal))
                {
                    /* Return the self handle */
                    *p_handle = gsp_thread_array[thread_count];
                    break;
                }
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (gs_thread_array_size <= thread_count)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_thread_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
/* PRQA S 9110 1 # It is a series of processes and is necessary so as not to reduce the readability based on design. */
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadSelf()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadEqual()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_ThreadEqual(osal_thread_handle_t handle1, osal_thread_handle_t handle2, bool* const p_result)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    bool            thread_equal;

    osal_ret = r_osal_rcfg_thread_mutex_lock(gs_inner_thread_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle1 and handle2 are created */
        if (NULL == gsp_thread_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_thread_search_array(OSAL_SEARCH_THREAD_REFER_HANDLE, handle1->thread_id, NULL);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_thread_search_array(OSAL_SEARCH_THREAD_REFER_HANDLE, handle2->thread_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_thread_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_ThreadEqual(handle1->thread, handle2->thread, &thread_equal);

        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_result = thread_equal;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadEqual()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadJoin()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadJoin(osal_thread_handle_t handle, int64_t* const p_return_value)
{
    e_osal_return_t osal_ret;
    e_osal_return_t join_ret;

    /* Check the handle is enable join */
    osal_ret = r_osal_rcfg_thread_join_pre(handle);

    if (OSAL_RETURN_OK == osal_ret)
    {
        join_ret = R_OSAL_OS_ThreadWaitFinish(handle->thread);
        if (OSAL_RETURN_OK == join_ret)
        {
            handle->thread = NULL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL != p_return_value)
        {
            *p_return_value = handle->thread_ret;
        }

        /* Delete the joined handle */
        osal_ret = r_osal_rcfg_thread_join_post(handle, join_ret);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadJoin()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadSleepForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSleepForTimePeriod(osal_milli_sec_t time_period)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_ThreadSleepForTimePeriod(time_period);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadSleepForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadSleepUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSleepUntilTimeStamp(const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_ThreadSleepUntilTimeStamp(p_time_stamp);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadSleepUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_internal_thread()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the 3rd parameter of pthread_create(). */
OSAL_STATIC void* r_osal_rcfg_internal_thread(void* p_arg)
{
    e_osal_return_t             osal_ret;
    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    st_osal_thread_control_t*   p_thread_control = (st_osal_thread_control_t *)p_arg;

    osal_ret = R_OSAL_OS_ThreadSetTaskName(p_thread_control->osal_config.task_name);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_ThreadSetCpuAffinity(p_thread_control->thread_id);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL != p_thread_control->osal_config.func)
        {
            p_thread_control->thread_ret = p_thread_control->osal_config.func(p_thread_control->osal_config.userarg);
        }
        else
        {
            p_thread_control->thread_ret = 0;
        }
    }

    return NULL;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_internal_thread()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_search_array()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_search_array(e_osal_rcfg_thread_search_array_t search_type, osal_thread_id_t check_id, size_t* p_index)
{
    e_osal_return_t                 osal_ret = OSAL_RETURN_OK;
    st_osal_thread_control_t const* res;
    st_osal_thread_control_t        key_control;
    st_osal_thread_control_t const* key_handle = &key_control;
    size_t                          last_compare_index;
    st_osal_thread_control_t const* last_compare_handle;

    key_control.thread_id = check_id;
    gsp_thread_last_compare_addr = NULL;

    /* PRQA S 0316 2 # Required cast on system, not modifiable. */
    /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
    res = (st_osal_thread_control_t const *)R_OSAL_OS_BinarySearch((const void *)&key_handle, (const void *)gsp_thread_array, gs_thread_array_size, sizeof(gsp_thread_array[0]), r_osal_rcfg_thread_compare_id);
    if (NULL == gsp_thread_last_compare_addr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_SEARCH_THREAD_CREATE_HANDLE == search_type)
        {
            if (NULL != res)
            {
                /* thread_id already exist */
                osal_ret = OSAL_RETURN_BUSY;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* Calc the index for insertion */
                /* PRQA S 0326, 0306 1 # uintptr_t is a type that represents a pointer type as an unsigned integer. */
                last_compare_index  = ((size_t)((uintptr_t)gsp_thread_last_compare_addr - (uintptr_t)&gsp_thread_array[0])) / sizeof(gsp_thread_array[0]);

                /* PRQA S 0316 1 # This casting is always compatible to the osal_thread_handle_t*. */
                last_compare_handle = *((const osal_thread_handle_t *)gsp_thread_last_compare_addr);

                /* Insert index is last compared index or last compared index + 1 in the binary search */
                if (check_id < last_compare_handle->thread_id)
                {
                    *p_index = last_compare_index;
                }
                else
                {
                    *p_index = last_compare_index + 1U;
                }
            }
        }
        else /* OSAL_SEARCH_THREAD_REFER_HANDLE */
        {
            if (NULL == res)
            {
                /* thread_id does not exist */
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_index))
            {
                /* Calc the index for reference */
                /* PRQA S 0306 1 # uintptr_t is a type that represents a pointer type as an unsigned integer. */
                *p_index = ((size_t)((uintptr_t)res - (uintptr_t)&gsp_thread_array[0])) / sizeof(gsp_thread_array[0]);
            }
        } /* end of if (OSAL_SEARCH_THREAD_CREATE_HANDLE == search_type) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_search_array()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_compare_id()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_thread_compare_id(const void* p_array1, const void* p_array2)
{
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int ret;

    /* PRQA S 0316 2 # This casting is always compatible to the osal_thread_handle_t*. */
    st_osal_thread_control_t const* p_handle1 = *((const osal_thread_handle_t *)p_array1);
    st_osal_thread_control_t const* p_handle2 = *((const osal_thread_handle_t *)p_array2);

    gsp_thread_last_compare_addr = (void const *)p_array2;

    if (p_handle1->thread_id < p_handle2->thread_id)
    {
        ret = -1;
    }
    else if (p_handle2->thread_id < p_handle1->thread_id)
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
* End of function r_osal_rcfg_thread_compare_id()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_create()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_create(st_osal_thread_config_t const* p_config, osal_thread_id_t thread_id, size_t index)
{
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    osal_thread_handle_t*   p_handle_array;

    /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
    /* PRQA S 5118,2707 1 # Freed in r_osal_rcfg_thread_delete_handle() */
    p_handle_array = realloc((void *)gsp_thread_array, (gs_thread_array_size + 1U) * sizeof(osal_thread_handle_t));
    if (NULL == p_handle_array)
    {
        osal_ret = OSAL_RETURN_MEM;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Move element for insertion */
        if (index < gs_thread_array_size)
        {
            /* PRQA S 1496 2 # Caller of this API should guarantee size of buffer */
            /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memmove((void *)&p_handle_array[index + 1U], (void *)&p_handle_array[index], (size_t)(gs_thread_array_size - index) * sizeof(p_handle_array[0]));
        }

        gsp_thread_array = p_handle_array;
        gsp_thread_array[index] = NULL;

        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        gsp_thread_array[index] = (st_osal_thread_control_t *)R_OSAL_OS_Malloc(sizeof(st_osal_thread_control_t));
        if (NULL == gsp_thread_array[index])
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_thread_array[index]->thread_id      = thread_id;
        gsp_thread_array[index]->parent_thread  = false;
        gsp_thread_array[index]->osal_config    = *p_config;

        /* PRQA S 0314 1 # The 3rd arguments must be (void *) but no side effects in this case. */
        osal_ret = R_OSAL_OS_ThreadCreate(p_config, r_osal_rcfg_internal_thread, (void *)gsp_thread_array[index], &gsp_thread_array[index]->thread);
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_thread_array[index]);
        gsp_thread_array[index] = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_create()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_join_pre()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_join_pre(osal_thread_handle_t const handle)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    osal_os_thread_t    self_id;
    bool                thread_equal;

    osal_ret = r_osal_rcfg_thread_mutex_lock(gs_inner_thread_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the join to not own thread */
        osal_ret = R_OSAL_OS_ThreadSelf(&self_id);

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_ThreadEqual(self_id, handle->thread, &thread_equal);

            if ((OSAL_RETURN_OK == osal_ret) && (true == thread_equal))
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                if (true == handle->parent_thread)
                {
                    /* Already joined by another thread */
                    osal_ret = OSAL_RETURN_HANDLE;
                }
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->parent_thread = true;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) */

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_thread_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_join_pre()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_017
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_join_post()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_join_post(osal_thread_handle_t const handle, e_osal_return_t join_ret)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    size_t          index;

    osal_ret = r_osal_rcfg_thread_mutex_lock(gs_inner_thread_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->parent_thread = false;

        if (OSAL_RETURN_OK == join_ret)
        {
            /* Check the handle is created */
            if (NULL == gsp_thread_array)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_thread_search_array(OSAL_SEARCH_THREAD_REFER_HANDLE, handle->thread_id, &index);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_thread_delete_handle(index);
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_thread_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_join_post()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_018
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_delete_handle()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_delete_handle(size_t index)
{
    e_osal_return_t         osal_ret  = OSAL_RETURN_OK;
    osal_thread_handle_t*   p_handle_array = NULL;

    gs_thread_array_size--;

    if (NULL != gsp_thread_array[index])
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_thread_array[index]);
        gsp_thread_array[index] = NULL;

        /* Move element for deletion */
        if (index < gs_thread_array_size)
        {
            /* PRQA S 1496 2 # Caller of this API should guarantee size of buffer */
            /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memmove((void *)&gsp_thread_array[index], (void *)&gsp_thread_array[index + 1U], (size_t)(gs_thread_array_size - index) * sizeof(gsp_thread_array[0]));
        }
    }

    if (0U != gs_thread_array_size)
    {
        /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
        /* PRQA S 5118,2706 1 # Freed in r_osal_rcfg_thread_delete_handle() */
        p_handle_array = realloc((void *)gsp_thread_array, gs_thread_array_size * sizeof(osal_thread_handle_t));
        if (NULL == p_handle_array)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }
    else
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_thread_array);
        gsp_thread_array = NULL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_thread_array = p_handle_array;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_delete_handle()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_019
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_mutex_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_mutex_destroy(osal_os_mutex_t* p_mutex)
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
* End of function r_osal_rcfg_thread_mutex_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_020
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_mutex_lock()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_mutex_lock(osal_os_mutex_t mutex)
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
* End of function r_osal_rcfg_thread_mutex_lock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_023]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
