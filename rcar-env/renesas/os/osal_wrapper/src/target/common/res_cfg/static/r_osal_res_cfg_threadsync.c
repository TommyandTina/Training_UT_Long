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
* File Name :    r_osal_res_cfg_threadsync.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configuration Functions for Thread Synchronization Manager
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include "target/common/res_cfg/static/r_osal_res_cfg.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private data types
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_cond_config_addr_t
 * @brief structure to condition config address.
 **********************************************************************************************************************/
typedef struct
{
    const struct st_osal_rcfg_cond_config *p_usr_config;
}st_osal_cond_config_addr_t;

/***********************************************************************************************************************
 * @typedef st_osal_mutex_config_addr_t
 * @brief structure to mutex config address.
 **********************************************************************************************************************/
typedef struct
{
    const struct st_osal_rcfg_mutex_config *p_usr_config;
}st_osal_mutex_config_addr_t;

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * @var gs_osal_threadsync_is_initialized_enu
 * @brief Thread Synchronization Manager initial state flag.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_osal_threadsync_is_initialized_enu = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * @var gsp_osal_cond_control
 * @brief The value for the user to operate the condition variable without being aware of the Condition variable information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_cond_control_t *gsp_osal_cond_control = NULL;

/***********************************************************************************************************************
 * @var gs_osal_num_of_cond_table
 * @brief Number of the cond control information table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_num_of_cond_table = 0;

/***********************************************************************************************************************
 * @var gsp_osal_mutex_control
 * @brief The value for the user to operate the mutex without being aware of the Mutex information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_mutex_control_t *gsp_osal_mutex_control = NULL;

/***********************************************************************************************************************
 * @var gs_osal_num_of_mutex_table
 * @brief Number of the mutex control information table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_num_of_mutex_table = 0;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Initialize Thread Synchronization Manager (condition).
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_control_init(void);

/***********************************************************************************************************************
* @brief            Initialize attribute of Mutex and Condition.
* @param[in,out]    p_mutex_attr    The pointer of osal_os_mutexattr_t.
* @param[in,out]    p_cond_attr     The pointer of osal_os_condattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_init_attr(osal_os_mutexattr_t *p_mutex_attr,
                                                       osal_os_condattr_t *p_cond_attr);

/***********************************************************************************************************************
* @brief            Initialize control table for condition.
* @param[in]        mutex_attr    The pointer of osal_os_mutexattr_t.
* @param[in]        cond_attr     The pointer of osal_os_condattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_init_table(osal_os_mutexattr_t mutex_attr,
                                                        osal_os_condattr_t cond_attr);

/***********************************************************************************************************************
* @brief            Sort configuration table for condition.
* @param[in]        array_num              The num of array.
* @param[in,out]    p_cond_config_index    The pointer of st_osal_cond_config_addr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_sort_config(size_t array_num,
                                                         st_osal_cond_config_addr_t *p_cond_config_index);

/***********************************************************************************************************************
* @brief            Deinitialize attribute of Mutex and Condition.
* @param[in]        mutex_attr    The pointer of osal_os_mutexattr_t.
* @param[in]        cond_attr     The pointer of osal_os_condattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_deinit_attr(osal_os_mutexattr_t mutex_attr,
                                                         osal_os_condattr_t cond_attr);

/***********************************************************************************************************************
* @brief            Initialize Thread Synchronization Manager (mutex).
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_control_init(void);

/***********************************************************************************************************************
* @brief            Initialize control table for mutex.
* @param[in]        mutex_attr    The pointer of osal_os_mutexattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_init_table(osal_os_mutexattr_t mutex_attr);

/***********************************************************************************************************************
* @brief            Sort configuration table for mutex.
* @param[in]        array_num               The num of array.
* @param[in,out]    p_mutex_config_index    The pointer of st_osal_mutex_config_addr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_sort_config(size_t array_num,
                                                          st_osal_mutex_config_addr_t *p_mutex_config_index);

/***********************************************************************************************************************
* @brief            Deinitialize Thread Synchronization Manager (condition).
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_control_deinit(void);

/***********************************************************************************************************************
* @brief            Deinitialize Thread Synchronization Manager (mutex).
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_control_deinit(void);

/***********************************************************************************************************************
* @brief            Pre-check for Deinitialize Thread Synchronization Manager (condition).
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_check_busy(void);

/***********************************************************************************************************************
* @brief            Pre-check for Deinitialize Thread Synchronization Manager (mutex).
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_check_busy(void);

/***********************************************************************************************************************
* @brief            Count up the number of condition variables used.
* @param[in]        cond_handle    The pointer of osal_cond_handle_t.
* @param[in]        p_time_stamp   The value of time to wait for mutex unblocked.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_use_countup(osal_cond_handle_t cond_handle,
                                                         const st_osal_time_t *p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Compare function for the argument of R_OSAL_OS_BinarySearch().
* @param[in]        p_key           The key.
* @param[in]        p_search_info   The search information.
* @return           int
* @retval           Returns the number value of the comparison result.
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_cond_search(const void *p_key, 
                                        const void *p_search_info);

/*******************************************************************************************************************//**
* @brief            Compare function for the argument of R_OSAL_OS_BinarySearch().
* @param[in]        p_key           The key.
* @param[in]        p_search_info   The search information.
* @return           int
* @retval           Returns the number value of the comparison result.
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_mutex_search(const void *p_key, 
                                         const void *p_search_info);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_041
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_ThsyncGetInitStatus(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    return gs_osal_threadsync_is_initialized_enu;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_042
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_RCFG_ThsyncSetInitStatus(e_osal_init_status_t status)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_threadsync_is_initialized_enu = status;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_002]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_043
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncControlInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_cond_control_init();
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mutex_control_init();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_003]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_044
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncControlDeinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* The variable is changed so as not to overwrite the error factor. */
    osal_ret  = r_osal_rcfg_mutex_control_deinit();
    local_ret = r_osal_rcfg_cond_control_deinit();
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = local_ret;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_004]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_045
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThsyncCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThsyncCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mutex_check_busy();
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_cond_check_busy();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThsyncCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_005]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_046
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexCreate(osal_mutex_id_t mutex_id,
                                        osal_mutex_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    e_osal_return_t         local_ret;
    st_osal_mutex_control_t *p_control;
    osal_mutex_id_t         local_mutex_id = mutex_id;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0314 3 # No problem because it follows the specification of the calling function. */
    /* PRQA S 0316 1 # No problem because it is returned to the type of the second argument. */
    p_control = (st_osal_mutex_control_t*)R_OSAL_OS_BinarySearch((const void*)&local_mutex_id,
                                                                 (const void*)gsp_osal_mutex_control,
                                                                 gs_osal_num_of_mutex_table,
                                                                 sizeof(st_osal_mutex_control_t),
                                                                 r_osal_rcfg_mutex_search);
    if (NULL == p_control)
    {
        osal_ret = OSAL_RETURN_ID;
    }
    else
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_control->mutex, p_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (false != p_control->active)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                p_control->active    = true;
                p_control->handle_id = (uint64_t)OSAL_RES_CFG_MUTEX_HANDLE_ID;
                /* PRQA S 2916 1 # Since it is the address of the dynamically allocated area, there is no problem. */
                *p_handle            = p_control;
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_control->mutex);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                p_control->active    = false;
                p_control->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                osal_ret             = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }/* end of if(OSAL_RETURN_OK != osal_ret). */
    }   /* end of if(NULL == p_control). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_006]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_047
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexDestroy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MutexDestroy(osal_mutex_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret  = R_OSAL_OS_MutexTryLock(handle->mutex);
        if (OSAL_RETURN_OK == osal_ret)
        {
            if (true != handle->active)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
            else
            {
                handle->active    = false;
                handle->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
            }

            local_ret = R_OSAL_OS_MutexUnlock(handle->mutex);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexDestroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_007]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_048
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexLockForTimePeriod()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MutexLockForTimePeriod(osal_mutex_handle_t handle,
                                                   osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret;
    bool             is_isr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
        if (OSAL_RETURN_OK == osal_ret)
        {
            if (true == is_isr)    /* Is ISR Context */
            {
                osal_ret = R_OSAL_OS_MutexTryLock(handle->mutex);
            }
            else   /* Is Normal context. */
            {
                osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(handle->mutex, time_period);
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check if the block is released after the de-initialization. */
            if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_threadsync_is_initialized_enu)
            {
                osal_ret = OSAL_RETURN_STATE;
                (void)R_OSAL_OS_MutexUnlock(handle->mutex);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (false == handle->active))
            {
                osal_ret = OSAL_RETURN_HANDLE;
                (void)R_OSAL_OS_MutexUnlock(handle->mutex);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexLockForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_008]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_049
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexLockUntilTimeStamp()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MutexLockUntilTimeStamp(osal_mutex_handle_t handle,
                                                    const st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    bool            is_isr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
        if (OSAL_RETURN_OK == osal_ret)
        {
            if (true == is_isr)    /* Is ISR Context */
            {
                osal_ret = R_OSAL_OS_MutexTryLock(handle->mutex);
            }
            else /* Is Normal context. */
            {
                osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(handle->mutex, p_time_stamp);
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check if the block is released after the de-initialization. */
            if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_threadsync_is_initialized_enu)
            {
                osal_ret = OSAL_RETURN_STATE;
                (void)R_OSAL_OS_MutexUnlock(handle->mutex);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (false == handle->active))
            {
                osal_ret = OSAL_RETURN_HANDLE;
                (void)R_OSAL_OS_MutexUnlock(handle->mutex);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexLockUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_009]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_050
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexTryLock()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MutexTryLock(osal_mutex_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret  = R_OSAL_OS_MutexTryLock(handle->mutex);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check if the block is released after the de-initialization. */
            if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_threadsync_is_initialized_enu)
            {
                osal_ret = OSAL_RETURN_STATE;
                (void)R_OSAL_OS_MutexUnlock(handle->mutex);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (false == handle->active))
            {
                osal_ret = OSAL_RETURN_HANDLE;
                (void)R_OSAL_OS_MutexUnlock(handle->mutex);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexTryLock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_010]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_051
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MutexUnlock()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MutexUnlock(osal_mutex_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 5 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_MUTEX_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (false == handle->active)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret  = R_OSAL_OS_MutexUnlock(handle->mutex);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MutexUnlock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_011]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_052
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondCreate(osal_cond_id_t cond_id,
                                       osal_cond_handle_t* const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    e_osal_return_t         local_ret;
    st_osal_cond_control_t  *p_control;
    osal_cond_id_t          local_cond_id = cond_id;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0314 3 # No problem because it follows the specification of the calling function. */
    /* PRQA S 0316 1 # No problem because it is returned to the type of the second argument. */
    p_control = (st_osal_cond_control_t*)R_OSAL_OS_BinarySearch((const void*)&local_cond_id,
                                                                (const void*)gsp_osal_cond_control,
                                                                gs_osal_num_of_cond_table,
                                                                sizeof(st_osal_cond_control_t),
                                                                r_osal_rcfg_cond_search);
    if (NULL == p_control)
    {
        osal_ret = OSAL_RETURN_ID;
    }
    else
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_control->inner_mtx, p_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (false != p_control->active)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                p_control->active    = true;
                p_control->handle_id = (uint64_t)OSAL_RES_CFG_COND_HANDLE_ID;
                /* PRQA S 2916 1 # Since it is the address of the dynamically allocated area, there is no problem. */
                *p_handle            = p_control;
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_control->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                p_control->active    = false;
                p_control->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                osal_ret             = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }   /* end of if(NULL == p_control). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_053
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondDestroy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_CondDestroy(osal_cond_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_cond_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_cond_control[(gs_osal_num_of_cond_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_COND_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (true != handle->active)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
            else
            {
                if (0U == handle->use_count)
                {
                    handle->active    = false;
                    handle->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                }
                else
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }
            }

            local_ret = R_OSAL_OS_MutexUnlock(handle->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondDestroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_013]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_054
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondWaitForTimePeriod()
***********************************************************************************************************************/
/* PRQA S 3673 2 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_CondWaitForTimePeriod(osal_cond_handle_t cond_handle,
                                                  osal_mutex_handle_t mutex_handle,
                                                  osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;
    st_osal_time_t  abstime_roundup;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)cond_handle < (uintptr_t)&gsp_osal_cond_control[0]) ||
        ((uintptr_t)cond_handle > (uintptr_t)&gsp_osal_cond_control[(gs_osal_num_of_cond_table - (size_t)1)]) ||
        (0U != ((uintptr_t)cond_handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    else if (((uintptr_t)mutex_handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
             ((uintptr_t)mutex_handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
             (0U != ((uintptr_t)mutex_handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 5 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_COND_HANDLE_ID != cond_handle->handle_id) || (OSAL_RES_CFG_MUTEX_HANDLE_ID != mutex_handle->handle_id))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (false == mutex_handle->active)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &abstime_roundup);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992,2880 4 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check if the user's mutex object is locked. */
        local_ret = R_OSAL_OS_MutexTryLock(mutex_handle->mutex);
        if (OSAL_RETURN_BUSY == local_ret)
        {
            /* Do nothing */
        }
        else if (OSAL_RETURN_OK == local_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)R_OSAL_OS_MutexUnlock(mutex_handle->mutex);
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_cond_use_countup(cond_handle, &abstime_roundup);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_CondWaitUntilTimeStamp(cond_handle->cond, mutex_handle->mutex, &abstime_roundup);
        cond_handle->use_count--;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondWaitForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_014]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_055
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondWaitUntilTimeStamp()
***********************************************************************************************************************/
/* PRQA S 3673 2 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_CondWaitUntilTimeStamp(osal_cond_handle_t cond_handle,
                                                   osal_mutex_handle_t mutex_handle,
                                                   const st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;
    st_osal_time_t  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)cond_handle < (uintptr_t)&gsp_osal_cond_control[0]) ||
        ((uintptr_t)cond_handle > (uintptr_t)&gsp_osal_cond_control[(gs_osal_num_of_cond_table - (size_t)1)]) ||
        (0U != ((uintptr_t)cond_handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    else if (((uintptr_t)mutex_handle < (uintptr_t)&gsp_osal_mutex_control[0]) ||
             ((uintptr_t)mutex_handle > (uintptr_t)&gsp_osal_mutex_control[(gs_osal_num_of_mutex_table - (size_t)1)]) ||
             (0U != ((uintptr_t)mutex_handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 5 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_COND_HANDLE_ID != cond_handle->handle_id) || (OSAL_RES_CFG_MUTEX_HANDLE_ID != mutex_handle->handle_id))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (false == mutex_handle->active)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        time_stamp.tv_sec  = p_time_stamp->tv_sec;
        time_stamp.tv_nsec = p_time_stamp->tv_nsec;
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2996,2992,2880 4 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check if the user's mutex object is locked. */
        local_ret = R_OSAL_OS_MutexTryLock(mutex_handle->mutex);
        if (OSAL_RETURN_BUSY == local_ret)
        {
            /* Do nothing */
        }
        else if (OSAL_RETURN_OK == local_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)R_OSAL_OS_MutexUnlock(mutex_handle->mutex);
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_cond_use_countup(cond_handle, &time_stamp);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_CondWaitUntilTimeStamp(cond_handle->cond, mutex_handle->mutex, &time_stamp);
        cond_handle->use_count--;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondWaitUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_015]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_056
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondSignal()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_CondSignal(osal_cond_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_cond_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_cond_control[(gs_osal_num_of_cond_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_COND_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (true != handle->active)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
            else
            {
                osal_ret = R_OSAL_OS_CondSignal(handle->cond);
            }

            local_ret = R_OSAL_OS_MutexUnlock(handle->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_cond_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_cond_control[(gs_osal_num_of_cond_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondSignal()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_016]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_057
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_CondBroadcast()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_CondBroadcast(osal_cond_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_cond_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_cond_control[(gs_osal_num_of_cond_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_COND_HANDLE_ID != handle->handle_id)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (true != handle->active)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
            else
            {
                osal_ret = R_OSAL_OS_CondBroadcast(handle->cond);
            }

            local_ret = R_OSAL_OS_MutexUnlock(handle->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_cond_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_cond_control[(gs_osal_num_of_cond_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_CondBroadcast()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_017]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_058
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MemoryBarrier()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MemoryBarrier(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_MemoryBarrier();

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MemoryBarrier()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_018]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_018]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_059
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_control_init()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_control_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    e_osal_return_t             local_ret;
    osal_os_mutexattr_t         mutex_attr;
    osal_os_condattr_t          cond_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_cond_init_attr(&mutex_attr, &cond_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        gs_osal_num_of_cond_table = 0;
        while ((uint64_t)OSAL_COND_INVALID_ID != g_osal_cond_config[gs_osal_num_of_cond_table].id)
        {
            gs_osal_num_of_cond_table++;
        }

        if (0U != gs_osal_num_of_cond_table)
        {
            osal_ret = r_osal_rcfg_cond_init_table(mutex_attr, cond_attr);
        }
        else
        {
            /* Do nothing */
        }

        local_ret = r_osal_rcfg_cond_deinit_attr(mutex_attr, cond_attr);
        if ((OSAL_RETURN_OK == osal_ret) && (OSAL_RETURN_OK != local_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_control_init()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_022]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_072
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_init_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_cond_control_init() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_init_attr(osal_os_mutexattr_t *p_mutex_attr,
                                                       osal_os_condattr_t *p_cond_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_MutexInitAttr(p_mutex_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_CondInitAttr(p_cond_attr);
        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)R_OSAL_OS_MutexDeinitAttr(*p_mutex_attr);
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_init_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_022]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_073
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_init_table()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_cond_control_init() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_init_table(osal_os_mutexattr_t mutex_attr,
                                                        osal_os_condattr_t cond_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    size_t                      loop_count;
    st_osal_cond_config_addr_t  *p_cond_config_index;
    st_osal_cond_control_t      *p_cond_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
    p_cond_config_index = (st_osal_cond_config_addr_t*)R_OSAL_OS_Malloc((sizeof(st_osal_cond_config_addr_t) * gs_osal_num_of_cond_table));
    if (NULL == p_cond_config_index)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        osal_ret = r_osal_rcfg_cond_sort_config(gs_osal_num_of_cond_table, p_cond_config_index);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
        gsp_osal_cond_control = (st_osal_cond_control_t *)R_OSAL_OS_Malloc((sizeof(st_osal_cond_control_t) * gs_osal_num_of_cond_table));
        if (NULL == gsp_osal_cond_control)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    loop_count = 0;
    while ((gs_osal_num_of_cond_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        gsp_osal_cond_control[loop_count].usr_config    = p_cond_config_index[loop_count].p_usr_config;
        gsp_osal_cond_control[loop_count].inner_timeout = (osal_milli_sec_t)OSAL_RES_CFG_INNER_TIMEOUT;
        p_cond_control                                  = &gsp_osal_cond_control[loop_count];

        osal_ret = R_OSAL_OS_CondStaticCreate(cond_attr, &p_cond_control->cond);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &p_cond_control->inner_mtx);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            p_cond_control->active    = false;
            p_cond_control->use_count = 0U;
            p_cond_control->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gsp_osal_cond_control);
        gsp_osal_cond_control = NULL;
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
    R_OSAL_OS_Free((void*)p_cond_config_index);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_init_table()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_022]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_074
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_sort_config()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_cond_init_table() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_sort_config(size_t array_num,
                                                         st_osal_cond_config_addr_t *p_cond_config_index)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret = OSAL_RETURN_OK;
    uint64_t           tmp;
    size_t             loop_count;
    size_t             cmp_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_cond_config_index[0U].p_usr_config = &g_osal_cond_config[0U];
    for (loop_count = 1U; ((loop_count < array_num) && (OSAL_RETURN_OK == osal_ret)); loop_count++)
    {
        tmp = g_osal_cond_config[loop_count].id;
        for (cmp_count = loop_count; ((cmp_count > 0U) && (OSAL_RETURN_OK == osal_ret)); cmp_count--)
        {
            if (p_cond_config_index[(cmp_count - 1U)].p_usr_config->id > tmp)
            {
                p_cond_config_index[cmp_count].p_usr_config = p_cond_config_index[(cmp_count - 1U)].p_usr_config;
            }
            else if (p_cond_config_index[(cmp_count - 1U)].p_usr_config->id == tmp)
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                break;
            }
        }
        p_cond_config_index[cmp_count].p_usr_config = &g_osal_cond_config[loop_count];
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_sort_config()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_022]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_075
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_deinit_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_cond_control_init() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_deinit_attr(osal_os_mutexattr_t mutex_attr,
                                                         osal_os_condattr_t cond_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_CondDeinitAttr(cond_attr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        (void)R_OSAL_OS_MutexDeinitAttr(mutex_attr);
    }
    else
    {
        osal_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_deinit_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_022]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_060
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_control_init()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_control_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    e_osal_return_t             local_ret;
    osal_os_mutexattr_t         mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        gs_osal_num_of_mutex_table = 0;
        while ((uint64_t)OSAL_MUTEX_INVALID_ID != g_osal_mutex_config[gs_osal_num_of_mutex_table].id)
        {
            gs_osal_num_of_mutex_table++;
        }

        if (0U != gs_osal_num_of_mutex_table)
        {
            osal_ret = r_osal_rcfg_mutex_init_table(mutex_attr);
        }
        else
        {
            /* Do nothing */
        }

        local_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
        if ((OSAL_RETURN_OK == osal_ret) && (OSAL_RETURN_OK != local_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_control_init()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_019]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_076
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_init_table()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mutex_control_init() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_init_table(osal_os_mutexattr_t mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    size_t                      loop_count;
    st_osal_mutex_config_addr_t *p_mutex_config_index;
    st_osal_mutex_control_t     *p_mutex_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
    p_mutex_config_index = (st_osal_mutex_config_addr_t *)R_OSAL_OS_Malloc(
                                                (sizeof(st_osal_mutex_config_addr_t) * gs_osal_num_of_mutex_table));
    if (NULL == p_mutex_config_index)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        osal_ret = r_osal_rcfg_mutex_sort_config(gs_osal_num_of_mutex_table, p_mutex_config_index);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
        gsp_osal_mutex_control = (st_osal_mutex_control_t *)R_OSAL_OS_Malloc((sizeof(st_osal_mutex_control_t) * gs_osal_num_of_mutex_table));
        if (NULL == gsp_osal_mutex_control)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    loop_count = 0;
    while ((gs_osal_num_of_mutex_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        gsp_osal_mutex_control[loop_count].usr_config    = p_mutex_config_index[loop_count].p_usr_config;
        gsp_osal_mutex_control[loop_count].inner_timeout = (osal_milli_sec_t)OSAL_RES_CFG_INNER_TIMEOUT;
        p_mutex_control                                  = &gsp_osal_mutex_control[loop_count];

        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &p_mutex_control->mutex);
        if (OSAL_RETURN_OK == osal_ret)
        {
            p_mutex_control->active    = false;
            p_mutex_control->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gsp_osal_mutex_control);
        gsp_osal_mutex_control = NULL;
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
    R_OSAL_OS_Free((void*)p_mutex_config_index);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_init_table()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_019]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_077
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_sort_config()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mutex_init_table() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_sort_config(size_t array_num,
                                                          st_osal_mutex_config_addr_t *p_mutex_config_index)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret = OSAL_RETURN_OK;
    uint64_t           tmp;
    size_t             loop_count;
    size_t             cmp_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_mutex_config_index[0U].p_usr_config = &g_osal_mutex_config[0U];
    for (loop_count = 1U; ((loop_count < array_num) && (OSAL_RETURN_OK == osal_ret)); loop_count++)
    {
        tmp = g_osal_mutex_config[loop_count].id;
        for (cmp_count = loop_count; ((cmp_count > 0U) && (OSAL_RETURN_OK == osal_ret)); cmp_count--)
        {
            if (p_mutex_config_index[(cmp_count - 1U)].p_usr_config->id > tmp)
            {
                p_mutex_config_index[cmp_count].p_usr_config = p_mutex_config_index[(cmp_count - 1U)].p_usr_config;
            }
            else if (p_mutex_config_index[(cmp_count - 1U)].p_usr_config->id == tmp)
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                break;
            }
        }
        p_mutex_config_index[cmp_count].p_usr_config = &g_osal_mutex_config[loop_count];
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_sort_config()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_019]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_061
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_control_deinit()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_control_deinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    e_osal_return_t         api_ret;
    size_t                  loop_count;
    st_osal_cond_control_t  *p_cond_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_osal_cond_control)
    {
        loop_count = 0;
        while ((gs_osal_num_of_cond_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            p_cond_control = &gsp_osal_cond_control[loop_count];

            if (NULL != p_cond_control->cond)
            {
                api_ret = R_OSAL_OS_CondDestroy(p_cond_control->cond);
                if (OSAL_RETURN_OK != api_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }

            if (NULL != p_cond_control->inner_mtx)
            {
                api_ret = R_OSAL_OS_MutexDestroy(p_cond_control->inner_mtx);
                if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }

            p_cond_control->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
            p_cond_control->active    = false;
            p_cond_control->use_count = 0U;
            p_cond_control->cond      = NULL;
            p_cond_control->inner_mtx = NULL;

            loop_count++;
        }   /* end of while ((gs_osal_num_of_cond_table > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gsp_osal_cond_control);
        gsp_osal_cond_control     = NULL;
        gs_osal_num_of_cond_table = 0;
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != gsp_osal_cond_control). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_control_deinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_023]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_062
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_control_deinit()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_control_deinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    e_osal_return_t         api_ret;
    size_t                  loop_count;
    st_osal_mutex_control_t *p_mutex_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_osal_mutex_control)
    {
        loop_count = 0;
        while ((gs_osal_num_of_mutex_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            p_mutex_control = &gsp_osal_mutex_control[loop_count];

            if (NULL != p_mutex_control->mutex)
            {
                api_ret = R_OSAL_OS_MutexDestroy(p_mutex_control->mutex);
                if (OSAL_RETURN_OK != api_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }

            p_mutex_control->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
            p_mutex_control->active    = false;
            p_mutex_control->mutex     = NULL;

            loop_count++;
        }   /* end of while ((gs_osal_num_of_mutex_table > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gsp_osal_mutex_control);
        gsp_osal_mutex_control     = NULL;
        gs_osal_num_of_mutex_table = 0;
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != gsp_osal_mutex_control). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_control_deinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_020]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_063
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_check_busy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_check_busy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret = OSAL_RETURN_OK;
    e_osal_return_t              local_ret;
    size_t                       loop_count;
    const st_osal_cond_control_t *p_cond_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    loop_count = 0;
    while ((gs_osal_num_of_cond_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_cond_control = &gsp_osal_cond_control[loop_count];

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_cond_control->inner_mtx, p_cond_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (0U != p_cond_control->use_count)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_cond_control->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }

        loop_count++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_check_busy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_024]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_064
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_check_busy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mutex_check_busy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t               osal_ret = OSAL_RETURN_OK;
    size_t                        loop_count;
    const st_osal_mutex_control_t *p_mutex_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    loop_count = 0;
    while ((gs_osal_num_of_mutex_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_mutex_control = &gsp_osal_mutex_control[loop_count];

        osal_ret = R_OSAL_OS_MutexTryLock(p_mutex_control->mutex);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_MutexUnlock(p_mutex_control->mutex);
        }
        else
        {
            /* Do nothing */
        }

        loop_count++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_check_busy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_021]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_078
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_use_countup()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_cond_use_countup(osal_cond_handle_t cond_handle,
                                                         const st_osal_time_t *p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(cond_handle->inner_mtx, p_time_stamp);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check if the block is released after the de-initialization. */
        if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_threadsync_is_initialized_enu)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (true == cond_handle->active)
            {
                cond_handle->use_count++;
            }
            else
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
        }
        else
        {
            /* Do nothing */
        }

        local_ret = R_OSAL_OS_MutexUnlock(cond_handle->inner_mtx);
        if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
            cond_handle->use_count--;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_use_countup()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_046]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_046]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_067
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_cond_search()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_cond_search(const void *p_key, 
                                        const void *p_search_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 2 # There is no problem because it is just restored to the type at the time of function call. */
    osal_cond_id_t               key        = *(const osal_cond_id_t *)p_key;
    const st_osal_cond_control_t *p_control = (const st_osal_cond_control_t *)p_search_info;
    int                          result;    /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (key == p_control->usr_config->id)
    {
        result = 0;
    }
    else if (key > p_control->usr_config->id)
    {
        result = 1;
    }
    else
    {
        result = -1;
    }

    return result;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_cond_search()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_028]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_068
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mutex_search()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_mutex_search(const void *p_key, 
                                         const void *p_search_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 2 # There is no problem because it is just restored to the type at the time of function call. */
    osal_mutex_id_t               key        = *(const osal_mutex_id_t *)p_key;
    const st_osal_mutex_control_t *p_control = (const st_osal_mutex_control_t *)p_search_info;
    int                           result;   /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (key == p_control->usr_config->id)
    {
        result = 0;
    }
    else if (key > p_control->usr_config->id)
    {
        result = 1;
    }
    else
    {
        result = -1;
    }

    return result;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mutex_search()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_026]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_THS_026]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
