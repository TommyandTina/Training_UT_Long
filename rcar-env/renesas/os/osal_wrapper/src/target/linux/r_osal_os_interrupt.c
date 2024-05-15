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
* File Name :    r_osal_os_interrupt.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for Interrupt
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
/* PRQA S 0602, 0603, 3214 1 # This definition is required to use glibc. */
#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <poll.h>
#include "target/linux/r_osal_os_types.h"
#include <stdio.h> /* must be include under the r_osal_api.h */

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variable
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @var     g_interrupt_thread_key
 * @brief   Interrupt thread key for judgment of ISR context.
***********************************************************************************************************************/
/* PRQA S 3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
pthread_key_t g_interrupt_thread_key;

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Destructor of the interrupt thread key.
 * @param[in]        p_arg    The address of st_osal_thread_control_t.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_interrupt_key_destructor(void* p_arg);

/***********************************************************************************************************************
 * @brief            Start the interrupt thread.
 * @param[in]        p_arg    The address of st_osal_thread_control_t.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void* r_osal_os_interrupt_thread(void* p_arg);

/***********************************************************************************************************************
 * @brief            Start the interrupt thread.
 * @param[in,out]    p_handle    A pointer to the control interrupt of device.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_start(st_osal_interrupt_control_t* p_handle);

/***********************************************************************************************************************
 * @brief            Called from cleanup handler when the interrupt thread is canceled.
 * @param[in]        p_mutex    The address of osal_os_mutex_t.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_interrupt_cleanup(void* p_mutex);

/***********************************************************************************************************************
 * @brief            Call the user function with disable pthread_cancel().
 * @param[in]        p_handle    A pointer to the control interrupt of device.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_execute_userfunction(const st_osal_interrupt_control_t* p_handle);

/***********************************************************************************************************************
 * @brief            Create the interrupt thread.
 * @param[in,out]    handle            A handle of device information.
 * @param[in]        irq_channel       To set the value of Irq channel number.
 * @param[in]        priority_value    To set the value of priority.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_create(const osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value);

/***********************************************************************************************************************
 * @brief            Set the attribute for the interrupt thread.
 * @param[in]        p_attr            A pointer to the thread attributes object.
 * @param[in]        priority_value    Priority for interrupt.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_attr(pthread_attr_t* p_attr, e_osal_interrupt_priority_t priority_value);

/***********************************************************************************************************************
 * @brief            Set flag of the ISR context to the interrupt thread key.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_isr(void);

/***********************************************************************************************************************
 * @brief            Set the task name.
 * @param[in]        spi_num         Store the SPI number of device
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_task_name(uint64_t spi_num);

/***********************************************************************************************************************
 * @brief            Set the CPU affinity.
 * @param[in]        spi_num         Store the SPI number of device
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_cpu_affinity(uint64_t spi_num);

/***********************************************************************************************************************
 * @brief            Wait for called R_OSAL_OS_InterruptEnableIsr().
 * @param[in]        p_handle    A pointer to the control interrupt information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_wait_enable(const st_osal_interrupt_control_t* p_handle);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptInit(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    linux_ret = pthread_key_create(&g_interrupt_thread_key, r_osal_os_interrupt_key_destructor);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_102
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptDeinit(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    linux_ret = pthread_key_delete(g_interrupt_thread_key);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_103
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptRegisterIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptRegisterIsr(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value, p_osal_isr_func_t irqHandlerFunction, void* p_irq_arg)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;

    /* p_irq_arg is possible NULL */
    if ((NULL == handle) ||
        (NULL == irqHandlerFunction))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if ((true == handle->int_handle[irq_channel].irq_enable) ||
             (NULL != handle->int_handle[irq_channel].irq_func))
    {
        osal_ret = OSAL_RETURN_BUSY;
    }
    else if (NULL != handle->int_handle[irq_channel].irq_thread)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Create and start the interrupt thread */
        osal_ret = R_OSAL_OS_MutexTryLock(handle->int_handle[irq_channel].irq_thread_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            handle->int_handle[irq_channel].irq_func    = irqHandlerFunction;
            handle->int_handle[irq_channel].irq_arg     = p_irq_arg;
            handle->int_handle[irq_channel].irq_use     = true;

            osal_ret = r_osal_os_interrupt_thread_create(handle, irq_channel, priority_value);

            mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_handle[irq_channel].irq_thread_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            handle->int_handle[irq_channel].irq_use     = false;
            handle->int_handle[irq_channel].irq_func    = NULL;
            handle->int_handle[irq_channel].irq_arg     = NULL;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptRegisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_104
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptUnregisterIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_InterruptUnregisterIsr(osal_device_handle_t handle, uint64_t irq_channel, p_osal_isr_func_t irqHandlerFunction)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    bool            is_isr;
    void*           p_res;

    if ((NULL == handle) ||
        (NULL == irqHandlerFunction))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if (irqHandlerFunction != handle->int_handle[irq_channel].irq_func)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (true == handle->int_handle[irq_channel].irq_enable)
    {
        osal_ret = OSAL_RETURN_BUSY;
    }
    else if (NULL == handle->int_handle[irq_channel].irq_thread)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    /* Interrupt thread cancel */
    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->int_handle[irq_channel].irq_func    = NULL;
        handle->int_handle[irq_channel].irq_arg     = NULL;

        linux_ret = pthread_cancel(handle->int_handle[irq_channel].irq_thread->thread);
        if ((0 != linux_ret) && (ESRCH != linux_ret)) /* ESRCH: Interrupt thread finished before calling to pthread_cancel */
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (false == is_isr)
        {
            /* Check the Interrupt thread:
             * p_res is PTHREAD_CANCELED:   Interrupt thread was canceled successfully.
             * p_res is NULL:               Interrupt thread was finished before the cancellation complete.
             * other value:                 return OSAL_RETURN_FAIL */
            linux_ret = pthread_join(handle->int_handle[irq_channel].irq_thread->thread, &p_res);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            /* PRQA S 0326 1 # PTHREAD_CANCELED is used in accordance with the specification. */
            else if ((PTHREAD_CANCELED != p_res) && (NULL != p_res))
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
            /* If context is interrupt thread, only execute detach */
            linux_ret = pthread_detach(handle->int_handle[irq_channel].irq_thread->thread);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        } /* end of if (false == is_isr) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)handle->int_handle[irq_channel].irq_thread);
        handle->int_handle[irq_channel].irq_thread          = NULL;
        handle->int_handle[irq_channel].irq_thread_active   = false;
        handle->int_handle[irq_channel].irq_use             = false;
    }

    return osal_ret;
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptUnregisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_105
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptEnableIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;
    bool            is_isr;
    uint32_t        unmask = 1;

    if ((NULL == handle) ||
        (NULL == handle->int_handle[irq_channel].irq_func))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* If already enabled, do nothing */
        if (false == handle->int_handle[irq_channel].irq_enable)
        {
            osal_ret = R_OSAL_OS_MutexTimedLock(handle->int_handle[irq_channel].irq_mtx, &handle->int_handle[irq_channel].abs_timeout);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }

            /* Enable interrupt thread */
            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->int_handle[irq_channel].irq_enable = true;

                osal_ret = R_OSAL_OS_IsISRContext(&is_isr);

                if ((OSAL_RETURN_OK == osal_ret) &&
                    (false == is_isr))
                {
                    /* PRQA S 0314 1 # The 2nd arguments must be (void *) but no side effects in this case. */
                    osal_ret = R_OSAL_OS_Write(handle->int_handle[irq_channel].dev_fd, (void *)&unmask, sizeof(unmask));
                    if (OSAL_RETURN_OK != osal_ret)
                    {
                        handle->int_handle[irq_channel].irq_enable = false;
                    }
                }

                if (OSAL_RETURN_OK == osal_ret)
                {
                    osal_ret = R_OSAL_OS_CondSignal(handle->int_handle[irq_channel].irq_cond);
                }

                mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_handle[irq_channel].irq_mtx);
                if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
                {
                    osal_ret = mutex_ret;
                }
            } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from R_OSAL_OS_MutexTimedLock */
        } /* end of if (false == handle->int_handle[irq_channel].irq_enable) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from if ((NULL == handle) || (NULL == handle->int_handle[irq_channel].irq_func)) */

    return osal_ret;
