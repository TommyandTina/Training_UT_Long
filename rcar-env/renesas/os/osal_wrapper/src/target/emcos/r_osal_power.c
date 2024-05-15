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
* File Name :    r_osal_power.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper Power Manager for eMCOS
***********************************************************************************************************************/
#include <string.h>

#include "target/emcos/r_osal_common.h"
#include "target/emcos/pma/r_pma.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_PM Power Manager Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_pm_is_initialized
 * Initialize flag
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_pm_is_initialized = false;

/*******************************************************************************************************************//**
 * @var gs_osal_pma_handle
 * Handle of Power Manager Agent
***********************************************************************************************************************/
OSAL_STATIC st_pma_handle_t gs_osal_pma_handle;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_PM Power Manager Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Translate from OSAL PM state to PMA state
* @param[in]        osal_pm_state   OSAL PM sate
* @param[out]       p_pma_hwa_state A pointer to store the PMA HWA state.
* @param[out]       p_pma_reset_req A pointer to store the PMA Reset request
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_translate_state(e_osal_pm_state_t osal_pm_state,
                                                          st_pma_hwa_state_t *p_pma_hwa_state,
                                                          e_pma_reset_req_t *p_pma_reset_req);

/*******************************************************************************************************************//**
* @brief            Get Power status.
* @param[in]        pma_handle  Power Manager handle
* @param[in]        pma_hwaId   PMA ID
* @param[out]       p_state     A pointer to store the power status.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_get_state(st_pma_handle_t pma_handle,
                                                       e_pma_hwa_id_t pma_hwaId,
                                                       e_osal_pm_state_t * const p_state);

/*******************************************************************************************************************//**
* @brief            Intermidiate_Power_State Cechk.
* @param[in]        handle      Device handle
* @param[in]        state       PMA State
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_get_lowest_intermidiate_power_state_pre(osal_device_handle_t const handle,
                                                                                     e_osal_pm_state_t* const p_state);

/*******************************************************************************************************************//**
* @brief            Get Policy Cechk.
* @param[in]        handle      Device handle
* @param[in]        p_policy    PMA Policy
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_get_policy_pre(osal_device_handle_t const handle,
                                                e_osal_pm_policy_t* const p_policy);

/*******************************************************************************************************************//**
* @brief            Required State Cechk.
* @param[in]        handle      Device handle
* @param[in]        p_state     PMA Required State
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_pm_get_required_state_pre(osal_device_handle_t const handle,
                                                           e_osal_pm_required_state_t* const p_state);

/*******************************************************************************************************************//**
* @brief            Get Reset State Cechk.
* @param[in]        handle      Device handle
* @param[in]        p_state     PMA Reset State
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_pm_get_reset_state_pre(osal_device_handle_t const handle,
                                                        e_osal_pm_reset_state_t* const p_state);

/*******************************************************************************************************************//**
* @brief            Post Clock Check.
* @param[in]        handle      Device handle
* @param[in]        clock_divider     Clock Divider
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pmsetpostclock_pre(osal_device_handle_t const device,
                                                             const uint32_t clock_divider);

/*******************************************************************************************************************//**
* @brief            Set Required State Check.
* @param[in]        handle      Device handle
* @param[in]        state       PMA Required State
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_set_required_state_pre(osal_device_handle_t const handle,
                                                                    e_osal_pm_required_state_t const state);

/*******************************************************************************************************************//**
* @brief            Set Reset State Cechk.
* @param[in]        handle      Device handle
* @param[in]        state       PMA Reset State
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_set_reset_state_pre(osal_device_handle_t const handle,
                                                                 e_osal_pm_reset_state_t const state);

/*******************************************************************************************************************//**
* @brief            PM Deinitialize Proccess.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_deinitialize_pre_proc(void);

/*******************************************************************************************************************//**
* @brief            Set Policy  Cechk.
* @param[in]        handle      Device handle
* @param[in]        policy      PMA Policy
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_set_policy_pre(osal_device_handle_t const handle,
                                                            e_osal_pm_policy_t const policy);

/*******************************************************************************************************************//**
* @brief            Set Policy Proccess.
* @param[in]        handle      Device handle
* @param[in]        policy      PMA Policy
* @param[in]        applyImmediate      Immediate Flag
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_set_policy_proc(osal_device_handle_t handle,
                                                             e_osal_pm_policy_t policy,
                                                             bool applyImmediate);
/*******************************************************************************************************************//**
* @brief            Set Policy Proccess.
* @param[in]        handle      Device handle
* @param[in]        policy      PMA Policy
* @param[in]        applyImmediate      Immediate Flag
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_set_policy_sub_proc(osal_device_handle_t const handle,
                                                              e_pma_power_policy_t pma_policy);
/*******************************************************************************************************************//**
* @brief            PM Waite State  Cechk.
* @param[in]        handle      Device handle
* @param[in]        time_period      Time Period
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_wait_for_state_pre(osal_device_handle_t const handle,
                                                                osal_milli_sec_t time_period);
/*******************************************************************************************************************//**
* @brief            PM Waite State  Proccess.
* @param[in]        p_timespec         Time Spec
* @param[in]        p_time_period      Time Period
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_wait_for_state_proc_1(struct timespec *p_timespec,
                                                                   osal_milli_sec_t *p_time_period);

/*******************************************************************************************************************//**
* @brief            PM Waite State  Proccess.
* @param[in]        handle           Device handle
* @param[in]        time_period      Time Period
* @param[in]        time_stamp       Time Stamp
* @param[in]        wait_hwa_state   Wait HW State
* @param[in]        wait_reset_req   Wait Reset Req
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_wait_for_state_proc_2(osal_device_handle_t handle,
                                                                   osal_milli_sec_t  time_period,
                                                                   struct timespec   time_stamp,
                                                                   st_pma_hwa_state_t wait_hwa_state,
                                                                   e_pma_reset_req_t  wait_reset_req);

/*******************************************************************************************************************//**
* @brief            PM Initialize Proccess.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_initialize_proc_1(void);

/*******************************************************************************************************************//**
* @brief            PM Initialize Proccess.
* @param[in]        handle           Device handle
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_initialize_proc_2(st_osal_device_control_t const *device_control);

/*******************************************************************************************************************//**
* @brief            PM Initialize Proccess.
* @param[in]        handle           Device handle
* @param[in]        pm_policy        PM Policy
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_initialize_proc_3(st_osal_device_control_t const *device_control,
                                                               e_pma_power_policy_t     pma_policy);

/*******************************************************************************************************************//**
* @brief            PM Initialize Proccess.
* @param[in]        handle           Device handle
* @param[in]        pm_policy        PM Policy
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_DEV
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_initialize_proc_4(st_osal_device_control_t const *device_control,
                                                               e_pma_power_policy_t     pma_policy);

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_PmGetLowestIntermediatePowerState()
* XOS3_OSAL_EMCOS_CD_CD_POW_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_005]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetLowestIntermediatePowerState(osal_device_handle_t handle, e_osal_pm_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    e_pma_return_t      pma_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    e_pma_power_state_t lowest_power;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_pm_get_lowest_intermidiate_power_state_pre(handle, p_state);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
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

            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmGetLowestIntermediatePowerState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmGetPolicy()
* XOS3_OSAL_EMCOS_CD_CD_POW_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_006]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t* const p_policy)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret;
    e_pma_return_t       pma_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                  os_ret;
    e_pma_power_policy_t pma_policy;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_pm_get_policy_pre(handle, p_policy);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            pma_ret = R_PMA_GetPowerPolicy(handle->pma_handle, handle->dev_info->pma_id, &pma_policy);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                osal_ret  = OSAL_RETURN_FAIL;
                *p_policy = OSAL_PM_POLICY_INVALID;
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

            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
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
    }/* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmGetPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmGetRequiredState()
* XOS3_OSAL_EMCOS_CD_CD_POW_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_007]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret;
    e_pma_return_t     pma_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                os_ret;
    st_pma_hwa_state_t pma_status;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_pm_get_required_state_pre(handle, p_state);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            pma_ret = R_PMA_GetHwaState(handle->pma_handle, handle->dev_info->pma_id, &pma_status);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
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

            }    /* end of if(R_PMA_RESULT_OK != pma_ret). */

            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }    /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmGetRequiredState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmGetResetState()
