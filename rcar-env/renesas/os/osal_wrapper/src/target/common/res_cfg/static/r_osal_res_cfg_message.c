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
* File Name :    r_osal_res_cfg_message.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configuration Functions for Message Manager
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
 * @typedef st_osal_mq_config_addr_t
 * @brief structure to message config address.
 **********************************************************************************************************************/
typedef struct
{
    const struct st_osal_rcfg_mq_config *p_usr_config;
}st_osal_mq_config_addr_t;

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * @var gs_osal_mq_is_initialized_enu
 * @brief Message Queue Manager initial state flag.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_osal_mq_is_initialized_enu = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * @var gs_osal_num_of_mq_table
 * @brief Number of the message queue control information table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_num_of_mq_table = 0U;

/***********************************************************************************************************************
 * @var gsp_osal_mq_control
 * @brief The value for the user to operate the message queue without being aware of the message queue information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_mq_control_t *gsp_osal_mq_control = NULL;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Initialize mutex, condition variable attribute.
* @param[in,out]    p_mutex_attr    The pointer of mutex attributes.
* @param[in,out]    p_mq_attr       The pointer of message queue inner attributes.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_init_attr(osal_os_mutexattr_t *p_mutex_attr,
                                                     osal_os_mqattr_t *p_mq_attr);

/***********************************************************************************************************************
* @brief            Create message queue control information table
* @param[in,out]    pp_mq_config_index      The struct of st_osal_mq_config_addr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_create_ctrl_table(st_osal_mq_config_addr_t **pp_mq_config_index);

/***********************************************************************************************************************
* @brief            Sort configuration table for message queue.
* @param[in]        array_num           The num of array.
* @param[in,out]    p_mq_config_addr    The pointer of st_osal_mq_config_addr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_sort_config(size_t array_num,
                                                       st_osal_mq_config_addr_t *p_mq_config_addr);

/***********************************************************************************************************************
* @brief            Deinitialize mutex, condition variable attribute.
* @param[in]        mutex_attr    The pointer of mutex attributes.
* @param[in]        mq_attr       The pointer of message queue inner attributes.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_deinit_attr(osal_os_mutexattr_t mutex_attr,
                                                       osal_os_mqattr_t mq_attr);

/***********************************************************************************************************************
* @brief            Deinit message queue control information table
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_deinit_ctrl_table(void);

/***********************************************************************************************************************
* @brief            Destroy mutex object
* @param[in,out]    p_mq_control       The struct of st_osal_mq_control_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_destroy_mutex(const st_osal_mq_control_t *p_mq_control);

/***********************************************************************************************************************
* @brief            Compare function for the argument of R_OSAL_OS_BinarySearch().
* @param[in]        p_key           The key.
* @param[in]        p_search_info   The search information.
* @return           int
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_mq_search(const void *p_key, 
                                      const void *p_search_info);

/***********************************************************************************************************************
* @brief            Message queue active check.
* @param[in]        handle       The struct of osal_mq_handle_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_active_check_cmn(osal_mq_handle_t handle);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_041
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_RCFG_MqGetInitStatus(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    return gs_osal_mq_is_initialized_enu;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_042
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_RCFG_MqSetInitStatus(e_osal_init_status_t status)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_mq_is_initialized_enu = status;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_002]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_043
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqControlInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret;
    st_osal_mq_control_t     *p_mq_control;
    size_t                   num_of_info        = 0U;
    st_osal_mq_config_addr_t *p_mq_config_index = NULL;
    osal_os_mutexattr_t      mutex_attr         = NULL;
    osal_os_mqattr_t         mq_attr            = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_rcfg_mq_init_attr(&mutex_attr, &mq_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_create_ctrl_table(&p_mq_config_index);
    }
    else
    {
        /* Do nothing */
    }

    while ((gs_osal_num_of_mq_table > num_of_info) && (OSAL_RETURN_OK == osal_ret))
    {
        gsp_osal_mq_control[num_of_info].usr_config    = p_mq_config_index[num_of_info].p_usr_config;
        gsp_osal_mq_control[num_of_info].inner_timeout = (osal_milli_sec_t)OSAL_RES_CFG_INNER_TIMEOUT;
        p_mq_control = &gsp_osal_mq_control[num_of_info];

        if ((0U == p_mq_control->usr_config->config.max_num_msg) ||
            (0U == p_mq_control->usr_config->config.msg_size))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &p_mq_control->inner_mtx);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_OS_MqCreate(&p_mq_control->usr_config->config, mq_attr, &p_mq_control->mq);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)R_OSAL_OS_MutexDestroy(p_mq_control->inner_mtx);
                    p_mq_control->inner_mtx = NULL;
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
                p_mq_control->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                p_mq_control->active    = false;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (0U == p_mq_control->usr_config->config.max_num_msg) || (0U == p_mq_control->usr_config->config.msg_size) */
        num_of_info++;
    }   /* end of while ((gs_osal_num_of_mq_table > num_of_info) && (OSAL_RETURN_OK == osal_ret)) */

    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
    R_OSAL_OS_Free((void*)p_mq_config_index);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_deinit_attr(mutex_attr, mq_attr);
    }
    else
    {
        (void)r_osal_rcfg_mq_deinit_attr(mutex_attr, mq_attr);
        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gsp_osal_mq_control);
        gsp_osal_mq_control = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_003]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_044
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqControlDeinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_osal_mq_control)
    {
        osal_ret = r_osal_rcfg_mq_deinit_ctrl_table();

        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        R_OSAL_OS_Free((void*)gsp_osal_mq_control);
        gsp_osal_mq_control     = NULL;
        gs_osal_num_of_mq_table = 0;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_004]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_045
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t            osal_ret      = OSAL_RETURN_OK;
    const st_osal_mq_control_t *p_mq_control;
    size_t                     num_of_info   = 0U;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((gs_osal_num_of_mq_table > num_of_info) && (OSAL_RETURN_OK == osal_ret))
    {
        p_mq_control = &gsp_osal_mq_control[num_of_info];
        osal_ret     = R_OSAL_OS_MqCheckBusy(p_mq_control->mq);

        num_of_info++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_005]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_046
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqCreate(const st_osal_mq_config_t * const p_config,
                                     osal_mq_id_t mq_Id,
                                     osal_mq_handle_t * const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret;
    e_osal_return_t      local_ret;
    st_osal_mq_control_t *p_mq_control;
    osal_mq_id_t         local_mq_id = mq_Id;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)p_config; /* unused */

    /* PRQA S 0314 3 # No problem because it follows the specification of the calling function. */
    /* PRQA S 0316 1 # No problem because it is returned to the type of the second argument. */
    p_mq_control = (st_osal_mq_control_t *)R_OSAL_OS_BinarySearch((const void*)&local_mq_id,
                                                                  (const void*)gsp_osal_mq_control,
                                                                  gs_osal_num_of_mq_table,
                                                                  sizeof(st_osal_mq_control_t),
                                                                  r_osal_rcfg_mq_search);
    if (NULL == p_mq_control)
    {
        osal_ret = OSAL_RETURN_ID;
    }
    else
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mq_control->inner_mtx, p_mq_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Check if the block is released after the de-initialization. */
            if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_mq_is_initialized_enu)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else if (true == p_mq_control->active)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                p_mq_control->handle_id = (uint64_t)OSAL_RES_CFG_MESSAGE_HANDLE_ID;
                p_mq_control->active    = true;
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_mq_control->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (osal_ret == OSAL_RETURN_OK))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }   /* end of if (NULL == p_mq_control) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2916 1 # Since it is the address of the dynamically allocated area, there is no problem. */
        *p_handle = p_mq_control;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_006]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_047
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqDelete()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_MqDelete(osal_mq_handle_t handle)
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
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 9 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret  = R_OSAL_HIF_MutexLockForTimePeriod(handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_OS_MqCheckBusy(handle->mq);
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                handle->active    = false;
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
        }   /* end of if (OSAL_RETURN_OK != osal_ret). */
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqDelete()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_007]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_048
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqGetConfig()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MqGetConfig(osal_mq_handle_t handle,
                                        st_osal_mq_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) || (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
        if (OSAL_RETURN_OK == osal_ret)
        {
            p_config->max_num_msg = handle->usr_config->config.max_num_msg;
            p_config->msg_size    = handle->usr_config->config.msg_size;
        }
        else
        {
            /* Do nothing */
        }
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqGetConfig()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_008]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_049
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqIsEmpty()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MqIsEmpty(osal_mq_handle_t handle,
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
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) || (NULL == handle->usr_config))
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
        osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_MqIsEmpty(handle->mq, p_result);
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
* End of function R_OSAL_RCFG_MqIsEmpty()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_009]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_050
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqIsFull()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_RCFG_MqIsFull(osal_mq_handle_t handle,
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
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) || (NULL == handle->usr_config))
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
        osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_MqIsFull(handle->mq, p_result);
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
* End of function R_OSAL_RCFG_MqIsFull()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_010]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_051
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqReceiveForTimePeriod()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
e_osal_return_t R_OSAL_RCFG_MqReceiveForTimePeriod(osal_mq_handle_t handle,
                                                   osal_milli_sec_t time_period,
                                                   void* p_buffer,
                                                   size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (handle->usr_config->config.msg_size != buffer_size)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
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
        osal_ret = R_OSAL_HIF_MqReceiveUntilTimeStamp(handle->mq, &time_stamp, p_buffer, buffer_size);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqReceiveForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_011]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_052
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqReceiveUntilTimeStamp()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
e_osal_return_t R_OSAL_RCFG_MqReceiveUntilTimeStamp(osal_mq_handle_t handle,
                                                    const st_osal_time_t* const p_time_stamp,
                                                    void* p_buffer,
                                                    size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (handle->usr_config->config.msg_size != buffer_size)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }  /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
                     ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
                     (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MqReceiveUntilTimeStamp(handle->mq, p_time_stamp, p_buffer, buffer_size);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqReceiveUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_053
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqReset()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
e_osal_return_t R_OSAL_RCFG_MqReset(osal_mq_handle_t handle)
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
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 9 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
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
            osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_OS_MqReset(handle->mq);
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
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqReset()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_013]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_054
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqSendForTimePeriod()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
e_osal_return_t R_OSAL_RCFG_MqSendForTimePeriod(osal_mq_handle_t handle,
                                                osal_milli_sec_t time_period,
                                                const void* p_buffer,
                                                size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (handle->usr_config->config.msg_size != buffer_size)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MqSendUntilTimeStamp(handle->mq, &time_stamp, p_buffer, buffer_size);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqSendForTimePeriod()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_014]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_055
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_MqSendUntilTimeStamp()
***********************************************************************************************************************/
/* PRQA S 3673 1 # This I/F is specified in the requirement specification, and const cannot be assigned. */
e_osal_return_t R_OSAL_RCFG_MqSendUntilTimeStamp(osal_mq_handle_t handle,
                                                 const st_osal_time_t* const p_time_stamp,
                                                 const void* p_buffer,
                                                 size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_RES_CFG_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (handle->usr_config->config.msg_size != buffer_size)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gsp_osal_mq_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gsp_osal_mq_control[(gs_osal_num_of_mq_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_rcfg_mq_active_check_cmn(handle);
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MqSendUntilTimeStamp(handle->mq, p_time_stamp, p_buffer, buffer_size);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_MqSendUntilTimeStamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_015]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_015]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_066
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_init_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MqControlInit() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_init_attr(osal_os_mutexattr_t *p_mutex_attr,
                                                     osal_os_mqattr_t *p_mq_attr)
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
        osal_ret = R_OSAL_OS_MqInitAttr(p_mq_attr);
        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)R_OSAL_OS_MutexDeinitAttr(*p_mutex_attr);
            *p_mutex_attr = NULL;
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
* End of function r_osal_rcfg_mq_init_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_067
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_create_ctrl_table()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MqControlInit() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_create_ctrl_table(st_osal_mq_config_addr_t **pp_mq_config_index)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_addr_t *p_mq_config_addr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_num_of_mq_table = 0;
    while((uint64_t)OSAL_MQ_INVALID_ID != g_osal_mq_config[gs_osal_num_of_mq_table].id)
    {
        gs_osal_num_of_mq_table++;
    }

    if (0U != gs_osal_num_of_mq_table)
    {
        /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
        p_mq_config_addr = (st_osal_mq_config_addr_t*)R_OSAL_OS_Malloc(sizeof(st_osal_mq_config_addr_t) * gs_osal_num_of_mq_table);
        if (NULL == p_mq_config_addr)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            osal_ret = r_osal_rcfg_mq_sort_config(gs_osal_num_of_mq_table, p_mq_config_addr);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
            gsp_osal_mq_control = (st_osal_mq_control_t *)R_OSAL_OS_Malloc((sizeof(st_osal_mq_control_t) * gs_osal_num_of_mq_table));
            if (NULL == gsp_osal_mq_control)
            {
                osal_ret = OSAL_RETURN_MEM;
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                R_OSAL_OS_Free((void*)p_mq_config_addr);
            }
            else
            {
                *pp_mq_config_index = p_mq_config_addr;
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
    } /* end of if (gs_osal_num_of_mq_table != 0) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_create_ctrl_table()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_068
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_sort_config()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mq_create_ctrl_table() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_sort_config(size_t array_num,
                                                       st_osal_mq_config_addr_t *p_mq_config_addr)
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
    p_mq_config_addr[0U].p_usr_config = &g_osal_mq_config[0U];
    for (loop_count = 1U; ((loop_count < array_num) && (OSAL_RETURN_OK == osal_ret)); loop_count++)
    {
        tmp = g_osal_mq_config[loop_count].id;
        for (cmp_count = loop_count; ((cmp_count > 0U) && (OSAL_RETURN_OK == osal_ret)); cmp_count--)
        {
            if (p_mq_config_addr[(cmp_count - 1U)].p_usr_config->id > tmp)
            {
                p_mq_config_addr[cmp_count].p_usr_config = p_mq_config_addr[(cmp_count - 1U)].p_usr_config;
            }
            else if (p_mq_config_addr[(cmp_count - 1U)].p_usr_config->id == tmp)
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                break;
            }
        }
        p_mq_config_addr[cmp_count].p_usr_config = &g_osal_mq_config[loop_count];
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_sort_config()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_069
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_deinit_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MqControlInit() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_deinit_attr(osal_os_mutexattr_t mutex_attr,
                                                       osal_os_mqattr_t mq_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != mq_attr)
    {
        osal_ret = R_OSAL_OS_MqDeinitAttr(mq_attr);
    }
    else
    {
        /* Do nothing */
    }

    if (NULL != mutex_attr)
    {
        api_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
        if ((OSAL_RETURN_OK == osal_ret) && (OSAL_RETURN_OK != api_ret))
        {
            osal_ret = api_ret;
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
* End of function r_osal_rcfg_mq_deinit_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_070
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_deinit_ctrl_table()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_RCFG_MqControlDeinit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_deinit_ctrl_table(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret      = OSAL_RETURN_OK;
    e_osal_return_t      api_ret;
    size_t               num_of_info   = 0;
    st_osal_mq_control_t *p_mq_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((gs_osal_num_of_mq_table > num_of_info) && (OSAL_RETURN_OK == osal_ret))
    {
        p_mq_control = &gsp_osal_mq_control[num_of_info];

        if (NULL != p_mq_control->inner_mtx)
        {
            osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_mq_control->inner_mtx, p_mq_control->inner_timeout);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                osal_ret = R_OSAL_OS_MqDelete(p_mq_control->mq);

                p_mq_control->handle_id = (uint64_t)OSAL_RES_CFG_INVALID_RES_HANDLE_ID;
                p_mq_control->active    = false;

                api_ret = R_OSAL_OS_MutexUnlock(p_mq_control->inner_mtx);
                if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }   /* end of if (OSAL_RETURN_OK != osal_ret). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != p_mq_control->inner_mtx). */

        if (NULL != p_mq_control->inner_mtx)
        {
            api_ret = r_osal_rcfg_mq_destroy_mutex(p_mq_control);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                p_mq_control->inner_mtx = NULL;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != p_mq_control->inner_mtx). */
        num_of_info++;
    }   /* end of while ((gs_osal_num_of_mq_table > num_of_info) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_deinit_ctrl_table()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_071
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_destroy_mutex()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_rcfg_mq_deinit_ctrl_table() in order to improve the result of
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_destroy_mutex(const st_osal_mq_control_t *p_mq_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int32_t         waiting_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_MutexDestroy(p_mq_control->inner_mtx);
    if (OSAL_RETURN_OK != osal_ret)
    {
        waiting_time = 0;
        while ((OSAL_RES_CFG_MUTEX_DESTROY_TIMEOUT > waiting_time) && (OSAL_RETURN_BUSY == osal_ret))
        {
            (void)R_OSAL_HIF_ThreadSleepForTimePeriod(OSAL_RES_CFG_MESSAGE_MS_SLEEP_TIME);
            waiting_time += OSAL_RES_CFG_MESSAGE_MS_SLEEP_TIME;
            osal_ret = R_OSAL_OS_MutexDestroy(p_mq_control->inner_mtx);
        }   /* end of while ((OSAL_RES_CFG_MUTEX_DESTROY_TIMEOUT > waiting_time) && (OSAL_RETURN_BUSY == osal_ret)) */

        if ((OSAL_RES_CFG_MUTEX_DESTROY_TIMEOUT <= waiting_time) || (OSAL_RETURN_OK != osal_ret))
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
    }   /* end of if (OSAL_RETURN_OK != osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_destroy_mutex()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_063
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_search()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_rcfg_mq_search(const void *p_key, 
                                      const void *p_search_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 2 # There is no problem because it is just restored to the type at the time of function call. */
    osal_mq_id_t               key        = *(const osal_mq_id_t *)p_key;
    const st_osal_mq_control_t *p_control = (const st_osal_mq_control_t *)p_search_info;
    int                        result;  /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

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
* End of function r_osal_rcfg_mq_search()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_023]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_064
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_rcfg_mq_active_check_cmn()
***********************************************************************************************************************/
/* PRQA S 9106 2 # This function is intended to be used in common. */
/* PRQA S 3673 1 # It does not rewrite the arguments. */
OSAL_STATIC e_osal_return_t r_osal_rcfg_mq_active_check_cmn(osal_mq_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* Check if the block is released after the de-initialization. */
    if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_mq_is_initialized_enu)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (false == handle->active)
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_rcfg_mq_active_check_cmn()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_045]
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_045]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