/* PRQA S 9110 1 # It is a series of processes and is necessary so as not to reduce the readability based on design. */
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptEnableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_106
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptDisableIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_InterruptDisableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            is_isr;
    uint32_t        mask = 0;

    if ((NULL == handle) ||
        (NULL == handle->int_handle[irq_channel].irq_func))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    /* Disable interrupt thread */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
    }

    if ((OSAL_RETURN_OK == osal_ret) &&
        (false == is_isr))
    {
        /* PRQA S 0314 1 # The 2nd arguments must be (void *) but no side effects in this case. */
        osal_ret = R_OSAL_OS_Write(handle->int_handle[irq_channel].dev_fd, (void *)&mask, sizeof(mask));
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->int_handle[irq_channel].irq_enable = false;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptDisableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_107
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptSetIsrPriority()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_InterruptSetIsrPriority(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if ((NULL == handle) ||
        (NULL == handle->int_handle[irq_channel].irq_thread))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209 2 # The 2nd parameter of this function must be int. */
        linux_ret = pthread_setschedprio(handle->int_handle[irq_channel].irq_thread->thread, (int)priority_value + OSAL_INTERRUPT_PRIORITY_BASE);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptSetIsrPriority()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_108
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_key_destructor()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_interrupt_key_destructor(void *p_arg)
{
    R_OSAL_OS_Free(p_arg);
    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_key_destructor()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_109
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_thread()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
OSAL_STATIC void* r_osal_os_interrupt_thread(void* p_arg)
{
    e_osal_return_t                 osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                             linux_ret;
    /* PRQA S 0316 1 # This casting is always compatible to the st_osal_interrupt_control_t*. */
    st_osal_interrupt_control_t*    p_int_handle = (st_osal_interrupt_control_t *)p_arg;
    uint32_t                        unmask = 1;
    uint32_t                        buffer;
    struct pollfd                   fds[1];

    osal_ret = r_osal_os_interrupt_thread_start(p_int_handle);

    while ((OSAL_RETURN_OK == osal_ret) && (true == p_int_handle->irq_thread_active))
    {
        fds[0].fd = p_int_handle->dev_fd;
        fds[0].events = POLLIN;

        linux_ret = poll(fds, 1, -1); /* timeout is not used */
        if (1 <= linux_ret)
        {
            /* PRQA S 0314 1 # The 2nd arguments must be (void *) but no side effects in this case. */
            osal_ret = R_OSAL_OS_Read(p_int_handle->dev_fd, (void *)&buffer, sizeof(buffer));
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_interrupt_execute_userfunction(p_int_handle);
            }

            /* Unmask for next interrupt */
            if (true == p_int_handle->irq_enable)
            {
                /* PRQA S 0314 1 # The 2nd arguments must be (void *) but no side effects in this case. */
                osal_ret = R_OSAL_OS_Write(p_int_handle->dev_fd, (void *)&unmask, sizeof(unmask));
            }
        }

        if ((linux_ret < 1) || (OSAL_RETURN_OK != osal_ret))
        {
            break;
        }
    }

    p_int_handle->irq_thread_active = false;

    return NULL;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_thread()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_110
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_thread_start()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_start(st_osal_interrupt_control_t* p_handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = R_OSAL_OS_MutexTryLock(p_handle->irq_mtx);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Start the interrupt thread */
        osal_ret = R_OSAL_OS_MutexTimedLock(p_handle->irq_thread_mtx, &p_handle->abs_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            p_handle->irq_thread_active = true;

            osal_ret = r_osal_os_interrupt_set_isr();

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_interrupt_set_task_name(p_handle->spi_num);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_interrupt_set_cpu_affinity(p_handle->spi_num);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_OS_CondSignal(p_handle->irq_cond);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(p_handle->irq_thread_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        }

        /* Wait the executing R_OSAL_OS_InterruptEnableIsr() */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_interrupt_wait_enable(p_handle);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(p_handle->irq_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
/* PRQA S 9104,9107 2 # The initialization function has already multiple sub-functions to reduce complexity.
   We have decided that further division is not desirable from perspective of maintainability and readability. */
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_thread_start()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_111
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_cleanup()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the 1st parameter of pthread_cleanup_push(). */
OSAL_STATIC void r_osal_os_interrupt_cleanup(void* p_mutex)
{
    /* PRQA S 0316 1 # This casting is always compatible to the osal_os_mutex_t. */
    (void)R_OSAL_OS_MutexUnlock((osal_os_mutex_t)p_mutex);
    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_cleanup()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_112
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_execute_userfunction()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_execute_userfunction(const st_osal_interrupt_control_t* p_handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 2 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    int             oldstate;

    if ((NULL != p_handle->irq_func) && (true == p_handle->irq_enable))
    {
        /* pthread_cancel is blocked until the user function finish */
        linux_ret = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            p_handle->irq_func(p_handle->dev_handle, p_handle->irq_channel, p_handle->irq_arg);

            linux_ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_execute_userfunction()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_113
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_thread_create()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_create(const osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value)
{
    e_osal_return_t     osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                 linux_ret;
    /* PRQA S 0759 1 # This object is used in accordance with the specification of pthread. */
    pthread_attr_t      attr;

    /* Create the interrupt thread */
    osal_ret = r_osal_os_interrupt_init_attr(&attr, priority_value);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        handle->int_handle[irq_channel].irq_thread = (osal_os_thread_t)R_OSAL_OS_Malloc(sizeof(st_osal_os_thread_t));
        if (NULL == handle->int_handle[irq_channel].irq_thread)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0314 1 # The 3rd arguments must be (void *) but no side effects in this case. */
            linux_ret = pthread_create(&handle->int_handle[irq_channel].irq_thread->thread, &attr, r_osal_os_interrupt_thread, (void*)(&handle->int_handle[irq_channel]));
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }

        linux_ret = pthread_attr_destroy(&attr);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    /* Wait for starting the interrupt thread */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_CondTimedWait(handle->int_handle[irq_channel].irq_cond, handle->int_handle[irq_channel].irq_thread_mtx, &handle->int_handle[irq_channel].abs_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (true != handle->int_handle[irq_channel].irq_thread_active)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_thread_create()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_114
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_init_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_attr(pthread_attr_t* p_attr, e_osal_interrupt_priority_t priority_value)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                 linux_ret;
    struct sched_param  int_thread_priority;

    linux_ret = pthread_attr_init(p_attr);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_attr_setinheritsched(p_attr, PTHREAD_EXPLICIT_SCHED);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            linux_ret = pthread_attr_setschedpolicy(p_attr, SCHED_FIFO);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 5209 1 # Used to set the 2nd parameter of pthread_attr_setschedparam(). */
            int_thread_priority.sched_priority = (int)priority_value + OSAL_INTERRUPT_PRIORITY_BASE;

            linux_ret = pthread_attr_setschedparam(p_attr, &int_thread_priority);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)pthread_attr_destroy(p_attr);
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_init_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_115
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_set_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_isr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int*            p_key_value; /* PRQA S 5209 # Used to set 2nd parameter of pthread_setspecific(). */

    /* PRQA S 5209 2 # Used to set 2nd parameter of pthread_setspecific(). */
    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    p_key_value = (int *)R_OSAL_OS_Malloc(sizeof(int));
    if (p_key_value == NULL)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_key_value = 1;

        /* PRQA S 0314 1 # The 2nd arguments must be (void *) but no side effects in this case. */
        (void)pthread_setspecific(g_interrupt_thread_key, (const void *)p_key_value);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_set_isr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_116
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_set_task_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_task_name(uint64_t spi_num)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    /* Maximum length of thread name is 16byte (include \0) */
    char            thread_name[16];

    linux_ret = snprintf(thread_name, sizeof(thread_name), "int_task%03lu", spi_num);
    if (linux_ret < 0)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_setname_np(pthread_self(), thread_name);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_set_task_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_117
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_set_cpu_affinity()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_cpu_affinity(uint64_t spi_num)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    uint64_t        cnt;
    cpu_set_t       cpu_set;
    uint64_t        affinity = 0;

    cnt = 0;
    while (OSAL_INTERRUPT_AFFINITY_INVALID_ID != g_osal_interrupt_thread_affinity[cnt].spi_num)
    {
        if (spi_num == g_osal_interrupt_thread_affinity[cnt].spi_num)
        {
            affinity = g_osal_interrupt_thread_affinity[cnt].affinity;
            break;
        }
        cnt++;
    }

    if (0U != affinity)
    {
        /* PRQA S 0315, 3200, 4413 1 # These messages are to the toolchain definition. */
        CPU_ZERO(&cpu_set);

        for (cnt = 0; cnt < 64U; cnt++)
        {
            if (0U != (affinity & ((uint64_t)1 << cnt)))
            {
                /* PRQA S 0602, 1021, 1840, 1844, 3200, 3226, 3396, 3397 1 # These messages are to the toolchain definition. */
                CPU_SET(cnt, &cpu_set);
            }
        }

        linux_ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_set_cpu_affinity()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_INT_118
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_wait_enable()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_wait_enable(const st_osal_interrupt_control_t* p_handle)
{
    volatile e_osal_return_t    osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                         linux_ret;

    /* PRQA S 0314 3 # The 2nd arguments must be (void *) but no side effects in this case. */
    /* PRQA S 0316 2 # Required cast on system, not modifiable. */
    /* PRQA S 0602, 1336, 3344 1 # These messages are to the toolchain definition. */
    pthread_cleanup_push(&r_osal_os_interrupt_cleanup, (void *)p_handle->irq_mtx);

    linux_ret = pthread_cond_wait(&p_handle->irq_cond->cond, &p_handle->irq_mtx->mutex);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    /* PRQA S 2742, 2880 1 # These messages are to the toolchain definition. */
    pthread_cleanup_pop(0);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_wait_enable()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_INT_022]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
