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
* File Name :    r_osal_res_cfg_threadsync.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configuration Functions for Thread Synchronization Manager
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
 * @typedef e_osal_rcfg_thsync_search_array_t
 * @brief   Purpose of handle array search.
 **********************************************************************************************************************/
typedef enum
{
    OSAL_SEARCH_THSYNC_CREATE_HANDLE,   /*!< Search to create a handle */
    OSAL_SEARCH_THSYNC_REFER_HANDLE     /*!< Search to refer a handle  */
} e_osal_rcfg_thsync_search_array_t;

/***********************************************************************************************************************
 * Global variable
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @var     gs_thsync_init_state
 * @brief   Status of thread synchronization manager.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_thsync_init_state = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * @var     gs_inner_mutex_mtx
 * @brief   Mutex of thread synchronization manager.
***********************************************************************************************************************/
OSAL_STATIC osal_os_mutex_t gs_inner_mutex_mtx;

/***********************************************************************************************************************
 * @var     gsp_mutex_array
 * @brief   Handle array.
***********************************************************************************************************************/
OSAL_STATIC osal_mutex_handle_t* gsp_mutex_array = NULL;

/***********************************************************************************************************************
 * @var     gs_mutex_array_size
 * @brief   Size of handle array.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_mutex_array_size = 0;

/***********************************************************************************************************************
 * @var     gsp_mutex_last_compare_addr
 * @brief   Last comparison address of bsearch.
***********************************************************************************************************************/
OSAL_STATIC void const* gsp_mutex_last_compare_addr;

/***********************************************************************************************************************
 * @var     gs_inner_cond_mtx
 * @brief   Mutex of thread synchronization manager.
***********************************************************************************************************************/
OSAL_STATIC osal_os_mutex_t gs_inner_cond_mtx;

/***********************************************************************************************************************
 * @var     gsp_cond_array
 * @brief   Handle array.
***********************************************************************************************************************/
OSAL_STATIC osal_cond_handle_t* gsp_cond_array = NULL;

/***********************************************************************************************************************
 * @var     gs_cond_array_size
 * @brief   Size of handle array.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_cond_array_size = 0;

/***********************************************************************************************************************
 * @var     gsp_cond_last_compare_addr
 * @brief   Last comparison address of bsearch.
***********************************************************************************************************************/
OSAL_STATIC void const* gsp_cond_last_compare_addr;

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Search the mutex handle from the management array.
 * @param[in]        search_type    Type of the handle array to search.
 * @param[in]        check_id       A handle of ID for mutex.
 * @param[in,out]    p_index        A pointer to the index of the handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_search_array(e_osal_rcfg_thsync_search_array_t search_type, osal_mutex_id_t check_id, size_t* p_index);

