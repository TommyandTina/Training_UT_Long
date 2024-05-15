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
* File Name :    r_osal_dev_cfg_power.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Resource Configuration Functions Power Related Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <string.h>

#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var gs_osal_pm_is_initialized_enu
 * @brief Power Manager initial state flag.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_osal_pm_is_initialized_enu = OSAL_MANAGER_STATE_UNINITIALIZED;

/*******************************************************************************************************************//**
 * @var gs_osal_pma_handle
 * @brief Handle of Power Manager Agent.
***********************************************************************************************************************/
OSAL_STATIC st_pma_handle_t gs_osal_pma_handle;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            Get Power status.
* @param[in]        pma_handle  Power Manager Agent handle
* @param[in]        pma_hwaId   PMA ID
* @param[out]       p_state     A pointer to store the power status.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_get_state(st_pma_handle_t pma_handle,
                                                     e_pma_hwa_id_t pma_hwaId,
                                                     e_osal_pm_state_t * const p_state);

/*******************************************************************************************************************//**
* @brief            Convert from OSAL PM status to PMA status.
* @param[in]        osal_pm_state   OSAL PM sate
* @param[out]       p_pma_hwa_state A pointer to store the PMA HWA state.
* @param[out]       p_pma_reset_req A pointer to store the PMA Reset request
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_convert_wait_state(e_osal_pm_state_t osal_pm_state,
                                                              st_pma_hwa_state_t *p_pma_hwa_state,
                                                              e_pma_reset_req_t *p_pma_reset_req);

/*******************************************************************************************************************//**
* @brief            Wait until specified status
* @param[in]        handle              Device handle
* @param[in]        p_wait_hwa_state    Wait hwa state
* @param[in]        p_wait_reset_req    Wait reset request
* @param[in]        time_period         Time period
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_wait_for_state(osal_device_handle_t handle,
                                                          const st_pma_hwa_state_t *p_wait_hwa_state,
                                                          e_pma_reset_req_t wait_reset_req,
                                                          osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief            Check wait enforcement
* @param[in]        handle          Device handle
* @param[in]        pma_policy      PMA policy
* @param[in]        applyImmediate  Immediate Flag
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_check_policy_wait_conditions(osal_device_handle_t handle,
                                                                        e_pma_power_policy_t pma_policy,
                                                                        bool applyImmediate);

/*******************************************************************************************************************//**
* @brief            Wait until specified policy
* @param[in]        handle          Device handle
* @param[in]        pma_policy      PMA policy
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_wait_for_policy(osal_device_handle_t handle,
                                                           e_pma_power_policy_t pma_policy);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_041
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_PmGetInitStatus(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    return gs_osal_pm_is_initialized_enu;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_002]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_042
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_DCFG_PmSetInitStatus(e_osal_init_status_t status)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_pm_is_initialized_enu = status;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_003]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_043
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmControlInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    e_pma_return_t           pma_ret;
    st_pma_config_t          pma_config;
    size_t                   dev_count         = 0;
    st_osal_device_control_t *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    pma_config.task_priority           = OSAL_THREAD_PRIORITY_HIGHEST;
    pma_config.timeout_ms              = OSAL_DEV_CFG_PM_TIMEOUT;
    pma_config.polling_max_num         = OSAL_DEV_CFG_PM_POLLING_MAX_NUM;
    pma_config.loop_num_for_rclk_cycle = OSAL_DEV_CFG_PM_LOOP_NUM_FOR_RCLK_CYCLE;

    pma_ret = R_PMA_Init(&gs_osal_pma_handle, &pma_config);
    if (R_PMA_ERR_NO_VACANT_HANDLE == pma_ret)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((R_PMA_ERR_DEVICE == pma_ret) || (R_PMA_ERR_IO == pma_ret))
    {
        osal_ret = OSAL_RETURN_DEV;
    }
    else if (R_PMA_RESULT_OK != pma_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    while ((g_osal_num_of_device_table > dev_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_device_control                = &gp_osal_device_control[dev_count];
        p_device_control->pma_handle    = gs_osal_pma_handle;
        p_device_control->pm_wait_state = false;

        if (R_PMA_HWA_ID_NONE != p_device_control->dev_info->pma_id)
        {
            pma_ret = R_PMA_GetLockHwa(p_device_control->pma_handle, p_device_control->dev_info->pma_id);
            if (R_PMA_ERR_INVALID_ARGUMENT == pma_ret)
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else if ((R_PMA_RESULT_OK != pma_ret) && (R_PMA_ERR_ALREADY_IN_USE != pma_ret))
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
            /* the device is not supported by PMA, do nothing */
        }   /* end of if (R_PMA_HWA_ID_NONE != p_device_control->dev_info->pma_id) */
        dev_count++;
    }   /* end of while ((g_osal_num_of_device_table > dev_count) && (OSAL_RETURN_OK == osal_ret)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_004]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_004]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_044
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmControlDeinit(void)
{
     /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    e_pma_return_t           pma_ret;
    size_t                   dev_count         = 0;
    st_osal_device_control_t *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while (g_osal_num_of_device_table > dev_count)
    {
        p_device_control = &gp_osal_device_control[dev_count];
        if (NULL != p_device_control->pma_handle)
        {
            (void)R_PMA_UnlockHwa(p_device_control->pma_handle, p_device_control->dev_info->pma_id);
        }
        else
        {
            /* Do nothing */
        }
        p_device_control->pma_handle    = NULL;
        p_device_control->pm_wait_state = false;

        dev_count++;
    }   /* end of while (g_osal_num_of_device_table > dev_count) */

    if (NULL != gs_osal_pma_handle)
    {
        pma_ret = R_PMA_Quit(gs_osal_pma_handle);
        if (R_PMA_RESULT_OK != pma_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            gs_osal_pma_handle = NULL;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != gs_osal_pma_handle) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_005]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_005]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_045
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret          = OSAL_RETURN_OK;
    e_osal_return_t                 local_ret;
    size_t                          dev_count         = 0;
    const st_osal_device_control_t  *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((g_osal_num_of_device_table > dev_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_device_control = &gp_osal_device_control[dev_count];

        local_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_device_control->inner_mtx, p_device_control->inner_timeout);
        if (OSAL_RETURN_OK != local_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (true == p_device_control->pm_wait_state)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_device_control->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = local_ret;
            }
            else
            {
                 /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != local_ret) */
        dev_count++;
    }   /* end of while ((g_osal_num_of_device_table > dev_count) && (OSAL_RETURN_OK == osal_ret)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_006]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_046
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetRequiredState()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_PmGetRequiredState(osal_device_handle_t handle,
                                               e_osal_pm_required_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret;
    e_osal_return_t    local_ret;
    e_pma_return_t     pma_ret;
    st_pma_hwa_state_t pma_status;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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
            pma_ret = R_PMA_GetHwaState(handle->pma_handle, handle->dev_info->pma_id, &pma_status);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                if (R_PMA_ERR_UNSUPPORTED_OPERATION == pma_ret)
                {
                    osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                }
                else
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }
            else
            {
                switch (pma_status.enReqState)
                {
                    case R_PMA_HWA_REQUESTED:
                        *p_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
                        break;
                    case R_PMA_HWA_RELEASED:
                        *p_state = OSAL_PM_REQUIRED_STATE_RELEASED;
                        break;
                    default:
                        *p_state = OSAL_PM_REQUIRED_STATE_INVALID;
                        break;
                }
            }   /* end of if(R_PMA_RESULT_OK != pma_ret). */

            local_ret = R_OSAL_OS_MutexUnlock(handle->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = local_ret;
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
* End of function R_OSAL_DCFG_PmGetRequiredState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_009]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_047
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetRequiredState()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_PmSetRequiredState(osal_device_handle_t handle,
                                               e_osal_pm_required_state_t state,
                                               bool applyImmediate)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret;
    e_osal_return_t    local_ret;
    e_pma_return_t     pma_ret;
    st_pma_hwa_state_t pma_hwa_state;
    e_pma_reset_req_t  pma_reset_req;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) ||(OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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
            if (OSAL_PM_REQUIRED_STATE_REQUIRED == state)
            {
                pma_ret = R_PMA_RequestHwa(handle->pma_handle, handle->dev_info->pma_id);
                pma_hwa_state.enReqState      = R_PMA_HWA_REQUESTED;
                pma_hwa_state.enActualClkStat = R_PMA_STATE_ON;
                pma_hwa_state.enActualPwrStat = R_PMA_STATE_ON;
                pma_reset_req                 = R_PMA_UNDEFINED_RESET;
            }
            else
            {
                pma_ret = R_PMA_ReleaseHwa(handle->pma_handle, handle->dev_info->pma_id);
                pma_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
                pma_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
                pma_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
                pma_reset_req                 = R_PMA_UNDEFINED_RESET;
            }

            if ((R_PMA_RESULT_OK == pma_ret) && (true == applyImmediate))
            {
                pma_ret = R_PMA_WaitForState(handle->pma_handle,
                                             handle->dev_info->pma_id,
                                             &pma_hwa_state,
                                             pma_reset_req,
                                             OSAL_DEV_CFG_POWER_MS_SLEEP_TIME,
                                             OSAL_DEV_CFG_POWER_MS_SLEEP_RETRY,
                                             0);
            }
            else
            {
                /* Do nothing */
            }

            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
                    /* Do nothing */
                    break;
                case R_PMA_ERR_TIMEOUT:
                    osal_ret = OSAL_RETURN_TIME;
                    break;
                case R_PMA_ERR_DEVICE:
                    osal_ret = OSAL_RETURN_DEV;
                    break;
                case R_PMA_ERR_UNSUPPORTED_OPERATION:
                    osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                    break;
                default:
                    osal_ret = OSAL_RETURN_FAIL;
                    break;
            }

            local_ret = R_OSAL_OS_MutexUnlock(handle->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = local_ret;
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetRequiredState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_014]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_048
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmWaitForState()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmWaitForState(osal_device_handle_t handle,
                                           e_osal_pm_state_t state,
                                           osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret;
    st_pma_hwa_state_t wait_hwa_state;
    e_pma_reset_req_t  wait_reset_req;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = r_osal_dcfg_pm_convert_wait_state(state, &wait_hwa_state, &wait_reset_req);
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_pm_wait_for_state(handle, &wait_hwa_state, wait_reset_req, time_period);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmWaitForState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_016]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_049
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetState()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_PmGetState(osal_device_handle_t handle,
                                       e_osal_pm_state_t* const p_state)
{
   /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t   osal_ret;
    e_osal_return_t   local_ret;
    e_osal_pm_state_t pm_state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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
            osal_ret = r_osal_dcfg_pm_get_state(handle->pma_handle, handle->dev_info->pma_id, &pm_state);
            if (OSAL_RETURN_OK != osal_ret)
            {
                *p_state = OSAL_PM_STATE_INVALID;
            }
            else
            {
                 *p_state = pm_state;
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_011]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_050
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetLowestIntermediatePowerState()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_PmGetLowestIntermediatePowerState(osal_device_handle_t handle,
                                                              e_osal_pm_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    e_osal_return_t     local_ret;
    e_pma_return_t      pma_ret;
    e_pma_power_state_t lowest_power;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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
            pma_ret = R_PMA_GetLowestPowerState(handle->pma_handle, handle->dev_info->pma_id, &lowest_power);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                *p_state = OSAL_PM_STATE_INVALID;
            }
            else
            {
                switch (lowest_power)
                {
                    case R_PMA_STATE_PG:
                        *p_state = OSAL_PM_STATE_PG;
                        break;
                    case R_PMA_STATE_CG:
                        *p_state = OSAL_PM_STATE_CG;
                        break;
                    case R_PMA_STATE_ENABLED:
                        *p_state = OSAL_PM_STATE_ENABLED;
                        break;
                    default:
                        *p_state = OSAL_PM_STATE_INVALID;
                        break;
                }
            }   /* end of if(R_PMA_RESULT_OK != pma_ret). */

            local_ret = R_OSAL_OS_MutexUnlock(handle->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = local_ret;
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetLowestIntermediatePowerState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_007]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_051
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetPolicy()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_PmGetPolicy(osal_device_handle_t handle, 
                                        e_osal_pm_policy_t* const p_policy)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret;
    e_osal_return_t      local_ret;
    e_pma_return_t       pma_ret;
    e_pma_power_policy_t pma_policy;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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
            pma_ret = R_PMA_GetPowerPolicy(handle->pma_handle, handle->dev_info->pma_id, &pma_policy);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                if (R_PMA_ERR_UNSUPPORTED_OPERATION == pma_ret)
                {
                    osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                }
                else
                {
                    osal_ret  = OSAL_RETURN_FAIL;
                    *p_policy = OSAL_PM_POLICY_INVALID;
                }
            }
            else
            {
                switch (pma_policy)
                {
                    case R_PMA_PG_MODE:
                        *p_policy = OSAL_PM_POLICY_PG;
                        break;
                    case R_PMA_CG_MODE:
                        *p_policy = OSAL_PM_POLICY_CG;
                        break;
                    case R_PMA_HP_MODE:
                        *p_policy = OSAL_PM_POLICY_HP;
                        break;
                    default:
                        *p_policy = OSAL_PM_POLICY_INVALID;
                        break;
                }
            }   /* end of if(R_PMA_RESULT_OK != pma_ret). */

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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetPolicy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_008]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_052
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetPolicy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetPolicy(osal_device_handle_t handle,
                                        e_osal_pm_policy_t policy,
                                        bool applyImmediate)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret;
    e_osal_return_t      local_ret;
    e_pma_return_t       pma_ret;
    e_pma_power_policy_t pma_policy;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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
            if (OSAL_PM_POLICY_PG == policy)
            {
                pma_policy = R_PMA_PG_MODE;
            }
            else if (OSAL_PM_POLICY_CG == policy)
            {
                pma_policy = R_PMA_CG_MODE;
            }
            else
            {
                pma_policy = R_PMA_HP_MODE;
            }

            pma_ret = R_PMA_SetPowerPolicy(handle->pma_handle, handle->dev_info->pma_id, pma_policy);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                if (R_PMA_ERR_UNSUPPORTED_OPERATION == pma_ret)
                {
                    osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                }
                else
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }
            else
            {
                osal_ret = r_osal_dcfg_pm_check_policy_wait_conditions(handle, pma_policy, applyImmediate);
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
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetPolicy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_053
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetResetState()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_PmGetResetState(osal_device_handle_t handle,
                                            e_osal_pm_reset_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t   osal_ret;
    e_osal_return_t   local_ret;
    e_pma_return_t    pma_ret;
    e_pma_reset_req_t pma_reset_state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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
            pma_ret = R_PMA_GetReset(handle->pma_handle, handle->dev_info->pma_id, &pma_reset_state);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                switch (pma_reset_state)
                {
                    case R_PMA_RELEASED_RESET:
                        *p_state = OSAL_PM_RESET_STATE_RELEASED;
                        break;
                    case R_PMA_APPLIED_RESET:
                        *p_state = OSAL_PM_RESET_STATE_APPLIED;
                        break;
                    default:
                        *p_state = OSAL_PM_RESET_STATE_INVALID;
                        break;
                }
            }   /* end of if(R_PMA_RESULT_OK != pma_ret). */

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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetResetState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_010]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_054
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetResetState()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_PmSetResetState(osal_device_handle_t handle,
                                            e_osal_pm_reset_state_t state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;
    e_pma_return_t  pma_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == handle->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->dev_info) || (NULL == handle->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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
            if (OSAL_PM_RESET_STATE_APPLIED == state)
            {
                pma_ret = R_PMA_ApplyResetHwa(handle->pma_handle, handle->dev_info->pma_id);
                switch (pma_ret)
                {
                    case R_PMA_RESULT_OK:
                        /* Do nothing */
                        break;
                    case R_PMA_ERR_DEVICE:
                        osal_ret = OSAL_RETURN_DEV;
                        break;
                    case R_PMA_ERR_IO:
                        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                        break;
                    default :
                        osal_ret = OSAL_RETURN_FAIL;
                        break;
                }
            }
            else
            {
                pma_ret = R_PMA_ReleaseResetHwa(handle->pma_handle, handle->dev_info->pma_id);
                switch (pma_ret)
                {
                    case R_PMA_RESULT_OK:
                        /* Do nothing */
                        break;
                    case R_PMA_ERR_DEVICE:
                        osal_ret = OSAL_RETURN_DEV;
                        break;
                    case R_PMA_ERR_IO:
                        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                        break;
                    default :
                        osal_ret = OSAL_RETURN_FAIL;
                        break;
                }
            }   /* end of if(OSAL_PM_RESET_STATE_APPLIED == state). */

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
    }   /* end of if (OSAL_RETURN_OK != osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetResetState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_015]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_055
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetPostClock()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_PmSetPostClock(osal_device_handle_t device,
                                           bool clock_enable,
                                           uint32_t clock_divider)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;
    e_pma_return_t  pma_ret;
    e_pma_clk_req_t enable;
    uint32_t        clock_divider_tmp;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             strncmp_fba;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             strncmp_fbc;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)device < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)device > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)device & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((false == device->active) || (OSAL_DEV_CFG_DEVICE_HANDLE_ID != device->handle_id) ||
             (NULL == device->dev_info) || (NULL == device->pma_handle))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (OSAL_PM_POSTCKCR_INVALID == device->dev_info->postckcr)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        strncmp_fbc = strncmp("fbc", device->dev_info->type, 3);
        strncmp_fba = strncmp("fba", device->dev_info->type, 3);
        if ((0 != strncmp_fbc) && (0 != strncmp_fba))
        {
            osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
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

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(device->inner_mtx, device->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (true == clock_enable)
            {
                enable = R_PMA_CLK_ENABLE;
            }
            else
            {
                enable = R_PMA_CLK_DISABLE;
            }

            clock_divider_tmp = clock_divider;
            pma_ret = R_PMA_SetPostClock(device->pma_handle, enable, (uint32_t)(device->dev_info->postckcr),
                                         &clock_divider_tmp);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }

            local_ret = R_OSAL_OS_MutexUnlock(device->inner_mtx);
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetPostClock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_013]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_013]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_056
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_get_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_get_state(st_pma_handle_t pma_handle,
                                                     e_pma_hwa_id_t pma_hwaId,
                                                     e_osal_pm_state_t * const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret  = OSAL_RETURN_OK;
    e_pma_return_t     pma_ret;
    st_pma_hwa_state_t pma_status;
    e_pma_reset_req_t  pma_reset_status;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    pma_ret = R_PMA_GetHwaState(pma_handle, pma_hwaId, &pma_status);
    if (R_PMA_RESULT_OK != pma_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
        *p_state = OSAL_PM_STATE_INVALID;
    }
    else
    {
        if (R_PMA_HWA_RELEASED == pma_status.enReqState)
        {
            if (R_PMA_STATE_ON == pma_status.enActualPwrStat)
            {
                if (R_PMA_STATE_ON == pma_status.enActualClkStat)
                {
                    *p_state = OSAL_PM_STATE_ENABLED;
                }
                else
                {
                    *p_state = OSAL_PM_STATE_CG;
                }
            }
            else
            {
                if (R_PMA_STATE_ON == pma_status.enActualClkStat)
                {
                    *p_state = OSAL_PM_STATE_INVALID;
                }
                else
                {
                    *p_state = OSAL_PM_STATE_PG;
                }
            }   /* end of if(R_PMA_STATE_ON == pma_status.enActualPwrStat). */
        }
        else
        {
            pma_ret = R_PMA_GetReset(pma_handle, pma_hwaId, &pma_reset_status);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                *p_state = OSAL_PM_STATE_INVALID;
            }
            else
            {
                if (R_PMA_APPLIED_RESET == pma_reset_status)
                {
                    *p_state = OSAL_PM_STATE_RESET;
                }
                else
                {
                    *p_state = OSAL_PM_STATE_READY;
                }
            }
        }   /* end of if(R_PMA_HWA_RELEASED == pma_status.enReqState). */
    }   /* end of if(R_PMA_RESULT_OK != pma_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_get_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_057
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_convert_wait_state()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_PmWaitForState() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_convert_wait_state(e_osal_pm_state_t osal_pm_state,
                                                              st_pma_hwa_state_t *p_pma_hwa_state,
                                                              e_pma_reset_req_t *p_pma_reset_req)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    switch (osal_pm_state)
    {
        case OSAL_PM_STATE_PG:
            p_pma_hwa_state->enReqState      = R_PMA_HWA_RELEASED;
            p_pma_hwa_state->enActualClkStat = R_PMA_STATE_OFF;
            p_pma_hwa_state->enActualPwrStat = R_PMA_STATE_OFF;
            *p_pma_reset_req                 = R_PMA_UNDEFINED_RESET;
            break;

        case OSAL_PM_STATE_CG:
            p_pma_hwa_state->enReqState      = R_PMA_HWA_RELEASED;
            p_pma_hwa_state->enActualClkStat = R_PMA_STATE_OFF;
            p_pma_hwa_state->enActualPwrStat = R_PMA_STATE_ON;
            *p_pma_reset_req                 = R_PMA_UNDEFINED_RESET;
            break;

        case OSAL_PM_STATE_ENABLED:
            p_pma_hwa_state->enReqState      = R_PMA_HWA_RELEASED;
            p_pma_hwa_state->enActualClkStat = R_PMA_STATE_ON;
            p_pma_hwa_state->enActualPwrStat = R_PMA_STATE_ON;
            *p_pma_reset_req                 = R_PMA_UNDEFINED_RESET;
            break;

        case OSAL_PM_STATE_RESET:
            p_pma_hwa_state->enReqState      = R_PMA_HWA_REQUESTED;
            p_pma_hwa_state->enActualClkStat = R_PMA_STATE_ON;
            p_pma_hwa_state->enActualPwrStat = R_PMA_STATE_ON;
            *p_pma_reset_req                 = R_PMA_APPLIED_RESET;
            break;

        case OSAL_PM_STATE_READY:
            p_pma_hwa_state->enReqState      = R_PMA_HWA_REQUESTED;
            p_pma_hwa_state->enActualClkStat = R_PMA_STATE_ON;
            p_pma_hwa_state->enActualPwrStat = R_PMA_STATE_ON;
            *p_pma_reset_req                 = R_PMA_RELEASED_RESET;
            break;

        default:
            osal_ret = OSAL_RETURN_PAR;
            break;
    }   /* end of switch(osal_pm_state). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_convert_wait_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_016]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_058
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_wait_for_state()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_PmWaitForState() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_wait_for_state(osal_device_handle_t handle,
                                                          const st_pma_hwa_state_t *p_wait_hwa_state,
                                                          e_pma_reset_req_t wait_reset_req,
                                                          osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret;
    e_osal_return_t  local_ret;
    e_pma_return_t   pma_ret;
    st_osal_time_t   time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &time_stamp);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(handle->inner_mtx, &time_stamp);
        if (OSAL_RETURN_OK == osal_ret)
        {
            handle->pm_wait_state = true;

            osal_ret = R_OSAL_OS_MutexUnlock(handle->inner_mtx);
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
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (0 == time_period)
        {
            pma_ret = R_PMA_WaitForState(handle->pma_handle, handle->dev_info->pma_id, p_wait_hwa_state,
                                         wait_reset_req, 0, 0, 0);
        }
        else
        {
            pma_ret = R_PMA_WaitForState(handle->pma_handle, handle->dev_info->pma_id, p_wait_hwa_state,
                                         wait_reset_req, OSAL_DEV_CFG_POWER_MS_SLEEP_TIME, time_period, 0);
        }

        osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(handle->inner_mtx, &time_stamp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            handle->pm_wait_state = false;
        }
        else
        {
            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
                    /* Do nothing */
                    break;
                case R_PMA_ERR_TIMEOUT:
                    osal_ret = OSAL_RETURN_TIME;
                    break;
                case R_PMA_ERR_DEVICE:
                    osal_ret = OSAL_RETURN_DEV;
                    break;
                default:
                    osal_ret = OSAL_RETURN_FAIL;
                    break;
            }

            handle->pm_wait_state = false;

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
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_wait_for_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_016]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_060
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_check_policy_wait_conditions()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_PmSetPolicy() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_check_policy_wait_conditions(osal_device_handle_t handle,
                                                                        e_pma_power_policy_t pma_policy,
                                                                        bool applyImmediate)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret = OSAL_RETURN_OK;
    e_pma_return_t       pma_ret;
    st_pma_hwa_state_t   pma_status;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    pma_ret = R_PMA_GetHwaState(handle->pma_handle, handle->dev_info->pma_id, &pma_status);
    if (R_PMA_RESULT_OK != pma_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if ((true == applyImmediate) && (R_PMA_HWA_RELEASED == pma_status.enReqState))
        {
            osal_ret = r_osal_dcfg_pm_wait_for_policy(handle, pma_policy);
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if (R_PMA_RESULT_OK != pma_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_check_policy_wait_conditions()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_POW_059
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_wait_for_policy()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_dcfg_pm_check_policy_wait_conditions() in order to improve
   the result of static code analysis (STMIF of HIS code metrics). */
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_wait_for_policy(osal_device_handle_t handle,
                                                           e_pma_power_policy_t pma_policy)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret = OSAL_RETURN_OK;
    e_pma_return_t       pma_ret;
    e_pma_power_policy_t check_pma_policy;
    st_pma_hwa_state_t   pma_status;
    e_pma_reset_req_t    reset_req;
    int32_t              wait_count;
    bool                 wait_end;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    switch (pma_policy)
    {
        case R_PMA_PG_MODE:
            pma_status.enReqState      = R_PMA_HWA_RELEASED;
            pma_status.enActualClkStat = R_PMA_STATE_OFF;
            pma_status.enActualPwrStat = R_PMA_STATE_OFF;
            reset_req                  = R_PMA_UNDEFINED_RESET;
            break;

        case R_PMA_CG_MODE:
            pma_status.enReqState      = R_PMA_HWA_RELEASED;
            pma_status.enActualClkStat = R_PMA_STATE_OFF;
            pma_status.enActualPwrStat = R_PMA_STATE_ON;
            reset_req                  = R_PMA_UNDEFINED_RESET;
            break;

        default: /* R_PMA_HP_MODE */
            pma_status.enReqState      = R_PMA_HWA_RELEASED;
            pma_status.enActualClkStat = R_PMA_STATE_ON;
            pma_status.enActualPwrStat = R_PMA_STATE_ON;
            reset_req                  = R_PMA_UNDEFINED_RESET;
            break;
    }   /* end of switch (pma_policy). */

    wait_end = false;
    for (wait_count = 0; (OSAL_RETURN_OK == osal_ret) && (OSAL_DEV_CFG_POWER_MS_SLEEP_RETRY > wait_count); wait_count++)
    {
        pma_ret = R_PMA_WaitForState(handle->pma_handle, handle->dev_info->pma_id, &pma_status,
                                     reset_req, OSAL_DEV_CFG_POWER_MS_SLEEP_TIME, 1, 0);
        if (R_PMA_ERR_TIMEOUT == pma_ret)
        {
            pma_ret = R_PMA_GetPowerPolicy(handle->pma_handle, handle->dev_info->pma_id, &check_pma_policy);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else if (pma_policy == check_pma_policy)
            {
                wait_end = true;
            }
            else
            {
                /* Do nothing */
            }
        }
        else if (R_PMA_ERR_DEVICE == pma_ret)
        {
            osal_ret = OSAL_RETURN_DEV;
        }
        else if (R_PMA_RESULT_OK != pma_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else /* R_PMA_RESULT_OK */
        {
            wait_end = true;
        }

        if (true == wait_end)
        {
            break;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of for (wait_count = 0; (OSAL_RETURN_OK == osal_ret)... */

    if (OSAL_DEV_CFG_POWER_MS_SLEEP_RETRY <= wait_count)
    {
        osal_ret = OSAL_RETURN_TIME;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_wait_for_policy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_POW_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_POW_012]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
