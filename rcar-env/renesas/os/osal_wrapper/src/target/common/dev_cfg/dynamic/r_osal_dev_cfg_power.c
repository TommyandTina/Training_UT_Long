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
* File Name :    r_osal_dev_cfg_power.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configuration Functions for Power Manager
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <string.h>
#include "target/common/dev_cfg/dynamic/r_osal_dev_cfg.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Definition
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @def     OSAL_PM_WAIT_TIME
 * @brief   Time out for waiting for state.
***********************************************************************************************************************/
#define OSAL_PM_WAIT_TIME   (333000L) /* 333us */

/***********************************************************************************************************************
 * @def     OSAL_PM_APPLY_IMMEDIATE_TIMEOUT
 * @brief   Time out for waiting for change policy.
***********************************************************************************************************************/
#define OSAL_PM_APPLY_IMMEDIATE_TIMEOUT (1000) /* msec */

/***********************************************************************************************************************
 * Global variable
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @var     gs_power_init_state
 * @brief   Status of power manager.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_power_init_state = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Get the HW status of the power and clock.
 * @param[in]        handle          A handle of device information.
 * @param[in,out]    p_pwr_status    A pointer to the Power status.
 * @param[in,out]    p_clk_status    A pointer to the Clock status.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
/* PRQA S 5209 1 # Used as the parameter of ioctl. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_get_power_clock_state(osal_device_handle_t handle, int* p_pwr_status, int* p_clk_status);

/***********************************************************************************************************************
 * @brief            Set the HW status of the power and clock according to the policy.
 * @param[in]        handle    A handle of device information.
 * @param[in]        policy    OSAL pm policy.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_power_clock_state(osal_device_handle_t handle, e_osal_pm_policy_t policy);

/***********************************************************************************************************************
 * @brief            Wait for changing state when applyImmediate is true.
 * @param[in]        handle           A handle of device information.
 * @param[in]        state            OSAL PM sate
 * @param[in]        p_abs_timeout    Wait on a condition variable with timeout until time stamp.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_wait_for_required_state(osal_device_handle_t handle, e_osal_pm_state_t state, st_osal_time_t const* p_abs_timeout);

/***********************************************************************************************************************
 * @brief            Get the HW status considering required status.
 * @param[in]        handle           A handle of device information.
 * @param[in,out]    p_state          A pointer to the OSAL PM sate
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_get_state(osal_device_handle_t handle, e_osal_pm_state_t* const p_state);

/***********************************************************************************************************************
 * @brief            Get the HW status of the reset.
 * @param[in]        handle           A handle of device information.
 * @param[in,out]    p_reset_state    A pointer to the OSAL pm reset state
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_get_reset_state(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_reset_state);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_DCFG_PmSetRequiredState() when set the required.
 * @param[in]        handle            A handle of device information.
 * @param[in]        applyImmediate    Immediate Flag
 * @param[in]        p_abs_timeout     The value for timeout by [sec] order.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_required(osal_device_handle_t handle, bool applyImmediate, st_osal_time_t const* p_abs_timeout);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_DCFG_PmSetRequiredState() when set the release.
 * @param[in]        handle            A handle of device information.
 * @param[in]        applyImmediate    Immediate Flag
 * @param[in]        p_abs_timeout     Wait on a condition variable with timeout until time stamp.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_released(osal_device_handle_t handle, bool applyImmediate, st_osal_time_t const* p_abs_timeout);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_DCFG_PmWaitForState().
 * @param[in]        handle           A handle of device information.
 * @param[in]        state            OSAL PM sate
 * @param[in]        p_abs_timeout    Wait on a condition variable with timeout until time stamp.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_wait_for_state(osal_device_handle_t handle, e_osal_pm_state_t state, st_osal_time_t const* p_abs_timeout);

/***********************************************************************************************************************
 * @brief            Set the status according to the policy.
 * @param[in,out]    handle            A handle of device information.
 * @param[in]        policy            PMA Policy
 * @param[in]        applyImmediate    Immediate Flag
 * @param[in]        p_abs_timeout     Wait on a condition variable with timeout until time stamp.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_policy(osal_device_handle_t handle, e_osal_pm_policy_t policy, bool applyImmediate, st_osal_time_t const* p_abs_timeout);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_DCFG_PmSetResetState().
 * @param[in]        handle         A handle of device information.
 * @param[in]        reset_state    OSAL pm reset state
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_reset_state(osal_device_handle_t handle, e_osal_pm_reset_state_t reset_state);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex for time period.
 * @param[in]        mutex    A handle of mutex information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_mutex_lock_for_time_period(osal_os_mutex_t mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex until timestamp.
 * @param[in]        mutex            A handle of mutex information.
 * @param[in]        p_abs_timeout    Wait on a condition variable with timeout until time stamp.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
 * @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout);

/***********************************************************************************************************************
 * @brief            Sleep for nsec unit.
 * @param[in]        timeout    Wait on a condition variable with timeout until time stamp.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_ns_sleep(osal_nano_sec_t timeout);

/***********************************************************************************************************************
 * @brief            Convert int status to #e_osal_pm_state_t.
 * @param[in]        pwr_status    Power status.
 * @param[in]        clk_status    Clock status.
 * @param[in,out]    p_state       A pointer to the OSAL PM sate
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
/* PRQA S 5209 1 # Used as the parameter of ioctl. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_convert_state(int pwr_status, int clk_status, e_osal_pm_state_t* const p_state);

/***********************************************************************************************************************
 * @brief            Convert #e_osal_pm_policy_t to #e_osal_pm_state_t.
 * @param[in]        policy    OSAL pm policy.
 * @param[in,out]    p_state   A pointer to the OSAL PM sate
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_convert_policy(e_osal_pm_policy_t policy, e_osal_pm_state_t* const p_state);

/***********************************************************************************************************************
 * @brief            Get the policy considering power shared.
 * @param[in]        handle    A handle of device information.
 * @param[in]        policy    OSAL pm policy.
 * @return           #e_osal_pm_policy_t
 * @retval           policy
 * @retval           OSAL_PM_POLICY_CG
***********************************************************************************************************************/
OSAL_STATIC e_osal_pm_policy_t r_osal_dcfg_pm_get_next_policy(const osal_device_handle_t handle, e_osal_pm_policy_t policy);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_PmGetInitStatus(void)
{
    return gs_power_init_state;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_DCFG_PmSetInitStatus(e_osal_init_status_t status)
{
    gs_power_init_state = status;
    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmControlInit(void)
{
    /* Do nothing on Linux */
    return OSAL_RETURN_OK;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmControlDeinit(void)
{
    /* Do nothing on Linux */
    return OSAL_RETURN_OK;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmCheckBusy(void)
{
    /* Do nothing on Linux */
    return OSAL_RETURN_OK;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetRequiredState()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_DCFG_PmGetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t* const p_state)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_pm_mutex_lock_for_time_period(handle->pm_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_IO_HANDLE_ID != handle->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((false == handle->pm_handle->is_power_support) &&
                (false == handle->pm_handle->is_clock_support))
            {
                osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_state = handle->pm_handle->pm_required_state;

            if ((OSAL_PM_REQUIRED_STATE_REQUIRED != *p_state) &&
                (OSAL_PM_REQUIRED_STATE_RELEASED != *p_state))
            {
                *p_state = OSAL_PM_REQUIRED_STATE_INVALID;
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetRequiredState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetRequiredState()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t state, bool applyImmediate)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    osal_milli_sec_t    time_period;
    st_osal_time_t      abs_timeout;

    if (true == applyImmediate)
    {
        time_period = OSAL_PM_APPLY_IMMEDIATE_TIMEOUT;
    }
    else
    {
        time_period = OSAL_INNER_TIMEOUT;
    }

    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &abs_timeout);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_pm_mutex_lock_until_time_stamp(handle->pm_mtx, &abs_timeout);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (OSAL_IO_HANDLE_ID != handle->handle_id)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                if ((false == handle->pm_handle->is_power_support) &&
                    (false == handle->pm_handle->is_clock_support))
                {
                    osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                }
            }

            /* Access to HW in the only case of input the state is different to current state */
            if ((OSAL_RETURN_OK == osal_ret) && (handle->pm_handle->pm_required_state != state))
            {
                if (OSAL_PM_REQUIRED_STATE_REQUIRED == state)
                {
                    osal_ret = r_osal_dcfg_pm_set_required(handle, applyImmediate, &abs_timeout);
                }
                else
                {
                    osal_ret = r_osal_dcfg_pm_set_released(handle, applyImmediate, &abs_timeout);
                }

                if (OSAL_RETURN_OK == osal_ret)
                {
                    handle->pm_handle->pm_required_state = state;
                }
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_dcfg_pm_mutex_lock_until_time_stamp */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetRequiredState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmWaitForState()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmWaitForState(osal_device_handle_t handle, e_osal_pm_state_t state, osal_milli_sec_t time_period)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    e_osal_return_t     wait_ret = OSAL_RETURN_OK;
    st_osal_time_t      abs_timeout;

    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &abs_timeout);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_pm_mutex_lock_until_time_stamp(handle->pm_mtx, &abs_timeout);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (OSAL_IO_HANDLE_ID != handle->handle_id)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                if ((false == handle->pm_handle->is_power_support) &&
                    (false == handle->pm_handle->is_clock_support) &&
                    (false == handle->pm_handle->is_reset_support))
                {
                    osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                }
            }
        }

        /* Can not close the device until this function finish */
        /* R_OSAL_DCFG_IoDeviceClose() returns BUSY */
        if (OSAL_RETURN_OK == osal_ret)
        {
            handle->pm_handle->pm_wait_state = true;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            handle->pm_handle->pm_wait_state = false;
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        wait_ret = r_osal_dcfg_pm_wait_for_state(handle, state, &abs_timeout);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_pm_mutex_lock_until_time_stamp(handle->pm_mtx, &abs_timeout);

        handle->pm_handle->pm_wait_state = false;

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if ((OSAL_RETURN_OK != wait_ret) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = wait_ret;
    }

    return osal_ret;
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmWaitForState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetState()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmGetState(osal_device_handle_t handle, e_osal_pm_state_t* const p_state)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    e_osal_pm_state_t   state = OSAL_PM_STATE_PG;

    osal_ret = r_osal_dcfg_pm_mutex_lock_for_time_period(handle->pm_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_IO_HANDLE_ID != handle->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((false == handle->pm_handle->is_power_support) &&
                (false == handle->pm_handle->is_clock_support) &&
                (false == handle->pm_handle->is_reset_support))
            {
                osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_pm_get_state(handle, &state);
            if (OSAL_RETURN_OK == osal_ret)
            {
                *p_state = state;
            }
            else
            {
                *p_state = OSAL_PM_STATE_INVALID;
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetLowestIntermediatePowerState()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_DCFG_PmGetLowestIntermediatePowerState(osal_device_handle_t handle, e_osal_pm_state_t* const p_state)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_pm_mutex_lock_for_time_period(handle->pm_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_IO_HANDLE_ID != handle->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((false == handle->pm_handle->is_power_support) &&
                (false == handle->pm_handle->is_clock_support))
            {
                osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_state = handle->pm_handle->pm_lower_state;

            if ((OSAL_PM_STATE_PG != *p_state) &&
                (OSAL_PM_STATE_CG != *p_state) &&
                (OSAL_PM_STATE_ENABLED != *p_state))
            {
                *p_state = OSAL_PM_STATE_INVALID;
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetLowestIntermediatePowerState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetPolicy()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_DCFG_PmGetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t* const p_policy)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_pm_mutex_lock_for_time_period(handle->pm_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_IO_HANDLE_ID != handle->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((false == handle->pm_handle->is_power_support) &&
                (false == handle->pm_handle->is_clock_support))
            {
                osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_policy = handle->pm_handle->pm_policy;

            if ((OSAL_PM_POLICY_PG != *p_policy) &&
                (OSAL_PM_POLICY_CG != *p_policy) &&
                (OSAL_PM_POLICY_HP != *p_policy))
            {
                *p_policy = OSAL_PM_POLICY_INVALID;
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetPolicy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetPolicy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t policy, bool applyImmediate)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    osal_milli_sec_t    time_period;
    st_osal_time_t      abs_timeout;

    if (true == applyImmediate)
    {
        time_period = OSAL_PM_APPLY_IMMEDIATE_TIMEOUT;
    }
    else
    {
        time_period = OSAL_INNER_TIMEOUT;
    }

    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(time_period, &abs_timeout);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_pm_mutex_lock_until_time_stamp(handle->pm_mtx, &abs_timeout);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (OSAL_IO_HANDLE_ID != handle->handle_id)
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                if ((false == handle->pm_handle->is_power_support) &&
                    (false == handle->pm_handle->is_clock_support))
                {
                    osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                }
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_dcfg_pm_set_policy(handle, policy, applyImmediate, &abs_timeout);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_dcfg_pm_mutex_lock_until_time_stamp */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetPolicy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmGetResetState()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmGetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_state)
{
    e_osal_return_t         osal_ret;
    e_osal_return_t         mutex_ret;
    e_osal_pm_reset_state_t reset_state = OSAL_PM_RESET_STATE_INVALID;

    osal_ret = r_osal_dcfg_pm_mutex_lock_for_time_period(handle->pm_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_IO_HANDLE_ID != handle->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (false == handle->pm_handle->is_reset_support)
            {
                osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_state = OSAL_PM_RESET_STATE_INVALID;

            osal_ret = r_osal_dcfg_pm_get_reset_state(handle, &reset_state);
            if (OSAL_RETURN_OK == osal_ret)
            {
                if (OSAL_PM_RESET_STATE_INVALID != reset_state)
                {
                    *p_state = reset_state;
                }
                else
                {
                    osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
                }
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmGetResetState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetResetState()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t state)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_pm_mutex_lock_for_time_period(handle->pm_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_IO_HANDLE_ID != handle->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (false == handle->pm_handle->is_reset_support)
            {
                osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_pm_set_reset_state(handle, state);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetResetState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_PmSetPostClock()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetPostClock(osal_device_handle_t device, bool clock_enable, uint32_t clock_divider)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    /* PRQA S 5209 2 # Return values from linux must be int. Used according to the specifications. */
    int             strcmp_ret1;
    int             strcmp_ret2;

    osal_ret = r_osal_dcfg_pm_mutex_lock_for_time_period(device->pm_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_IO_HANDLE_ID != device->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            strcmp_ret1 = strncmp(device->dev_id, "fba", strlen("fba"));
            strcmp_ret2 = strncmp(device->dev_id, "fbc", strlen("fbc"));
            if ((0 != strcmp_ret1) && (0 != strcmp_ret2))
            {
               osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_PmSetPostClock(device, clock_enable, (uint32_t)(clock_divider - 1U));
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(device->pm_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_PmSetPostClock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_get_power_clock_state()
***********************************************************************************************************************/
/* PRQA S 5209 1 # Used as the parameter of ioctl. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_get_power_clock_state(osal_device_handle_t handle, int* p_pwr_status, int* p_clk_status)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t power_ret;
    e_osal_return_t clock_ret;

    power_ret = R_OSAL_OS_PmGetPowerState(handle, p_pwr_status);
    if (OSAL_RETURN_OK == power_ret)
    {
        /* Do nothing */
    }
    else if (OSAL_RETURN_UNSUPPORTED_OPERATION == power_ret)
    {
        /* Always ON */
        *p_pwr_status = 1;
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        clock_ret = R_OSAL_OS_PmGetClockState(handle, p_clk_status);
        if (OSAL_RETURN_OK == clock_ret)
        {
            /* Do nothing */
        }
        else if (OSAL_RETURN_UNSUPPORTED_OPERATION == clock_ret)
        {
            /* Equals to power */
            *p_clk_status = *p_pwr_status;
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_get_power_clock_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_017
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_set_power_clock_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_power_clock_state(osal_device_handle_t handle, e_osal_pm_policy_t policy)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t power_ret;
    e_osal_return_t clock_ret;
    int             pwr_status; /* PRQA S 5209 # Used as the parameter of ioctl. */
    int             clk_status; /* PRQA S 5209 # Used as the parameter of ioctl. */
    int             current_pwr_status; /* PRQA S 5209 # Used as the parameter of ioctl. */
    int             current_clk_status; /* PRQA S 5209 # Used as the parameter of ioctl. */

    switch (policy)
    {
        case OSAL_PM_POLICY_PG :
            pwr_status = 0;
            clk_status = 0;
            break;

        case OSAL_PM_POLICY_CG :
            pwr_status = 1;
            clk_status = 0;
            break;

        case OSAL_PM_POLICY_HP :
        default :
            pwr_status = 1;
            clk_status = 1;
            break;
    }

    power_ret = R_OSAL_OS_PmGetPowerState(handle, &current_pwr_status);
    if ((OSAL_RETURN_OK != power_ret) &&
        (OSAL_RETURN_UNSUPPORTED_OPERATION != power_ret))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if ((OSAL_RETURN_OK == osal_ret) &&
        (OSAL_RETURN_OK == power_ret) &&
        (current_pwr_status != pwr_status))
    {
        osal_ret = R_OSAL_OS_PmSetPowerState(handle, pwr_status);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        clock_ret = R_OSAL_OS_PmGetClockState(handle, &current_clk_status);
        if ((OSAL_RETURN_OK != clock_ret) &&
            (OSAL_RETURN_UNSUPPORTED_OPERATION != clock_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if ((OSAL_RETURN_OK == osal_ret) &&
        (OSAL_RETURN_OK == clock_ret) &&
        (current_clk_status != clk_status))
    {
        osal_ret = R_OSAL_OS_PmSetClockState(handle, clk_status);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_set_power_clock_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_018
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_wait_for_required_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_wait_for_required_state(osal_device_handle_t handle, e_osal_pm_state_t state, st_osal_time_t const* p_abs_timeout)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t   current_state = OSAL_PM_STATE_INVALID;
    st_osal_time_t      current_timestamp;
    osal_nano_sec_t     timeout;
    osal_nano_sec_t     current_time;
    osal_nano_sec_t     sleep_time;
    int                 pwr_status = 0; /* PRQA S 5209 # Used as the parameter of ioctl. */
    int                 clk_status = 0; /* PRQA S 5209 # Used as the parameter of ioctl. */

    timeout = (p_abs_timeout->tv_sec * OSAL_SECTONSEC) + p_abs_timeout->tv_nsec;

    while (OSAL_RETURN_OK == osal_ret)
    {
        /* Get the current status from HW */
        osal_ret = r_osal_dcfg_pm_get_power_clock_state(handle, &pwr_status, &clk_status);

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_pm_convert_state(pwr_status, clk_status, &current_state);

            if ((OSAL_RETURN_OK == osal_ret) && (current_state == state))
            {
                break;
            }
        }

        /* Wait for timeout of 1 loop */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(&current_timestamp);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            current_time = (current_timestamp.tv_sec * OSAL_SECTONSEC) + current_timestamp.tv_nsec;
            if (timeout < current_time)
            {
                osal_ret = OSAL_RETURN_TIME;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((current_time + OSAL_PM_WAIT_TIME) < timeout)
            {
                sleep_time = OSAL_PM_WAIT_TIME;
            }
            else
            {
                sleep_time = timeout - current_time;
            }

            osal_ret = r_osal_dcfg_pm_ns_sleep(sleep_time);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_wait_for_required_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_019
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_get_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_get_state(osal_device_handle_t handle, e_osal_pm_state_t* const p_state)
{
    e_osal_return_t         osal_ret;
    e_osal_pm_reset_state_t reset_state = OSAL_PM_RESET_STATE_INVALID;
    int                     pwr_status = 0; /* PRQA S 5209 # Used as the parameter of ioctl. */
    int                     clk_status = 0; /* PRQA S 5209 # Used as the parameter of ioctl. */

    /* Get the current status from HW */
    if (OSAL_PM_REQUIRED_STATE_RELEASED == handle->pm_handle->pm_required_state)
    {
        osal_ret = r_osal_dcfg_pm_get_power_clock_state(handle, &pwr_status, &clk_status);

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_pm_convert_state(pwr_status, clk_status, p_state);
        }
    }
    else /* handle->pm_handle->pm_required_state == OSAL_PM_REQUIRED_STATE_REQUIRED */
    {
        osal_ret = r_osal_dcfg_pm_get_reset_state(handle, &reset_state);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (OSAL_PM_RESET_STATE_APPLIED == reset_state)
            {
                *p_state = OSAL_PM_STATE_RESET;
            }
            else
            {
                *p_state = OSAL_PM_STATE_READY;
            }
        }
    } /* end of if (OSAL_PM_REQUIRED_STATE_RELEASED == handle->pm_handle->pm_required_state) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_get_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_020
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_get_reset_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_get_reset_state(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_reset_state)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t reset_ret;
    /* PRQA S 5209 1 # Used as the parameter of ioctl. */
    int             reset_state = 0;

    reset_ret = R_OSAL_OS_PmGetResetState(handle, &reset_state);

    if (OSAL_RETURN_OK == reset_ret)
    {
        if (1 == reset_state)
        {
            *p_reset_state = OSAL_PM_RESET_STATE_APPLIED;
        }
        else if (0 == reset_state)
        {
            *p_reset_state = OSAL_PM_RESET_STATE_RELEASED;
        }
        else
        {
            *p_reset_state = OSAL_PM_RESET_STATE_INVALID;
        }
    }
    else if (OSAL_RETURN_UNSUPPORTED_OPERATION == reset_ret)
    {
        /* Ready */
        *p_reset_state = OSAL_PM_RESET_STATE_RELEASED;
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    } /* end of if (OSAL_RETURN_OK == reset_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_get_reset_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_021
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_set_required()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_required(osal_device_handle_t handle, bool applyImmediate, st_osal_time_t const* p_abs_timeout)
{
    e_osal_return_t osal_ret;

    /* Change the HW status */
    osal_ret = r_osal_dcfg_pm_set_power_clock_state(handle, OSAL_PM_POLICY_HP);

    /* Wait for changing status */
    if ((OSAL_RETURN_OK == osal_ret) &&
        (true == applyImmediate))
    {
        osal_ret = r_osal_dcfg_pm_wait_for_required_state(handle, OSAL_PM_STATE_ENABLED, p_abs_timeout);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_set_required()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_022
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_set_released()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_released(osal_device_handle_t handle, bool applyImmediate, st_osal_time_t const* p_abs_timeout)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    e_osal_pm_policy_t  next_policy;
    e_osal_pm_state_t   state;

    if ((OSAL_PM_POLICY_PG != handle->pm_handle->pm_policy) &&
        (OSAL_PM_POLICY_CG != handle->pm_handle->pm_policy) &&
        (OSAL_PM_POLICY_HP != handle->pm_handle->pm_policy))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    /* Change the HW status */
    if (OSAL_RETURN_OK == osal_ret)
    {
        next_policy = r_osal_dcfg_pm_get_next_policy(handle, handle->pm_handle->pm_policy);

        osal_ret = r_osal_dcfg_pm_set_power_clock_state(handle, next_policy);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_pm_convert_policy(next_policy, &state);
    }

    /* Wait for changing status */
    if ((OSAL_RETURN_OK == osal_ret) &&
        (true == applyImmediate))
    {
        osal_ret = r_osal_dcfg_pm_wait_for_required_state(handle, state, p_abs_timeout);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->pm_handle->pm_lower_state = state;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_set_released()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_023
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_wait_for_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_wait_for_state(osal_device_handle_t handle, e_osal_pm_state_t state, st_osal_time_t const* p_abs_timeout)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t   current_state;
    st_osal_time_t      current_timestamp;
    osal_nano_sec_t     timeout;
    osal_nano_sec_t     current_time;
    osal_nano_sec_t     sleep_time;

    timeout = (p_abs_timeout->tv_sec * OSAL_SECTONSEC) + p_abs_timeout->tv_nsec;

    while (OSAL_RETURN_OK == osal_ret)
    {
        /* Get the current status from HW */
        osal_ret = r_osal_dcfg_pm_get_state(handle, &current_state);

        if ((OSAL_RETURN_OK == osal_ret) && (current_state == state))
        {
            break;
        }

        /* Wait for timeout of 1 loop */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(&current_timestamp);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            current_time = (current_timestamp.tv_sec * OSAL_SECTONSEC) + current_timestamp.tv_nsec;
            if (timeout < current_time)
            {
                osal_ret = OSAL_RETURN_TIME;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((current_time + OSAL_PM_WAIT_TIME) < timeout)
            {
                sleep_time = OSAL_PM_WAIT_TIME;
            }
            else
            {
                sleep_time = timeout - current_time;
            }

            osal_ret = r_osal_dcfg_pm_ns_sleep(sleep_time);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_wait_for_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_024
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_set_policy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_policy(osal_device_handle_t handle, e_osal_pm_policy_t policy, bool applyImmediate, st_osal_time_t const* p_abs_timeout)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    e_osal_pm_policy_t  next_policy;
    e_osal_pm_state_t   current_state;

    if (OSAL_PM_REQUIRED_STATE_RELEASED == handle->pm_handle->pm_required_state)
    {
        /* Change the HW status */
        next_policy = r_osal_dcfg_pm_get_next_policy(handle, policy);

        osal_ret = r_osal_dcfg_pm_set_power_clock_state(handle, next_policy);

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_pm_convert_policy(next_policy, &current_state);
        }

        /* Wait for changing status */
        if ((OSAL_RETURN_OK == osal_ret) &&
            (true == applyImmediate))
        {
            osal_ret = r_osal_dcfg_pm_wait_for_required_state(handle, current_state, p_abs_timeout);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (current_state < handle->pm_handle->pm_lower_state)
            {
                handle->pm_handle->pm_lower_state = current_state;
            }
        }
    } /* end of if (OSAL_PM_REQUIRED_STATE_RELEASED == handle->pm_handle->pm_required_state) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->pm_handle->pm_policy = policy;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_set_policy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_025
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_set_reset_state()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_set_reset_state(osal_device_handle_t handle, e_osal_pm_reset_state_t reset_state)
{
    e_osal_return_t         osal_ret;
    e_osal_pm_reset_state_t current_reset_state = OSAL_PM_RESET_STATE_INVALID;
    int                     reset_value; /* PRQA S 5209 # Used as the parameter of ioctl. */

    if (OSAL_PM_RESET_STATE_APPLIED == reset_state)
    {
        reset_value = 1;
    }
    else
    {
        reset_value = 0;
    }

    osal_ret = r_osal_dcfg_pm_get_reset_state(handle, &current_reset_state);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_PM_RESET_STATE_INVALID == current_reset_state)
        {
            osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (reset_state != current_reset_state)
        {
            osal_ret = R_OSAL_OS_PmSetResetState(handle, reset_value);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_set_reset_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_026
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_mutex_lock_for_time_period()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_mutex_lock_for_time_period(osal_os_mutex_t mutex)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(mutex, OSAL_INNER_TIMEOUT);
    if (OSAL_RETURN_TIME == osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_mutex_lock_for_time_period()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_027
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(mutex, p_abs_timeout);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_028
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_ns_sleep()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_ns_sleep(osal_nano_sec_t timeout)
{
    e_osal_return_t osal_ret;
    st_osal_time_t  sleep_time;

    osal_ret = R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(&sleep_time);

    if (OSAL_RETURN_OK == osal_ret)
    {
        sleep_time.tv_sec   += timeout / 1000000000;
        sleep_time.tv_nsec  += (int32_t)(timeout % 1000000000);
        if (OSAL_MAX_TV_NSEC < sleep_time.tv_nsec)
        {
            sleep_time.tv_sec++;
            sleep_time.tv_nsec = (int32_t)(sleep_time.tv_nsec - (OSAL_MAX_TV_NSEC + 1));
        }

        osal_ret = R_OSAL_OS_ThreadSleep(&sleep_time);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_ns_sleep()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_029
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_convert_state()
***********************************************************************************************************************/
/* PRQA S 5209 1 # Used as the parameter of ioctl. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_convert_state(int pwr_status, int clk_status, e_osal_pm_state_t* const p_state)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if ((1 == pwr_status) && (1 == clk_status))
    {
        *p_state = OSAL_PM_STATE_ENABLED;
    }
    else if ((1 == pwr_status) && (0 == clk_status))
    {
        *p_state = OSAL_PM_STATE_CG;
    }
    else if ((0 == pwr_status) && (0 == clk_status))
    {
        *p_state = OSAL_PM_STATE_PG;
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_convert_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_030
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_convert_policy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_pm_convert_policy(e_osal_pm_policy_t policy, e_osal_pm_state_t* const p_state)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    switch (policy)
    {
        case OSAL_PM_POLICY_PG :
            *p_state = OSAL_PM_STATE_PG;
            break;

        case OSAL_PM_POLICY_CG :
            *p_state = OSAL_PM_STATE_CG;
            break;

        case OSAL_PM_POLICY_HP :
            *p_state = OSAL_PM_STATE_ENABLED;
            break;

        default :
            osal_ret = OSAL_RETURN_FAIL;
            break;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_convert_policy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_031
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_pm_get_next_policy()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_pm_policy_t r_osal_dcfg_pm_get_next_policy(const osal_device_handle_t handle, e_osal_pm_policy_t policy)
{
    e_osal_pm_policy_t next_policy;

    /* Power shared device can not power off */
    if ((OSAL_PM_DOMAIN_SHARED == handle->pm_handle->power_shared) && (OSAL_PM_POLICY_PG == policy))
    {
        next_policy = OSAL_PM_POLICY_CG;
    }
    else
    {
        next_policy = policy;
    }

    return next_policy;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_pm_get_next_policy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_035]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
