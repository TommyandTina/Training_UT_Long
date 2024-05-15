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
* File Name :    r_osal_res_cfg_thread.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configuration Functions for Thread Manager
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
 * @typedef st_osal_thread_config_addr_t
 * @brief structure to thread config address.
 **********************************************************************************************************************/
typedef struct
{
    const struct st_osal_rcfg_thread_config *p_usr_config;
}st_osal_thread_config_addr_t;

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * @var gs_osal_thread_is_initialized_enu
 * @brief Thread Manager initial state flag.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_osal_thread_is_initialized_enu = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * @var gs_osal_num_of_thread_table
 * @brief Number of the thread control information table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_num_of_thread_table = 0;

/***********************************************************************************************************************
 * @var gsp_osal_thread_control
 * @brief The value for the user to operate the thread without being aware of the thread information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_thread_control_t *gsp_osal_thread_control = NULL;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Initialize attribute of Mutex and Condition.
* @param[in,out]    p_mutex_attr    The pointer of osal_os_mutexattr_t.
* @param[in,out]    p_cond_attr     The pointer of osal_os_condattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_init_attr(osal_os_mutexattr_t *p_mutex_attr,
                                                         osal_os_condattr_t *p_cond_attr);

/***********************************************************************************************************************
* @brief            Initialize control table for thread.
* @param[in]        mutex_attr    The pointer of osal_os_mutexattr_t.
* @param[in]        cond_attr     The pointer of osal_os_condattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_init_table(osal_os_mutexattr_t mutex_attr,
                                                          osal_os_condattr_t cond_attr);

/***********************************************************************************************************************
* @brief            Sort configuration table for thread.
* @param[in]        array_num                The num of array.
* @param[in,out]    p_thread_config_index    The pointer of st_osal_thread_config_addr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_sort_config(size_t array_num, 
                                                    st_osal_thread_config_addr_t *p_thread_config_index);

/***********************************************************************************************************************
* @brief            Initialize each control table for thread.
* @param[in]        mutex_attr    The pointer of osal_os_mutexattr_t.
* @param[in]        cond_attr     The pointer of osal_os_condattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_init_each_table(osal_os_mutexattr_t mutex_attr,
                                                               osal_os_condattr_t cond_attr,
                                                               st_osal_thread_config_addr_t *p_thread_config_index);

/***********************************************************************************************************************
* @brief            Deinitialize attribute of Mutex and Condition.
* @param[in]        mutex_attr    The pointer of osal_os_mutexattr_t.
* @param[in]        cond_attr     The pointer of osal_os_condattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_deinit_attr(osal_os_mutexattr_t mutex_attr,
                                                           osal_os_condattr_t cond_attr);

/***********************************************************************************************************************
* @brief            Notifies the internal thread of finish.
* @param[in,out]    p_thread_control    The pointer to st_osal_thread_control_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_notify_finish(st_osal_thread_control_t *p_thread_control);

/***********************************************************************************************************************
* @brief            Freeing up system resources in thread deinitialize.
* @param[in,out]    p_thread_control    The pointer to st_osal_thread_control_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_deinit_system_resources(st_osal_thread_control_t *p_thread_control);

/***********************************************************************************************************************
* @brief            Destroy mutex.
* @param[in,out]    p_thread_control    The pointer to st_osal_thread_control_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_destroy_mutex(const st_osal_thread_control_t *p_thread_control);

/***********************************************************************************************************************
* @brief            Wait for user thread to finish.
* @param[in,out]    handle              The address of osal_thread_handle_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_wait_user_thread(osal_thread_handle_t handle);

/***********************************************************************************************************************
* @brief            Internal thread of the Thread Manager.
* @param[in]        p_arg  The address of st_osal_thread_control_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void* r_osal_rcfg_internal_thread(void * p_arg);

/***********************************************************************************************************************
* @brief            Notifies the internal thread of join.
* @param[in,out]    p_control    The pointer to st_osal_thread_control_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_notify_join(st_osal_thread_control_t *p_control);

/***********************************************************************************************************************
* @brief            Compare function for the argument of R_OSAL_OS_BinarySearch().
* @param[in]        p_key           The key.
* @param[in]        p_search_info   The search information.
* @return           int
* @retval           Returns the number value of the comparison result.
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_thread_search(const void *p_key, 
                                          const void *p_search_info);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_041
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_ThreadGetInitStatus(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    return gs_osal_thread_is_initialized_enu;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_042
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_RCFG_ThreadSetInitStatus(e_osal_init_status_t status)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_thread_is_initialized_enu = status;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_002]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_043
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadControlInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    e_osal_return_t              local_ret;
    osal_os_mutexattr_t          mutex_attr;
    osal_os_condattr_t           cond_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_thread_init_attr(&mutex_attr, &cond_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        gs_osal_num_of_thread_table = 0;
        while((uint64_t)OSAL_THREAD_INVALID_ID != g_osal_thread_config[gs_osal_num_of_thread_table].id)
        {
            gs_osal_num_of_thread_table++;
        }

        if (0U != gs_osal_num_of_thread_table)
        {
            osal_ret = r_osal_rcfg_thread_init_table(mutex_attr, cond_attr);
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        local_ret = r_osal_rcfg_thread_deinit_attr(mutex_attr, cond_attr);
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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_003]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_044
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadControlDeinit()
**********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadControlDeinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    e_osal_return_t          api_ret;
    size_t                   loop_count        = 0;
    st_osal_thread_control_t *p_thread_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_osal_thread_control)
    {
        while ((gs_osal_num_of_thread_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            p_thread_control = &gsp_osal_thread_control[loop_count];

            if ((NULL != p_thread_control->create_cond) && (NULL != p_thread_control->inner_mtx))
            {
                api_ret = r_osal_rcfg_thread_notify_finish(p_thread_control);
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

            if (NULL != p_thread_control->thread)
            {
                api_ret = R_OSAL_OS_ThreadWaitFinish(p_thread_control->thread);
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

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_thread_deinit_system_resources(p_thread_control);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    p_thread_control->handle_id     = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                    p_thread_control->active        = false;
                    p_thread_control->parent_thread = false;
                    p_thread_control->thread_ret    = 0;
                    p_thread_control->thread        = NULL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }   /* end of if (OSAL_RETURN_OK == osal_ret) */
            loop_count++;
        }   /* end of while ((gs_osal_num_of_thread_table > loop_count) && (OSAL_RETURN_OK == osal_ret)) */

        R_OSAL_OS_ThreadStaticDeinit();

        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gsp_osal_thread_control);
        gsp_osal_thread_control     = NULL;
        gs_osal_num_of_thread_table = 0;
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != gsp_osal_thread_control). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_004]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_045
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                osal_ret          = OSAL_RETURN_OK;
    e_osal_return_t                local_ret;
    size_t                         loop_count        = 0;
    const st_osal_thread_control_t *p_thread_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((gs_osal_num_of_thread_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_thread_control = &gsp_osal_thread_control[loop_count];
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_thread_control->inner_mtx, p_thread_control->inner_timeout);
        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((uint64_t)OSAL_RES_CFG_THREAD_HANDLE_ID == p_thread_control->handle_id)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_thread_control->inner_mtx);
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
            osal_ret = OSAL_RETURN_FAIL;
        }   /* end of if (OSAL_RETURN_OK == osal_ret) */

        loop_count++;
    }/* end of while ((gs_osal_num_of_thread_table > loop_count) && (OSAL_RETURN_OK == osal_ret)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_005]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_046
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadCreate(const st_osal_thread_config_t* const p_config,
                                         osal_thread_id_t thread_id,
                                         osal_thread_handle_t* const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret;
    e_osal_return_t          local_ret;
    st_osal_thread_control_t *p_control;
    osal_thread_id_t         local_thread_id = thread_id;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0314 3 # No problem because it follows the specification of the calling function. */
    /* PRQA S 0316 1 # No problem because it is returned to the type of the second argument. */
    p_control = (st_osal_thread_control_t*)R_OSAL_OS_BinarySearch((const void*)&local_thread_id,
                                                                  (const void*)gsp_osal_thread_control,
                                                                  gs_osal_num_of_thread_table,
                                                                  sizeof(st_osal_thread_control_t),
                                                                  r_osal_rcfg_thread_search);
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
            /* Check if the block is released after the de-initialization. */
            if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_thread_is_initialized_enu)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) &&
                ((uint64_t)OSAL_RES_CFG_THREAD_HANDLE_ID == p_control->handle_id))
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                p_control->func       = p_config->func;
                p_control->userarg    = p_config->userarg;
                p_control->thread_ret = 0;

                local_ret = R_OSAL_OS_CondSignal(p_control->create_cond);
                if (OSAL_RETURN_OK == local_ret)
                {
                    p_control->handle_id = (uint64_t)OSAL_RES_CFG_THREAD_HANDLE_ID;
                    p_control->active    = true;
                }
                else
                {
                    osal_ret = local_ret;
                }
            }
            else
            {
                /* Do nothing */
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_control->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* PRQA S 2916 1 # Since it is the address of the dynamically allocated area, there is no problem. */
                *p_handle = p_control;
            }
        }   /* end of if ((OSAL_RETURN_OK != osal_ret) && (OSAL_MANAGER_STATE_INITIALIZED == gs_osal_thread_is_initialize_enu)) */
    }   /* end of if (NULL == p_control) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_006]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_047
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadSelf()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSelf(osal_thread_handle_t* const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    void            *p_arg;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_ThreadStaticSelf(&p_arg);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* It is not a thread created using the OSAL API. */
        if (NULL == p_arg)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 2916,0316 1 # There is no problem because it is just reconverted to the type when it was set. */
            *p_handle = (st_osal_thread_control_t *)p_arg;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadSelf()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_007]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_048
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadEqual()
***********************************************************************************************************************/
/* PRQA S 3673 2 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_ThreadEqual(osal_thread_handle_t handle1,
                                        osal_thread_handle_t handle2,
                                        bool* const p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle1 < (uintptr_t)&gsp_osal_thread_control[0]) ||
        ((uintptr_t)handle1 > (uintptr_t)&gsp_osal_thread_control[(gs_osal_num_of_thread_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle1 & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    else if (((uintptr_t)handle2 < (uintptr_t)&gsp_osal_thread_control[0]) ||
             ((uintptr_t)handle2 > (uintptr_t)&gsp_osal_thread_control[(gs_osal_num_of_thread_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle2 & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_THREAD_HANDLE_ID != handle1->handle_id) || (OSAL_RES_CFG_THREAD_HANDLE_ID != handle2->handle_id))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
        if (handle1 == handle2)
        {
            *p_result = true;
        }
        else
        {
            *p_result = false;
        }
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle1 < (uintptr_t)&gsp_osal_thread_control[0]) ||
                      ((uintptr_t)handle1 > (uintptr_t)&gsp_osal_thread_control[(gs_osal_num_of_thread_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle1 & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadEqual()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_008]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_049
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadJoin()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadJoin(osal_thread_handle_t handle,
                                       int64_t* const p_return_value)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;
    void            *p_arg;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_thread_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_thread_control[(gs_osal_num_of_thread_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if (OSAL_RES_CFG_THREAD_HANDLE_ID != handle->handle_id)
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
        osal_ret = R_OSAL_OS_ThreadStaticSelf(&p_arg);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* It is not a thread created using the OSAL API. */
            if (NULL != p_arg)
            {
                /* PRQA S 0316 1 # There is no problem because it is just reconverted to the type when it was set. */
                if ((st_osal_thread_control_t *)p_arg == handle)
                {
                    osal_ret = OSAL_RETURN_HANDLE;
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
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

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
            if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_thread_is_initialized_enu)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_rcfg_thread_wait_user_thread(handle);
                if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_return_value))
                {
                    *p_return_value = handle->thread_ret;
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

            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
            }
            else
            {
                /* Do nothing */
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
        }   /* end of if (OSAL_RETURN_OK == osal_ret) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
    /* PRQA S 9104 1 # For better readability this rule is ignored and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadJoin()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_009]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_050
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadSleepForTimePeriod()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSleepForTimePeriod(osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_HIF_ThreadSleepForTimePeriod(time_period);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadSleepForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_010]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_051
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_ThreadSleepUntilTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_ThreadSleepUntilTimeStamp(const st_osal_time_t* const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_HIF_ThreadSleepUntilTimeStamp(p_time_stamp);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_ThreadSleepUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_011]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_011]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_058
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_init_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_ThreadControlInit() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_init_attr(osal_os_mutexattr_t *p_mutex_attr,
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
* End of function r_osal_rcfg_thread_init_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_059
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_init_table()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_ThreadControlInit() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_init_table(osal_os_mutexattr_t mutex_attr,
                                                          osal_os_condattr_t cond_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret;
    st_osal_thread_config_addr_t *p_thread_config_index;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
    p_thread_config_index = (st_osal_thread_config_addr_t *)R_OSAL_OS_Malloc(sizeof(st_osal_thread_config_addr_t) * gs_osal_num_of_thread_table);
    if (NULL == p_thread_config_index)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        osal_ret = r_osal_rcfg_thread_sort_config(gs_osal_num_of_thread_table, p_thread_config_index);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
        gsp_osal_thread_control = (st_osal_thread_control_t *)R_OSAL_OS_Malloc((sizeof(st_osal_thread_control_t) * gs_osal_num_of_thread_table));
        if (NULL == gsp_osal_thread_control)
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_ThreadStaticInit(gs_osal_num_of_thread_table);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_thread_init_each_table(mutex_attr, cond_attr, p_thread_config_index);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gsp_osal_thread_control);
        gsp_osal_thread_control = NULL;
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
    R_OSAL_OS_Free((void*)p_thread_config_index);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_init_table()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_060
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_sort_config()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_thread_init_table() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_sort_config(size_t array_num,
                                                           st_osal_thread_config_addr_t *p_thread_config_index)
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
    p_thread_config_index[0U].p_usr_config = &g_osal_thread_config[0U];
    for (loop_count = 1U; ((loop_count < array_num) && (OSAL_RETURN_OK == osal_ret)); loop_count++)
    {
        tmp = g_osal_thread_config[loop_count].id;
        for (cmp_count = loop_count; ((cmp_count > 0U) && (OSAL_RETURN_OK == osal_ret)); cmp_count--)
        {
            if (p_thread_config_index[(cmp_count - 1U)].p_usr_config->id > tmp)
            {
                p_thread_config_index[cmp_count].p_usr_config = p_thread_config_index[(cmp_count - 1U)].p_usr_config;
            }
            else if (p_thread_config_index[(cmp_count - 1U)].p_usr_config->id == tmp)
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                break;
            }
        }
        p_thread_config_index[cmp_count].p_usr_config = &g_osal_thread_config[loop_count];
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_sort_config()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_061
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_init_each_table()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_thread_init_table() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_init_each_table(osal_os_mutexattr_t mutex_attr,
                                                               osal_os_condattr_t cond_attr,
                                                               st_osal_thread_config_addr_t *p_thread_config_index)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret    = OSAL_RETURN_OK;
    size_t                      loop_count  = 0U;
    osal_os_threadattr_t        thread_attr = NULL;
    st_osal_thread_control_t    *p_thread_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((gs_osal_num_of_thread_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        gsp_osal_thread_control[loop_count].usr_config    = p_thread_config_index[loop_count].p_usr_config;
        gsp_osal_thread_control[loop_count].inner_timeout = (osal_milli_sec_t)OSAL_RES_CFG_INNER_TIMEOUT;
        p_thread_control                                  = &gsp_osal_thread_control[loop_count];

        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &p_thread_control->inner_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            /* Do nothing */
        }
        else
        {
            osal_ret = R_OSAL_OS_CondStaticCreate(cond_attr, &p_thread_control->create_cond);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_CondStaticCreate(cond_attr, &p_thread_control->join_cond);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((OSAL_THREAD_PRIORITY_HIGHEST < p_thread_control->usr_config->config.priority) ||
                (OSAL_THREAD_PRIORITY_LOWEST >  p_thread_control->usr_config->config.priority) ||
                (0U == p_thread_control->usr_config->config.stack_size))
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                osal_ret = R_OSAL_OS_ThreadInitAttr(p_thread_control->usr_config->config.priority,
                                                    p_thread_control->usr_config->config.stack_size,
                                                    &thread_attr);
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            p_thread_control->active = false;

            /* PRQA S 0314 3 # No problem because it follows the specification of the calling function. */
            osal_ret = R_OSAL_OS_ThreadStaticCreate(&p_thread_control->usr_config->config,
                                                    r_osal_rcfg_internal_thread,
                                                    (void*)p_thread_control,
                                                    thread_attr,
                                                    &p_thread_control->thread);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_OS_ThreadDeinitAttr(thread_attr);
            }
            else
            {
                osal_ret = R_OSAL_OS_ThreadDeinitAttr(thread_attr);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    /* Do nothing */
                }
                else
                {
                    p_thread_control->handle_id     = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                    p_thread_control->parent_thread = false;
                    p_thread_control->func          = NULL;
                    p_thread_control->userarg       = NULL;
                    p_thread_control->thread_ret    = 0;

                    /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
                    osal_ret = R_OSAL_OS_ThreadSetSelf(p_thread_control->thread, (void *)p_thread_control);
                }
            }   /* end of if(OSAL_RETURN_OK != osal_ret). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        loop_count++;
    }   /* end of while((th_num > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_init_each_table()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_062
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_deinit_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_ThreadControlInit() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_deinit_attr(osal_os_mutexattr_t mutex_attr,
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
* End of function r_osal_rcfg_thread_deinit_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_063
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_notify_finish()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_ThreadControlDeinit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_notify_finish(st_osal_thread_control_t *p_thread_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_thread_control->inner_mtx, p_thread_control->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        p_thread_control->func    = NULL;
        p_thread_control->userarg = NULL;

        osal_ret = R_OSAL_OS_CondSignal(p_thread_control->create_cond);
        if (OSAL_RETURN_OK == osal_ret)
        {
            p_thread_control->active = true;
        }
        else
        {
            /* Do nothing */
        }

        local_ret = R_OSAL_OS_MutexUnlock(p_thread_control->inner_mtx);
        if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_notify_finish()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_064
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_deinit_system_resources()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_ThreadControlDeinit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_deinit_system_resources(st_osal_thread_control_t *p_thread_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t api_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_thread_control->inner_mtx, p_thread_control->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (NULL != p_thread_control->create_cond)
        {
            api_ret = R_OSAL_OS_CondDestroy(p_thread_control->create_cond);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                p_thread_control->create_cond = NULL;
            }
        }
        else
        {
            /* Do nothing */
        }

        if (NULL != p_thread_control->join_cond)
        {
            api_ret = R_OSAL_OS_CondDestroy(p_thread_control->join_cond);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == api_ret)
            {
                p_thread_control->join_cond = NULL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != p_thread_control->join_cond) */

        api_ret = R_OSAL_OS_MutexUnlock(p_thread_control->inner_mtx);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if (OSAL_RETURN_OK != osal_ret) */

    if (NULL != p_thread_control->inner_mtx)
    {
        api_ret = r_osal_rcfg_thread_destroy_mutex(p_thread_control);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == api_ret)
        {
            p_thread_control->inner_mtx = NULL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != p_thread_control->inner_mtx) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_deinit_system_resources()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_065
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_destroy_mutex()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_thread_deinit_system_resources() in order to improve the 
   result of static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_destroy_mutex(const st_osal_thread_control_t *p_thread_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          waiting_time = 0U;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_MutexDestroy(p_thread_control->inner_mtx);
    if (OSAL_RETURN_BUSY == osal_ret)
    {
        while (((size_t)OSAL_RES_CFG_MUTEX_DESTROY_TIMEOUT >= waiting_time) && (OSAL_RETURN_BUSY == osal_ret))
        {
            (void)R_OSAL_HIF_ThreadSleepForTimePeriod(OSAL_RES_CFG_THREAD_MS_SLEEP_TIME);
            waiting_time += (size_t)OSAL_RES_CFG_THREAD_MS_SLEEP_TIME;
            osal_ret      = R_OSAL_OS_MutexDestroy(p_thread_control->inner_mtx);
        }

        if (OSAL_RETURN_OK != osal_ret)
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
    }   /* end of if (OSAL_RETURN_BUSY == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_destroy_mutex()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_066
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_wait_user_thread()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_ThreadJoin() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_wait_user_thread(osal_thread_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true == handle->active)
    {
        if (false == handle->parent_thread)
        {
            handle->parent_thread = true;

            osal_ret = R_OSAL_OS_CondTimedWait(handle->join_cond, handle->inner_mtx, NULL);
            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->parent_thread = false;
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (true == handle->active). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_wait_user_thread()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_009]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_052
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_internal_thread()
***********************************************************************************************************************/
/* PRQA S 3673 1 # The thread must be generated by this I/F, so it cannot be changed. */
OSAL_STATIC void* r_osal_rcfg_internal_thread(void * p_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # There is no problem because it is just restored to the type at the time of function call. */
    st_osal_thread_control_t *p_control  = (st_osal_thread_control_t *)p_arg;
    e_osal_return_t          local_error;
    e_osal_return_t          api_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_control)
    {
        local_error = OSAL_RETURN_FAIL;
    }
    else
    {
        local_error = R_OSAL_OS_ThreadSetTaskName(p_control->usr_config->config.task_name);
        if (OSAL_RETURN_OK == local_error)
        {
            local_error = R_OSAL_OS_ThreadSetCpuAffinity(p_control->usr_config->id);
        }
        else
        {
            /* Do nothing */
        }
    }

    while ((OSAL_MANAGER_STATE_INITIALIZED == gs_osal_thread_is_initialized_enu) &&
           (OSAL_RETURN_OK == local_error))
    {
        api_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_control->inner_mtx, p_control->inner_timeout);
        if (OSAL_RETURN_OK == api_ret)
        {
            if (true != p_control->active)
            {
                api_ret = R_OSAL_OS_CondTimedWait(p_control->create_cond, p_control->inner_mtx, NULL);
                if (OSAL_RETURN_OK != api_ret)
                {
                    local_error = OSAL_RETURN_FAIL;
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

            api_ret = R_OSAL_OS_MutexUnlock(p_control->inner_mtx);
            if (OSAL_RETURN_OK != api_ret)
            {
                local_error = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            local_error = OSAL_RETURN_FAIL;
        }   /* end of if(OSAL_RETURN_OK == local_error). */

        if ((OSAL_RETURN_OK == local_error) && (NULL != p_control->func))
        {
            p_control->thread_ret = p_control->func(p_control->userarg);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == local_error)
        {
            local_error = r_osal_rcfg_thread_notify_join(p_control);
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == local_error) */
    }   /* end of while ((NULL != p_control) && (OSAL_MANAGER_STATE_UNINITIALIZED != gs_osal_thread_is_initialized_enu) &&
                        (OSAL_RETURN_OK == local_error)). */
    return NULL;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_internal_thread()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_067
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_notify_join()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_internal_thread() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_thread_notify_join(st_osal_thread_control_t *p_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t local_error = OSAL_RETURN_OK;
    e_osal_return_t api_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_control->inner_mtx, p_control->inner_timeout);
    if (OSAL_RETURN_OK == api_ret)
    {
        p_control->active =  false;

        if (false != p_control->parent_thread)
        {
            api_ret = R_OSAL_OS_CondSignal(p_control->join_cond);
            if (OSAL_RETURN_OK != api_ret)
            {
                local_error = OSAL_RETURN_FAIL;
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

        api_ret = R_OSAL_OS_MutexUnlock(p_control->inner_mtx);
        if (OSAL_RETURN_OK != api_ret)
        {
            local_error = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == api_ret). */

    return local_error;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_notify_join()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_054
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_thread_search()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_thread_search(const void *p_key, 
                                          const void *p_search_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 2 # There is no problem because it is just restored to the type at the time of function call. */
    osal_thread_id_t                key        = *(const osal_thread_id_t *)p_key;
    const st_osal_thread_control_t  *p_control = (const st_osal_thread_control_t *)p_search_info;
    int                             result; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (key < p_control->usr_config->id)
    {
        result = -1;
    }
    else if (key > p_control->usr_config->id)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_thread_search()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_014]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_014]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