* XOS3_OSAL_EMCOS_CD_CD_POW_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_008]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t   osal_ret;
    e_pma_return_t    pma_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int               os_ret;
    e_pma_reset_req_t pma_reset_state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_pm_get_reset_state_pre(handle, p_state);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
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

            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmGetResetState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmGetState()
* XOS3_OSAL_EMCOS_CD_CD_POW_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_009]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmGetState(osal_device_handle_t handle, e_osal_pm_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t   osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int               os_ret;
    e_osal_pm_state_t pm_state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_pm_get_lowest_intermidiate_power_state_pre(handle, p_state);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = osal_internal_pm_get_state(handle->pma_handle, handle->dev_info->pma_id, &pm_state);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                *p_state = OSAL_PM_STATE_INVALID;
            }
            else
            {
                 *p_state = pm_state;
            }

            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmGetState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmSetPolicy()
* XOS3_OSAL_EMCOS_CD_CD_POW_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_010]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmSetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t policy, bool applyImmediate)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_pm_set_policy_pre(handle, policy);
    
    if (OSAL_RETURN_OK == osal_ret)
    {
         osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = osal_internal_pm_set_policy_proc(handle, policy, applyImmediate);
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmSetPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmSetPostClock()
* XOS3_OSAL_EMCOS_CD_CD_POW_007
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_011]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmSetPostClock(osal_device_handle_t device, bool clock_enable, uint32_t clock_divider)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 2981 1 # Since this variable is used in the judgment, it is necessary to initialize. Do not change. */
    e_osal_return_t osal_ret;
    e_pma_return_t  pma_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    e_pma_clk_req_t enable;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_pmsetpostclock_pre(device, clock_divider);

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = R_OSAL_Internal_TimedlockMutex(&device->inner_mtx, device->inner_timeout);
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
            /* PRQA S 1340 1 # The argument type is cast to match the uint32_t const *, so no problem. */
            pma_ret = R_PMA_SetPostClock(device->pma_handle, enable, (uint32_t)(device->dev_info->postckcr), (uint32_t const*)&clock_divider);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&device->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }    /* end of if(OSAL_RETURN_OK != osal_ret). */
    }
    else
    {
        /* Do nothing */
    }    /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmSetPostClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmSetRequiredState()