/***********************************************************************************************************************
 * @brief            Compare mutex ID for R_OSAL_OS_BinarySearch().
 * @param[in]        p_array1    A pointer to the handle of thread.
 * @param[in]        p_array2    A pointer to the handle of thread.
 * @return           #int
 * @retval           0
 * @retval           1
 * @retval           -1
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_mutex_compare_id(const void* p_array1, const void* p_array2);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_MutexLockForTimePeriod() and R_OSAL_RCFG_MutexLockUntilTimeStamp().
 * @param[in]        handle          A handle of created mutex.
 * @param[in]        p_time_stamp    The value of time to wait for mutex unblocked.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_lock(const osal_mutex_handle_t handle, const st_osal_time_t* const p_time_stamp);

/***********************************************************************************************************************
 * @brief            Search the condition variable handle from the management array.
 * @param[in]        search_type    Type of the handle array to search.
 * @param[in]        check_id       A handle of condition variable.
 * @param[in,out]    p_index        A pointer to the index of the handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_search_array(e_osal_rcfg_thsync_search_array_t search_type, osal_cond_id_t check_id, size_t* p_index);

/***********************************************************************************************************************
 * @brief            Compare condition variable ID for R_OSAL_OS_BinarySearch().
 * @param[in]        p_array1    A pointer to the handle of thread.
 * @param[in]        p_array2    A pointer to the handle of thread.
 * @return           #int
 * @retval           0
 * @retval           1
 * @retval           -1
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_cond_compare_id(const void* p_array1, const void* p_array2);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_CondWaitForTimePeriod() and R_OSAL_RCFG_CondWaitUntilTimeStamp().
 * @param[in,out]    cond_handle     A handle of created condition variable.
 * @param[in]        mutex_handle    A handle of created mutex.
 * @param[in]        p_time_stamp    The value of time to wait for mutex unblocked.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_TIME
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_wait(const osal_cond_handle_t cond_handle, const osal_mutex_handle_t mutex_handle, const st_osal_time_t* const p_time_stamp);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_RCFG_MutexCreate().
 * @param[in]        mutex_id    A pointer to the ID for mutex.
 * @param[in]        index       The index of the handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_create(osal_mutex_id_t mutex_id, size_t index);

/***********************************************************************************************************************
 * @brief            Delete the mutex handle.
 * @param[in]        index    The index of the handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_delete_handle(size_t index);

/***********************************************************************************************************************
 * @brief            Create the condition variable handle.
 * @param[in]        cond_id    A handle of condition variable.
 * @param[in]        index      The index of the handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_init(osal_cond_id_t cond_id, size_t index);

/***********************************************************************************************************************
 * @brief            Delete the condition variable handle.
 * @param[in]        index    The index of the handle array.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_delete_handle(size_t index);

/***********************************************************************************************************************
 * @brief            Common function for destroy the mutex.
 * @param[in,out]    p_mutex    A pointer to the mutex information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thsync_mutex_destroy(osal_os_mutex_t* p_mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex for time period.
 * @param[in,out]    mutex    A handle of the mutex information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thsync_mutex_lock_for_time_period(osal_os_mutex_t mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex until timestamp.
 * @param[in]        mutex            A handle of the mutex information.
 * @param[in]        p_abs_timeout    The value of time to wait for mutex unblocked.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thsync_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout);

/***********************************************************************************************************************
 * @brief            Common function for destroy the condition variable.
 * @param[in,out]    p_cond    A pointer to the condition variable object.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thsync_cond_destroy(osal_os_cond_t* p_cond);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_ThsyncGetInitStatus(void)
{
    return gs_thsync_init_state;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_RCFG_ThsyncSetInitStatus(e_osal_init_status_t status)
{
    gs_thsync_init_state = status;
    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncControlInit(void)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    osal_os_mutexattr_t mutex_attr = NULL;

    /* Initialize the global variables */
    osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &gs_inner_mutex_mtx);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &gs_inner_cond_mtx);
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
        osal_ret = R_OSAL_OS_MemoryBarrierInit();
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_mutex_array             = NULL;
        gs_mutex_array_size         = 0;
        gsp_mutex_last_compare_addr = NULL;

        gsp_cond_array              = NULL;
        gs_cond_array_size          = 0;
        gsp_cond_last_compare_addr  = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncControlDeinit(void)
{
    e_osal_return_t osal_ret;

    /* Destroy the mutex */
    osal_ret = R_OSAL_OS_MutexTryLock(gs_inner_cond_mtx);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        (void)R_OSAL_OS_MutexUnlock(gs_inner_cond_mtx);

        osal_ret = r_osal_rcfg_thsync_mutex_destroy(&gs_inner_cond_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexTryLock(gs_inner_mutex_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        (void)R_OSAL_OS_MutexUnlock(gs_inner_mutex_mtx);

        osal_ret = r_osal_rcfg_thsync_mutex_destroy(&gs_inner_mutex_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    /* Initialize the global variables */
    gs_cond_array_size          = 0;
    gsp_cond_last_compare_addr  = NULL;

    gs_mutex_array_size         = 0;
    gsp_mutex_last_compare_addr = NULL;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncCheckBusy(void)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_cond_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* If exist the created condition variable handle, return BUSY */
        if (NULL != gsp_cond_array)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_cond_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_mutex_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* If exist the created mutex handle, return BUSY */
            if (NULL != gsp_mutex_array)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mutex_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexCreate(osal_mutex_id_t mutex_id, osal_mutex_handle_t *const p_handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    size_t          index = 0;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_mutex_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the mutex_id does not created */
        if (NULL != gsp_mutex_array)
        {
            osal_ret = r_osal_rcfg_mutex_search_array(OSAL_SEARCH_THSYNC_CREATE_HANDLE, mutex_id, &index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mutex_create(mutex_id, index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2822 1 # The pointer and size are checked in the r_osal_rcfg_mutex_search_array function. */
            *p_handle = gsp_mutex_array[index];
            gs_mutex_array_size++;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mutex_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexDestroy()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_MutexDestroy(osal_mutex_handle_t handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    size_t          index;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_mutex_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the mutex handle is created */
        if (NULL == gsp_mutex_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mutex_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, handle->mutex_id, &index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_thsync_mutex_destroy(&handle->mutex);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mutex_delete_handle(index);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mutex_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexDestroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexLockForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexLockForTimePeriod(osal_mutex_handle_t handle, osal_milli_sec_t time_period)
{
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;

    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mutex_lock(handle, &time_stamp);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexLockForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexLockUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexLockUntilTimeStamp(osal_mutex_handle_t handle, const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret;

    osal_ret = r_osal_rcfg_mutex_lock(handle, p_time_stamp);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexLockUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexTryLock()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_MutexTryLock(osal_mutex_handle_t handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_mutex_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the mutex handle is created */
        if (NULL == gsp_mutex_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mutex_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, handle->mutex_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mutex_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexTryLock(handle->mutex);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexTryLock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexUnlock()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_MutexUnlock(osal_mutex_handle_t handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_mutex_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the mutex handle is created */
        if (NULL == gsp_mutex_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mutex_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, handle->mutex_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mutex_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexUnlock(handle->mutex);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexUnlock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondCreate(osal_cond_id_t cond_id, osal_cond_handle_t* const p_handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    size_t          index = 0;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_cond_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the cond_id does not created */
        if (NULL != gsp_cond_array)
        {
            osal_ret = r_osal_rcfg_cond_search_array(OSAL_SEARCH_THSYNC_CREATE_HANDLE, cond_id, &index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_cond_init(cond_id, index);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2822 1 # The pointer and size are checked in the r_osal_rcfg_cond_search_array function. */
            *p_handle = gsp_cond_array[index];
            gs_cond_array_size++;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_cond_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondDestroy()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_CondDestroy(osal_cond_handle_t handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;
    size_t          index;

    if (true == handle->active)
    {
        osal_ret = OSAL_RETURN_BUSY;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_cond_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check the condition variable handle is created */
            if (NULL == gsp_cond_array)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_cond_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, handle->cond_id, &index);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_thsync_cond_destroy(&handle->cond);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_cond_delete_handle(index);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_cond_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_rcfg_thsync_mutex_lock_for_time_period */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from if (true == handle->active) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondDestroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondWaitForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondWaitForTimePeriod(osal_cond_handle_t cond_handle, osal_mutex_handle_t mutex_handle, osal_milli_sec_t time_period)
{
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;

    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_cond_wait(cond_handle, mutex_handle, &time_stamp);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondWaitForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondWaitUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondWaitUntilTimeStamp(osal_cond_handle_t cond_handle, osal_mutex_handle_t mutex_handle, const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret;

    osal_ret = r_osal_rcfg_cond_wait(cond_handle, mutex_handle, p_time_stamp);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondWaitUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondSignal()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_CondSignal(osal_cond_handle_t handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_cond_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the condition variable handle is created */
        if (NULL == gsp_cond_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_cond_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, handle->cond_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_cond_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_CondSignal(handle->cond);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondSignal()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_017
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondBroadcast()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_RCFG_CondBroadcast(osal_cond_handle_t handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_for_time_period(gs_inner_cond_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the condition variable handle is created */
        if (NULL == gsp_cond_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_cond_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, handle->cond_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_cond_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_CondBroadcast(handle->cond);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondBroadcast()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_018
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MemoryBarrier()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MemoryBarrier(void)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_MemoryBarrier();

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MemoryBarrier()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_019
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_search_array()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_search_array(e_osal_rcfg_thsync_search_array_t search_type, osal_mutex_id_t check_id, size_t* p_index)
{
    e_osal_return_t                 osal_ret = OSAL_RETURN_OK;
    st_osal_mutex_control_t const*  res;
    st_osal_mutex_control_t         key_control;
    st_osal_mutex_control_t const*  key_handle = &key_control;
    size_t                          last_compare_index;
    st_osal_mutex_control_t const*  last_compare_handle;

    key_control.mutex_id = check_id;
    gsp_mutex_last_compare_addr = NULL;

    /* PRQA S 0316 2 # Required cast on system, not modifiable. */
    /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
    res = (st_osal_mutex_control_t const *)R_OSAL_OS_BinarySearch((const void *)&key_handle, (const void *)gsp_mutex_array, gs_mutex_array_size, sizeof(gsp_mutex_array[0]), r_osal_rcfg_mutex_compare_id);
    if (NULL == gsp_mutex_last_compare_addr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_SEARCH_THSYNC_CREATE_HANDLE == search_type)
        {
            if (NULL != res)
            {
                /* mutex_id already exist */
                osal_ret = OSAL_RETURN_BUSY;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* Calc the index for insertion */
                /* PRQA S 0326, 0306 1 # uintptr_t is a type that represents a pointer type as an unsigned integer. */
                last_compare_index  = ((size_t)((uintptr_t)gsp_mutex_last_compare_addr - (uintptr_t)&gsp_mutex_array[0])) / sizeof(gsp_mutex_array[0]);

                /* PRQA S 0316 1 # This casting is always compatible to the osal_mutex_handle_t*. */
                last_compare_handle = *((const osal_mutex_handle_t *)gsp_mutex_last_compare_addr);

                /* Insert index is last compared index or last compared index + 1 in the binary search */
                if (check_id < last_compare_handle->mutex_id)
                {
                    *p_index = last_compare_index;
                }
                else
                {
                    *p_index = last_compare_index + 1U;
                }
            }
        }
        else /* OSAL_SEARCH_THSYNC_REFER_HANDLE */
        {
            if (NULL == res)
            {
                /* mutex_id does not exist */
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_index))
            {
                /* Calc the index for reference */
                /* PRQA S 0306 1 # uintptr_t is a type that represents a pointer type as an unsigned integer. */
                *p_index = ((size_t)((uintptr_t)res - (uintptr_t)&gsp_mutex_array[0])) / sizeof(gsp_mutex_array[0]);
            }
        } /* end of if (OSAL_SEARCH_THSYNC_CREATE_HANDLE == search_type) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_search_array()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_020
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_compare_id()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_mutex_compare_id(const void* p_array1, const void* p_array2)
{
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int ret;

    /* PRQA S 0316 2 # This casting is always compatible to the osal_mutex_handle_t*. */
    st_osal_mutex_control_t const* p_handle1 = *((const osal_mutex_handle_t *)p_array1);
    st_osal_mutex_control_t const* p_handle2 = *((const osal_mutex_handle_t *)p_array2);

    gsp_mutex_last_compare_addr = (void const *)p_array2;

    if (p_handle1->mutex_id < p_handle2->mutex_id)
    {
        ret = -1;
    }
    else if (p_handle2->mutex_id < p_handle1->mutex_id)
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
* End of function r_osal_rcfg_mutex_compare_id()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_021
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_lock()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_lock(const osal_mutex_handle_t handle, const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    bool            is_isr;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_until_time_stamp(gs_inner_mutex_mtx, p_time_stamp);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the mutex handle is created */
        if (NULL == gsp_mutex_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mutex_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, handle->mutex_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mutex_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IsISRContext(&is_isr);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (false == is_isr)
            {
                osal_ret = r_osal_rcfg_thsync_mutex_lock_until_time_stamp(handle->mutex, p_time_stamp);
            }
            else
            {
                osal_ret = R_OSAL_OS_MutexTryLock(handle->mutex);
            }
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_lock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_022
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_search_array()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_search_array(e_osal_rcfg_thsync_search_array_t search_type, osal_cond_id_t check_id, size_t* p_index)
{
    e_osal_return_t                 osal_ret = OSAL_RETURN_OK;
    st_osal_cond_control_t const*   res;
    st_osal_cond_control_t          key_control;
    st_osal_cond_control_t const*   key_handle = &key_control;
    size_t                          last_compare_index;
    st_osal_cond_control_t const*   last_compare_handle;

    key_control.cond_id = check_id;
    gsp_cond_last_compare_addr = NULL;

    /* PRQA S 0316 2 # Required cast on system, not modifiable. */
    /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
    res = (st_osal_cond_control_t const *)R_OSAL_OS_BinarySearch((const void *)&key_handle, (const void *)gsp_cond_array, gs_cond_array_size, sizeof(gsp_cond_array[0]), r_osal_rcfg_cond_compare_id);
    if (NULL == gsp_cond_last_compare_addr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_SEARCH_THSYNC_CREATE_HANDLE == search_type)
        {
            if (NULL != res)
            {
                /* cond_id already exist */
                osal_ret = OSAL_RETURN_BUSY;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* Calc the index for insertion */
                /* PRQA S 0326, 0306 1 # uintptr_t is a type that represents a pointer type as an unsigned integer. */
                last_compare_index  = ((size_t)((uintptr_t)gsp_cond_last_compare_addr - (uintptr_t)&gsp_cond_array[0])) / sizeof(gsp_cond_array[0]);

                /* PRQA S 0316 1 # This casting is always compatible to the osal_cond_handle_t*. */
                last_compare_handle = *((const osal_cond_handle_t *)gsp_cond_last_compare_addr);

                /* Insert index is last compared index or last compared index + 1 in the binary search */
                if (check_id < last_compare_handle->cond_id)
                {
                    *p_index = last_compare_index;
                }
                else
                {
                    *p_index = last_compare_index + 1U;
                }
            }
        }
        else /* OSAL_SEARCH_THSYNC_REFER_HANDLE */
        {
            if (NULL == res)
            {
                /* cond_id does not exist */
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_index))
            {
                /* Calc the index for reference */
                /* PRQA S 0306 1 # uintptr_t is a type that represents a pointer type as an unsigned integer. */
                *p_index = ((size_t)((uintptr_t)res - (uintptr_t)&gsp_cond_array[0])) / sizeof(gsp_cond_array[0]);
            }
        } /* end of if (OSAL_SEARCH_THSYNC_CREATE_HANDLE == search_type) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_search_array()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_023
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_compare_id()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_cond_compare_id(const void* p_array1, const void* p_array2)
{
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int ret;

    /* PRQA S 0316 2 # This casting is always compatible to the osal_cond_handle_t*. */
    st_osal_cond_control_t const* p_handle1 = *((const osal_cond_handle_t *)p_array1);
    st_osal_cond_control_t const* p_handle2 = *((const osal_cond_handle_t *)p_array2);

    gsp_cond_last_compare_addr = (void const *)p_array2;

    if (p_handle1->cond_id < p_handle2->cond_id)
    {
        ret = -1;
    }
    else if (p_handle2->cond_id < p_handle1->cond_id)
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
* End of function r_osal_rcfg_cond_compare_id()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_024
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_wait()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_wait(const osal_cond_handle_t cond_handle, const osal_mutex_handle_t mutex_handle, const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_rcfg_thsync_mutex_lock_until_time_stamp(gs_inner_mutex_mtx, p_time_stamp);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the mutex handle is created */
        if (NULL == gsp_mutex_array)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mutex_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, mutex_handle->mutex_id, NULL);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_mutex_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_thsync_mutex_lock_until_time_stamp(gs_inner_cond_mtx, p_time_stamp);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check the condition variable handle is created */
            if (NULL == gsp_cond_array)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_cond_search_array(OSAL_SEARCH_THSYNC_REFER_HANDLE, cond_handle->cond_id, NULL);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_cond_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2982 1 # The "active" maybe referenced by another thread, not a verbose change. */
        cond_handle->active = true;
        osal_ret = R_OSAL_HIF_CondWaitUntilTimeStamp(cond_handle->cond, mutex_handle->mutex, p_time_stamp);
        cond_handle->active = false;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_wait()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_025
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_create()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_create(osal_mutex_id_t mutex_id, size_t index)
{
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    e_osal_return_t         mutex_ret;
    osal_os_mutexattr_t     mutex_attr = NULL;
    osal_mutex_handle_t*    p_handle_array;

    /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
    /* PRQA S 5118,2707 1 # Freed in r_osal_rcfg_mutex_delete_handle() */
    p_handle_array = (osal_mutex_handle_t *)realloc((void *)gsp_mutex_array, (gs_mutex_array_size + 1U) * sizeof(osal_mutex_handle_t));
    if (NULL == p_handle_array)
    {
        osal_ret =  OSAL_RETURN_MEM;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Move element for insertion */
        if (index < gs_mutex_array_size)
        {
            /* PRQA S 1496 2 # Caller of this API should guarantee size of buffer */
            /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memmove((void *)&p_handle_array[index + 1U], (void *)&p_handle_array[index], (size_t)(gs_mutex_array_size - index) * sizeof(p_handle_array[0]));
        }

        gsp_mutex_array = p_handle_array;
        gsp_mutex_array[index] = NULL;

        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        gsp_mutex_array[index] = (st_osal_mutex_control_t *)R_OSAL_OS_Malloc(sizeof(st_osal_mutex_control_t));
        if (NULL == gsp_mutex_array[index])
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_mutex_array[index]->mutex_id = mutex_id;

        osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &gsp_mutex_array[index]->mutex);
    }

    if (NULL != mutex_attr)
    {
        mutex_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_mutex_array[index]);
        gsp_mutex_array[index] = NULL;
    }

    return osal_ret;
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_create()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_026
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_delete_handle()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_delete_handle(size_t index)
{
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    osal_mutex_handle_t*    p_handle_array = NULL;

    gs_mutex_array_size--;

    if (NULL != gsp_mutex_array[index])
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_mutex_array[index]);
        gsp_mutex_array[index] = NULL;

        /* Move element for deletion */
        if (index < gs_mutex_array_size)
        {
            /* PRQA S 1496 2 # Caller of this API should guarantee size of buffer */
            /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memmove((void *)&gsp_mutex_array[index], (void *)&gsp_mutex_array[index + 1U], (size_t)((gs_mutex_array_size - index) * sizeof(gsp_mutex_array[0])));
        }
    }

    if (0U != gs_mutex_array_size)
    {
        /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
        /* PRQA S 5118,2706 1 # Freed in r_osal_rcfg_mutex_delete_handle() */
        p_handle_array = (osal_mutex_handle_t *)realloc((void *)gsp_mutex_array, gs_mutex_array_size * sizeof(osal_mutex_handle_t));
        if (NULL == p_handle_array)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }
    else
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_mutex_array);
        gsp_mutex_array = NULL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_mutex_array = p_handle_array;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_delete_handle()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_027
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_init()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_init(osal_cond_id_t cond_id, size_t index)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    osal_cond_handle_t* p_handle_array;

    /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
    /* PRQA S 5118,2707 1 # Freed in r_osal_rcfg_cond_delete_handle() */
    p_handle_array = (osal_cond_handle_t *)realloc((void *)gsp_cond_array, (gs_cond_array_size + 1U) * sizeof(osal_cond_handle_t));
    if (NULL == p_handle_array)
    {
        osal_ret = OSAL_RETURN_MEM;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Move element for insertion */
        if (index < gs_cond_array_size)
        {
            /* PRQA S 1496 2 # Caller of this API should guarantee size of buffer */
            /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memmove((void *)&p_handle_array[index + 1U], (void *)&p_handle_array[index], (size_t)(gs_cond_array_size - index) * sizeof(p_handle_array[0]));
        }

        gsp_cond_array = p_handle_array;
        gsp_cond_array[index] = NULL;

        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        gsp_cond_array[index] = (st_osal_cond_control_t *)R_OSAL_OS_Malloc(sizeof(st_osal_cond_control_t));
        if (NULL == gsp_cond_array[index])
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_cond_array[index]->cond_id  = cond_id;
        gsp_cond_array[index]->active   = false;

        osal_ret = R_OSAL_OS_CondCreate(&gsp_cond_array[index]->cond);
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_cond_array[index]);
        gsp_cond_array[index] = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_init()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_028
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_delete_handle()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_delete_handle(size_t index)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    osal_cond_handle_t* p_handle_array = NULL;

    gs_cond_array_size--;

    if (NULL != gsp_cond_array[index])
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_cond_array[index]);
        gsp_cond_array[index] = NULL;

        /* Move element for deletion */
        if (index < gs_cond_array_size)
        {
            /* PRQA S 1496 2 # Caller of this API should guarantee size of buffer */
            /* PRQA S 0314 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memmove((void *)&gsp_cond_array[index], (void *)&gsp_cond_array[index + 1U], (gs_cond_array_size - index) * sizeof(gsp_cond_array[0]));
        }
    }

    if (0U != gs_cond_array_size)
    {
        /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
        /* PRQA S 5118,2706 1 # Freed in r_osal_rcfg_cond_delete_handle() */
        p_handle_array = (osal_cond_handle_t *)realloc((void *)gsp_cond_array, gs_cond_array_size * sizeof(osal_cond_handle_t));
        if (NULL == p_handle_array)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }
    else
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_cond_array);
        gsp_cond_array = NULL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_cond_array = p_handle_array;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_delete_handle()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_029
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thsync_mutex_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thsync_mutex_destroy(osal_os_mutex_t* p_mutex)
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
* End of function r_osal_rcfg_thsync_mutex_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_030
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thsync_mutex_lock_for_time_period()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thsync_mutex_lock_for_time_period(osal_os_mutex_t mutex)
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
* End of function r_osal_rcfg_thsync_mutex_lock_for_time_period()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_031
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thsync_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thsync_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(mutex, p_abs_timeout);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thsync_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_041]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_032
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thsync_cond_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thsync_cond_destroy(osal_os_cond_t* p_cond)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_CondDestroy(*p_cond);
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_cond = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thsync_cond_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_042]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
