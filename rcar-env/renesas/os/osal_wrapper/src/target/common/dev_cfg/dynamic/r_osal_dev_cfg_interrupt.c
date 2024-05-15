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
* File Name :    r_osal_dev_cfg_interrupt.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configuration Functions for Interrupt Manager
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include "target/common/dev_cfg/dynamic/r_osal_dev_cfg.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variable
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @var     gs_interrupt_init_state
 * @brief   Status of interrupt manager.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_interrupt_init_state = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Common function for lock the mutex for time period.
 * @param[in]        mutex    A handle of mutex information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_interrupt_mutex_lock_for_time_period(osal_os_mutex_t mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex until timestamp.
 * @param[in]        mutex            A handle of mutex information.
 * @param[in]        p_abs_timeout    Wait on a condition variable with timeout until time stamp.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_interrupt_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_InterruptGetInitStatus(void)
{
    return gs_interrupt_init_state;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_DCFG_InterruptSetInitStatus(e_osal_init_status_t status)
{
    gs_interrupt_init_state = status;
    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptControlInit(void)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_InterruptInit();

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptControlDeinit(void)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_InterruptDeinit();

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptCheckBusy(void)
{
    /* Do nothing on Linux */
    return OSAL_RETURN_OK;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptRegisterIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptRegisterIsr(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value, p_osal_isr_func_t irqHandlerFunction, void* p_irq_arg)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    st_osal_time_t  abs_timeout;

    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(OSAL_INNER_TIMEOUT, &abs_timeout);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_interrupt_mutex_lock_until_time_stamp(handle->int_mtx, &abs_timeout);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check the handle and parameter */
            if ((OSAL_IO_HANDLE_ID != handle->handle_id) || ((size_t)0 == handle->irq_num))
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
            else if (handle->irq_num <= irq_channel)
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                /* Do nothing */
            }

            /* Function procedure */
            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->int_handle[irq_channel].abs_timeout = abs_timeout;
                osal_ret = R_OSAL_OS_InterruptRegisterIsr(handle, irq_channel, priority_value, irqHandlerFunction, p_irq_arg);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_dcfg_interrupt_mutex_lock_until_time_stamp */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptRegisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptUnregisterIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptUnregisterIsr(osal_device_handle_t handle, uint64_t irq_channel, p_osal_isr_func_t irqHandlerFunction)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_interrupt_mutex_lock_for_time_period(handle->int_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle and parameter */
        if ((OSAL_IO_HANDLE_ID != handle->handle_id) || ((size_t)0 == handle->irq_num))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (handle->irq_num <= irq_channel)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }

        /* Function procedure */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_InterruptUnregisterIsr(handle, irq_channel, irqHandlerFunction);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptUnregisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptEnableIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    st_osal_time_t  abs_timeout;

    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(OSAL_INNER_TIMEOUT, &abs_timeout);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_interrupt_mutex_lock_until_time_stamp(handle->int_mtx, &abs_timeout);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check the handle and parameter */
            if ((OSAL_IO_HANDLE_ID != handle->handle_id) || ((size_t)0 == handle->irq_num))
            {
                osal_ret = OSAL_RETURN_HANDLE;
            }
            else if (handle->irq_num <= irq_channel)
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                /* Do nothing */
            }

            /* Function procedure */
            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->int_handle[irq_channel].abs_timeout = abs_timeout;
                osal_ret = R_OSAL_OS_InterruptEnableIsr(handle, irq_channel);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_dcfg_interrupt_mutex_lock_until_time_stamp */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptEnableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptDisableIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptDisableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_interrupt_mutex_lock_for_time_period(handle->int_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle and parameter */
        if ((OSAL_IO_HANDLE_ID != handle->handle_id) || ((size_t)0 == handle->irq_num))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (handle->irq_num <= irq_channel)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }

        /* Function procedure */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_InterruptDisableIsr(handle, irq_channel);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptDisableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptIsISRContext()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptIsISRContext(bool* const p_is_isr)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_IsISRContext(p_is_isr);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptIsISRContext()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptSetIsrPriority()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptSetIsrPriority(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_interrupt_mutex_lock_for_time_period(handle->int_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle and parameter */
        if ((OSAL_IO_HANDLE_ID != handle->handle_id) || ((size_t)0 == handle->irq_num))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (handle->irq_num <= irq_channel)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }

        /* Function procedure */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_InterruptSetIsrPriority(handle, irq_channel, priority_value);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptSetIsrPriority()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptGetNumOfIrqChannels()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_DCFG_InterruptGetNumOfIrqChannels(osal_device_handle_t handle, size_t* const p_numOfChannels)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_interrupt_mutex_lock_for_time_period(handle->int_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle */
        if (OSAL_IO_HANDLE_ID != handle->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        /* Function procedure */
        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_numOfChannels = handle->irq_num;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptGetNumOfIrqChannels()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_interrupt_mutex_lock_for_time_period()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_interrupt_mutex_lock_for_time_period(osal_os_mutex_t mutex)
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
* End of function r_osal_dcfg_interrupt_mutex_lock_for_time_period()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_interrupt_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_interrupt_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(mutex, p_abs_timeout);
    if (OSAL_RETURN_TIME == osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_interrupt_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_026]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