* XOS3_OSAL_EMCOS_CD_CD_POW_008
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_012]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmSetRequiredState(osal_device_handle_t handle,
                                          e_osal_pm_required_state_t state,
                                          bool applyImmediate)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                os_ret;
    e_pma_return_t     pma_ret;
    st_pma_hwa_state_t pma_hwa_state;
    e_pma_reset_req_t  pma_reset_req;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_pm_set_required_state_pre(handle, state);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
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

            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (true == applyImmediate)
                {
                    pma_ret = R_PMA_WaitForState(handle->pma_handle, handle->dev_info->pma_id, &pma_hwa_state, pma_reset_req,
                                                 OSAL_POWER_MS_SLEEP_TIME, OSAL_POWER_MS_SLEEP_RETRY, 0);
                }
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
                default:
                    osal_ret = OSAL_RETURN_FAIL;
                    break;
            }


            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmSetRequiredState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmSetResetState()
* XOS3_OSAL_EMCOS_CD_CD_POW_009
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_013]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmSetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret;
    e_pma_return_t       pma_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                  os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_pm_set_reset_state_pre(handle, state);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
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
                    default :
                        osal_ret = OSAL_RETURN_FAIL;
                        break;
                }
            }   /* end of if(OSAL_PM_RESET_STATE_APPLIED == state). */

            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmSetResetState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_PmWaitForState()
* XOS3_OSAL_EMCOS_CD_CD_POW_010
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_014]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmWaitForState(osal_device_handle_t handle, e_osal_pm_state_t state, osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                os_ret;
    /* PRQA S 2983 1 # This variable is used and needs to be initialized. No correction required. */
    struct timespec    time_stamp;
    st_pma_hwa_state_t wait_hwa_state;
    e_pma_reset_req_t  wait_reset_req;
    osal_milli_sec_t   time_period_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 2983 1 # Do not fix it because it is temporary to avoid passing the argument directly by the pointer.*/
    time_period_tmp = time_period;
    osal_ret = osal_internal_pm_wait_for_state_pre(handle, time_period_tmp);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_translate_state(state, &wait_hwa_state, &wait_reset_req);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_pm_wait_for_state_proc_1(&time_stamp, &time_period_tmp);
    }
    else
    {
        /* Do nothing */
    }
    
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        os_ret = pthread_mutex_timedlock(&handle->inner_mtx, &time_stamp);
        if (ETIMEDOUT == os_ret)
        {
            osal_ret = OSAL_RETURN_TIME;
        }
        else if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_pm_wait_for_state_proc_2(handle, time_period_tmp, time_stamp, wait_hwa_state, wait_reset_req);

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_PmWaitForState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_pm_initialize()
* XOS3_OSAL_EMCOS_CD_CD_POW_011
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_003]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t osal_pm_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret = OSAL_RETURN_OK;
    e_pma_return_t           pma_ret;
    st_pma_config_t          pma_config;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_pm_is_initialized = true;

        R_OSAL_DCFG_GetPMAConfiguration(&pma_config);

        pma_ret = R_PMA_Init(&gs_osal_pma_handle, &pma_config);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* Do nothing */
        }
        else if (R_PMA_ERR_NO_VACANT_HANDLE == pma_ret)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_pm_initialize_proc_1();
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)osal_pm_deinitialize();
        }
        else
        {
            /* Do nothing */
        }
    }    /* end of if(false == gs_osal_pm_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_pm_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_pm_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_POW_012
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_001]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t osal_pm_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    e_pma_return_t           pma_ret;
    st_osal_device_control_t *device_control   = NULL;
    size_t                   dev_count         = 0;
    size_t                   dev_total_num;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_pm_is_initialized = false;

    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();
    while ((dev_total_num > dev_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)dev_count, &device_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == device_control) || (NULL == device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (NULL != device_control->pma_handle)
            {
                (void)R_PMA_UnlockHwa(device_control->pma_handle, device_control->dev_info->pma_id);
            }
            else
            {
                /* Do nothing */
            }
            device_control->pma_handle = NULL;
        }
        dev_count++;
    }

    if ((OSAL_RETURN_OK == osal_ret) && (NULL != gs_osal_pma_handle))
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
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_pm_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_pm_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_POW_013
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_002]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
e_osal_return_t osal_pm_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_pm_is_initialized = false;

        osal_ret = osal_internal_pm_deinitialize_pre_proc();

    }

    if (OSAL_RETURN_BUSY == osal_ret)
    {
        gs_osal_pm_is_initialized = true;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_pm_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_pm_set_is_init()
* XOS3_OSAL_EMCOS_CD_CD_POW_014
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_004]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_POW_015]
***********************************************************************************************************************/
void osal_pm_set_is_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_pm_is_initialized = true;

    return;
}
/***********************************************************************************************************************
* End of function osal_pm_set_is_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_translate_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_translate_state(e_osal_pm_state_t osal_pm_state,
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
    }    /* end of switch(osal_pm_state). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_translate_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_get_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_get_state(st_pma_handle_t pma_handle,
                                                       e_pma_hwa_id_t pma_hwaId,
                                                       e_osal_pm_state_t * const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret  = OSAL_RETURN_OK;
    e_pma_return_t      pma_ret;
    st_pma_hwa_state_t  pma_status;
    e_pma_reset_req_t   pma_reset_status;

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
                if (R_PMA_RELEASED_RESET == pma_reset_status)
                {
                    *p_state = OSAL_PM_STATE_READY;
                }
                else
                {
                    *p_state = OSAL_PM_STATE_RESET;
                }
            }
        }    /* end of if(R_PMA_HWA_RELEASED == pma_status.enReqState). */
    }    /* end of if(R_PMA_RESULT_OK != pma_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_get_lowest_intermidiate_power_state_pre()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_get_lowest_intermidiate_power_state_pre(osal_device_handle_t const handle,
                                                                                     e_osal_pm_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
          True if the correct handle was passed. False detection of the tool. */
        if ((false == handle->active) || (OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (NULL == handle->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
          True if the correct handle was passed. False detection of the tool. */
        else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
        {
            osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
        else
        {
            /* Do nothing */
        }
    }    /* end of if(OSAL_RETURN_OK == osal_ret). */
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
       True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {    
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == handle->pma_handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_state)
        {
            osal_ret = OSAL_RETURN_PAR;
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
* End of function osal_internal_pm_get_lowest_intermidiate_power_state_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_get_policy_pre()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_get_policy_pre(osal_device_handle_t const handle,
                                                            e_osal_pm_policy_t* const p_policy)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
            /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
          True if the correct handle was passed. False detection of the tool. */
        if ((false == handle->active) || (OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (NULL == handle->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
          True if the correct handle was passed. False detection of the tool. */
        else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
       True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == handle->pma_handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_policy)
        {
            osal_ret = OSAL_RETURN_PAR;
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
* End of function osal_internal_pm_get_policy_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_pm_get_required_state_pre()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_pm_get_required_state_pre(osal_device_handle_t const handle,
                                                           e_osal_pm_required_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((false == handle->active) || (OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (NULL == handle->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
       True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == handle->pma_handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_state)
        {
            osal_ret = OSAL_RETURN_PAR;
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
* End of function osal_pm_get_required_state_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_pm_get_reset_state_pre()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_pm_get_reset_state_pre(osal_device_handle_t const handle,
                                                        e_osal_pm_reset_state_t* const p_state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((false == handle->active) || (OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (NULL == handle->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == handle->pma_handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (NULL == p_state)
        {
            osal_ret = OSAL_RETURN_PAR;
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
* End of function osal_pm_get_reset_state_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pmsetpostclock_pre()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pmsetpostclock_pre(osal_device_handle_t const device,
                                                             const uint32_t clock_divider)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             strncmp_fba;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             strncmp_fbc;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == device)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((false == device->active) || (OSAL_DEVICE_HANDLE_ID != device->handle_id) || (NULL == device->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (NULL == device->pma_handle)
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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
       True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (64U <= clock_divider)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else if (OSAL_PM_POSTCKCR_INVALID == device->dev_info->postckcr)
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        strncmp_fbc = strncmp("fbc", device->dev_info->type, 3);
        strncmp_fba = strncmp("fba", device->dev_info->type, 3);
        if ((0!= strncmp_fbc) && (0 != strncmp_fba))
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

    return osal_ret;

}
/***********************************************************************************************************************
* End of function osal_internal_pmsetpostclock_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_set_required_state_pre()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_set_required_state_pre(osal_device_handle_t const handle,
                                                        e_osal_pm_required_state_t const state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((false == handle->active) ||(OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (NULL == handle->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == handle->pma_handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if ((OSAL_PM_REQUIRED_STATE_REQUIRED != state) &&
                 (OSAL_PM_REQUIRED_STATE_RELEASED != state))
        {
            osal_ret = OSAL_RETURN_PAR;
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
* End of function osal_internal_pm_set_required_state_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_set_reset_state_pre()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_set_reset_state_pre(osal_device_handle_t const handle,
                                                     e_osal_pm_reset_state_t const state)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((false == handle->active) ||(OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (NULL == handle->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == handle->pma_handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if ((OSAL_PM_RESET_STATE_APPLIED != state) &&
                 (OSAL_PM_RESET_STATE_RELEASED != state))
        {
            osal_ret = OSAL_RETURN_PAR;
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
* End of function osal_internal_pm_set_reset_state_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_deinitialize_pre_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_deinitialize_pre_proc(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    size_t                   dev_total_num;
    size_t                   dev_count       = 0;
    st_osal_device_control_t *device_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();
    while ((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)dev_count, &device_control);
        if ((OSAL_RETURN_OK != osal_ret) ||
            (NULL == device_control) ||
            (NULL == device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_Internal_TimedlockMutex(&device_control->inner_mtx, device_control->inner_timeout);
            if (OSAL_RETURN_OK == osal_ret)
            {
                if (true == device_control->pm_wait_state)
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }
                else
                {
                    /* Do nothing */
                }

                os_ret = pthread_mutex_unlock(&device_control->inner_mtx);
                if (0 != os_ret)
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
            }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        dev_count++;
    }   /* end of while((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret)) */

    return osal_ret;

}
/***********************************************************************************************************************
* End of function osal_internal_pm_deinitialize_pre_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_set_policy_pre()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_set_policy_pre(osal_device_handle_t const handle,
                                                            e_osal_pm_policy_t const policy)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((false == handle->active) || (OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (NULL == handle->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == handle->pma_handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if ((OSAL_PM_POLICY_INVALID >= policy) || (OSAL_PM_POLICY_HP < policy))
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;

}
/***********************************************************************************************************************
* End of function osal_internal_pm_set_policy_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_set_policy_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_set_policy_proc(osal_device_handle_t handle,
                                                             e_osal_pm_policy_t policy,
                                                             bool applyImmediate)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_pma_return_t  pma_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int              os_ret;
    e_pma_power_policy_t pma_policy;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_PM_POLICY_PG == policy)
    {
        pma_policy = R_PMA_PG_MODE;
    }
    else if (OSAL_PM_POLICY_CG == policy)
    {
        pma_policy = R_PMA_CG_MODE;
    }
    else /* else if (OSAL_PM_POLICY_HP == policy),  already checked in osal_internal_pm_set_policy_pre() */
    {
        pma_policy = R_PMA_HP_MODE;
    }

    pma_ret = R_PMA_SetPowerPolicy(handle->pma_handle, handle->dev_info->pma_id, pma_policy);
    if (R_PMA_RESULT_OK != pma_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if ((OSAL_RETURN_OK == osal_ret) && (true == applyImmediate))
    {
        osal_ret = osal_internal_pm_set_policy_sub_proc(handle, pma_policy);
    }

    os_ret = pthread_mutex_unlock(&handle->inner_mtx);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_set_policy_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_set_policy_sub_proc()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_set_policy_sub_proc(osal_device_handle_t const handle,
                                                             e_pma_power_policy_t pma_policy)

{    
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t              wait_count;
    e_pma_power_policy_t check_pma_policy = R_PMA_PG_MODE;
    e_pma_return_t  pma_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    for (wait_count = 0; (OSAL_RETURN_OK == osal_ret) && (OSAL_POWER_MS_SLEEP_RETRY > wait_count); wait_count++)
    {
        pma_ret = R_PMA_GetPowerPolicy(handle->pma_handle, handle->dev_info->pma_id, &check_pma_policy);
        if (pma_ret != R_PMA_RESULT_OK)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else if (pma_policy == check_pma_policy)
        {
            break;
        }
        else
        {
            (void)R_OSAL_Internal_MsSleep(OSAL_POWER_MS_SLEEP_TIME);
        }
    }

    if (OSAL_POWER_MS_SLEEP_RETRY <= wait_count)
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
* End of function osal_internal_pm_set_policy_sub_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_wait_for_state_pre()
***********************************************************************************************************************/
/* PRQA S 3673 3 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_wait_for_state_pre(osal_device_handle_t const handle,
                                                                osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_pm_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((false == handle->active) || (OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (NULL == handle->dev_info))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (R_PMA_HWA_ID_NONE == handle->dev_info->pma_id)
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
    }   /* end of if(OSAL_RETURN_OK = osal_ret). */

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == handle->pma_handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (0 > time_period)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK = osal_ret). */


    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_wait_for_state_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_wait_for_state_proc_1()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_wait_for_state_proc_1(struct timespec *p_timespec,
                                                                   osal_milli_sec_t *p_time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    osal_ret = R_OSAL_Internal_GetTimestamp(p_timespec);
    if (OSAL_RETURN_OK != osal_ret)
    {
       osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = R_OSAL_Internal_TimespecAdd(p_timespec, p_time_period);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }    /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_wait_for_state_proc_1()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_wait_for_state_proc_2()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_wait_for_state_proc_2(osal_device_handle_t handle,
                                                                   osal_milli_sec_t  time_period,
                                                                   struct timespec   time_stamp,
                                                                   st_pma_hwa_state_t wait_hwa_state,
                                                                   e_pma_reset_req_t  wait_reset_req)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t    osal_ret = OSAL_RETURN_OK;
    e_pma_return_t     pma_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                os_ret;
    st_pma_hwa_state_t wait_hwa_state_tmp;
    e_pma_reset_req_t  wait_reset_req_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    wait_hwa_state_tmp = wait_hwa_state;
    wait_reset_req_tmp = wait_reset_req;
    /* PRQA S 2880,2982 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    handle->pm_wait_state = true;

    os_ret = pthread_mutex_unlock(&handle->inner_mtx);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (0 == time_period)
        {
            pma_ret = R_PMA_WaitForState(handle->pma_handle, handle->dev_info->pma_id, &wait_hwa_state_tmp, wait_reset_req_tmp,
                                         0, 0, 0);
        }
        else
        {
            pma_ret = R_PMA_WaitForState(handle->pma_handle, handle->dev_info->pma_id, &wait_hwa_state_tmp, wait_reset_req_tmp,
                                         OSAL_POWER_MS_SLEEP_TIME, time_period,  0);
        }
        /* PRQA S 1340 1 # The argument type is cast to match the uint32_t const *, so no problem. */
        os_ret = pthread_mutex_timedlock(&handle->inner_mtx, &time_stamp);
        if (ETIMEDOUT == os_ret)
        {
            osal_ret = OSAL_RETURN_TIME;
        }
        else if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }/* end of if(ETIMEDOUT == os_ret). */

        if (OSAL_RETURN_OK == osal_ret)
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
        }
    }/* end of if(0 != os_ret). */

    handle->pm_wait_state = false;
    
    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_wait_for_state_proc_2()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_initialize_proc_1()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_pm_initialize_proc_1(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    size_t                   dev_count         = 0;
    size_t                   dev_total_num;
    st_osal_device_control_t *device_control   = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();
    while ((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)dev_count, &device_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == device_control) || (NULL == device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            device_control->pma_handle = gs_osal_pma_handle;
            if (R_PMA_HWA_ID_NONE == device_control->dev_info->pma_id)
            {
                /* the device is not supported by PMA, do nothing */
            }
            else
            {
                osal_ret = osal_internal_pm_initialize_proc_2(device_control);
            }    /* end of if(R_PMA_HWA_ID_NONE == device_control->dev_info->pma_id). */
        }    /* end of if((OSAL_RETURN_OK != osal_ret) || (NULL == device_control) || (NULL == device_control->dev_info)). */
        dev_count++;
    }    /* end of while((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_initialize_proc_1()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_initialize_proc_2()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_initialize_proc_2(st_osal_device_control_t const *device_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    e_pma_return_t           pma_ret;
    e_pma_power_policy_t     pma_policy;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    switch (device_control->dev_info->initial_policy)
    {
        case OSAL_PM_POLICY_PG:
            pma_policy = R_PMA_PG_MODE;
            break;
        case OSAL_PM_POLICY_CG:
            pma_policy = R_PMA_CG_MODE;
            break;
        case OSAL_PM_POLICY_HP:
            pma_policy = R_PMA_HP_MODE;
            break;
        default:
            osal_ret = OSAL_RETURN_CONF;
            break;
    }
    if (OSAL_RETURN_OK == osal_ret)
    {
        pma_ret = R_PMA_GetLockHwa(device_control->pma_handle, device_control->dev_info->pma_id);
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
            osal_ret = osal_internal_pm_initialize_proc_3(device_control, pma_policy);
        }
    }
    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_initialize_proc_2()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_initialize_proc_3()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_initialize_proc_3(st_osal_device_control_t const *device_control,
                                                               e_pma_power_policy_t     pma_policy)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    e_pma_return_t           pma_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    pma_ret = R_PMA_SetPowerPolicy(device_control->pma_handle, device_control->dev_info->pma_id, pma_policy);
    if (R_PMA_ERR_INVALID_ARGUMENT == pma_ret)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else if (R_PMA_ERR_DEVICE == pma_ret)
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
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_pm_initialize_proc_4(device_control, pma_policy);
    }
    else
    {
        /* Do nothing */
    }
    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_initialize_proc_3()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_pm_initialize_proc_4()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_pm_initialize_proc_4(st_osal_device_control_t const *device_control,
                                                               e_pma_power_policy_t     pma_policy)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    int32_t                  wait_count;
    e_pma_power_policy_t     check_pma_policy;
    e_pma_return_t           pma_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (wait_count = 0; (OSAL_RETURN_OK == osal_ret) && (OSAL_POWER_MS_SLEEP_RETRY > wait_count); wait_count++)
    {
        pma_ret = R_PMA_GetPowerPolicy(device_control->pma_handle, device_control->dev_info->pma_id, &check_pma_policy);
        if (R_PMA_RESULT_OK != pma_ret)
        {
            osal_ret = OSAL_RETURN_DEV;
        }
        else if (pma_policy <= check_pma_policy)
        {
            break;
        }
        else
        {
            osal_ret = R_OSAL_Internal_MsSleep(1);
        }
    }
    if (OSAL_POWER_MS_SLEEP_RETRY <= wait_count)
    {
        osal_ret = OSAL_RETURN_DEV;
    }
    else
    {
        /* Do nothing */
    }
    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_pm_initialize_proc_4()
***********************************************************************************************************************/
