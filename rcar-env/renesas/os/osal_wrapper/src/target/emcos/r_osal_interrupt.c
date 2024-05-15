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
* File Name :    r_osal_interrupt.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper Interrupt Manager for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mcos/pdev.h>
#include <mcos/uring.h>
#include <sys/lfs.h>

#include "target/emcos/r_osal_common.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_Interrupt Interrupt Manager Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_io_is_initialized
 * Initialize flag
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_interrupt_is_initialized = false;

/** @} */

/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Functions_Interrupt Interrupt Manager Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            An internal thread to call user handler.
* @param[in]        p_arg        The pointor of st_osal_interrupt_thread_control_t.
* @return           void *
* @retval           NULL
***********************************************************************************************************************/
OSAL_STATIC void *osal_internal_user_thread(void *p_arg);

/*******************************************************************************************************************//**
* @brief            An internal thread for receiving interrupts.
* @param[in]        p_arg        The pointor of st_osal_pdev_interrupt_t.
* @return           void *
* @retval           NULL
***********************************************************************************************************************/
OSAL_STATIC void *osal_internal_receive_thread(void *p_arg);

/*******************************************************************************************************************//**
* @brief            Set mutex attributes.
* @param[in]        p_mutex_attr       The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_set_mutex_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Initialize the interrupt information based on the device information.
* @param[in]        p_mutex_attr      The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_init_with_dev(const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Get the number of device and interrupt thread.
* @param[out]       p_device_num      The pointor of device number.
* @param[out]       p_interrupt_num   The pointor of interrupt thread number.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_interrupt_get_num_of_cfg(size_t *p_device_num, size_t *p_interrupt_num);

/*******************************************************************************************************************//**
* @brief            Init internal interrupt thread.
* @param[in]        p_device_control  The pointor of device control information.
* @param[in]        num_of_interrupt  Interrupt thread number.
* @param[in]        p_mutex_attr      The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_init_internal_thread(st_osal_device_control_t *p_device_control,
                                                               size_t num_of_interrupt,
                                                               const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Find interrupt control information.
* @param[in]        p_device_control  The pointor of device control information.
* @param[in]        num_of_interrupt  Interrupt thread number.
* @param[in]        channel_count     Chennel count.
* @param[in]        p_mutex_attr      The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_find_interrupt_info(st_osal_device_control_t *p_device_control,
                                                              size_t num_of_interrupt,
                                                              size_t channel_count,
                                                              const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Compare device id.
* @param[in]        p_str1  The first comparison device id
* @param[in]        p_str2  The second comparison device id
* @param[in]        len     Comparison device id size
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_compare_id(const char *p_str1, const char *p_str2, size_t len);

/*******************************************************************************************************************//**
* @brief            Initialize Interrupt Thread Information.
* @param[in/out]    p_info          Specify internal management data for interrupt thread control.
* @param[in]        p_device_handle Device handle acquired by "R_OSAL_IoOpen".
* @param[in]        p_mutex_attr    The pointor of mutex attribute object.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_initialize(st_osal_interrupt_thread_control_t *p_info,
                                                               st_osal_device_control_t *p_device_handle,
                                                               const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Get interrupt channel number of pdev entry.
* @param[in/out]    p_interrupt_control Specify internal management data for interrupt thread control.
* @param[in]        p_device_handle     Device handle acquired by "R_OSAL_IoOpen".
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_pdev_int_channel(st_osal_interrupt_thread_control_t *p_interrupt_control,
                                                               const st_osal_device_control_t *p_device_handle);

/*******************************************************************************************************************//**
* @brief            Set user thread attributes.
* @param[in/out]    p_thread_attr      The pointor of thread attributes.
* @param[in]        p_usr_config       Specify internal management data for interrupt thread control.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_set_thread_attr(pthread_attr_t *p_thread_attr,
                                                                    const struct st_osal_interrupt_thread_config *p_usr_config);

/*******************************************************************************************************************//**
* @brief            Create the uring and the receive thread.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_create_uring_and_thread(void);

/*******************************************************************************************************************//**
* @brief            Set interrupt control information to the pdev entry information.
* @param[in/out]    p_pdev_entry_info   The pointer of st_osal_pdev_entry_t.
* @param[in]        interrupt_count     The interrupt count of pdev entry.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_int_ctl_to_pdev(st_osal_pdev_entry_t *p_pdev_entry_info,
                                                              uint32_t interrupt_count);

/*******************************************************************************************************************//**
* @brief            Create the receive thread.
* @param[in/out]    p_pdev_entry_info   The pointer of st_osal_pdev_entry_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_create_rev_thread(st_osal_pdev_entry_t *p_pdev_entry_info);

/*******************************************************************************************************************//**
* @brief            Set receive thread attributes.
* @param[in/out]    p_thread_attr      The pointor of thread attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_rev_thread_attr(pthread_attr_t *p_thread_attr);

/*******************************************************************************************************************//**
* @brief            Terminates the interrupt user thread.
* @param[in]        num_of_interrupt  Interrupt thread number.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_join_user_thread(size_t num_of_interrupt);

/*******************************************************************************************************************//**
* @brief            Terminates the interrupt user thread (processing).
* @param[in]        p_interrupt_control  The pointor of Interrupt control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_join_user_thread_proc(st_osal_interrupt_thread_control_t  *p_interrupt_control);

/*******************************************************************************************************************//**
* @brief            Notify user thread of termination
* @param[in]        p_interrupt_control  The pointor of Interrupt control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_notify_user_th_termination(st_osal_interrupt_thread_control_t  *p_interrupt_control);

/*******************************************************************************************************************//**
* @brief            Terminates the interrupt reception thread.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_join_rev_thread(void);

/*******************************************************************************************************************//**
* @brief            Waiting for user thread
* @param[in]        p_interrupt_control  The pointor of Interrupt control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_wait_user_thread(st_osal_interrupt_thread_control_t *p_interrupt_control);

/*******************************************************************************************************************//**
* @brief            Call user function registered with the interrupt number.
* @param[in]        p_interrupt_control  The pointor of Interrupt control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_call_handler(const st_osal_interrupt_thread_control_t *p_interrupt_control);

/*******************************************************************************************************************//**
* @brief            Set interrupt configuration
* @param[in]        p_pdev_entry_info   The pointor of pdev entry information
* @param[out]       interrupt_count     Interrupt count
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_interrupt_config(const st_osal_pdev_entry_t * p_pdev_entry_info,
                                                               uint32_t interrupt_count);

/*******************************************************************************************************************//**
* @brief            Notification to interrupt user thread
* @param[in]        p_pdev_entry_info   The pointor of pdev entry information
* @param[in]        interrupt_count     Interrupt count
* @param[in]        interrupt_index     The index of interrupt control information
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_thread_signal(const st_osal_pdev_entry_t * p_pdev_entry_info,
                                                        uint32_t interrupt_count,
                                                        uint32_t interrupt_index);

/*******************************************************************************************************************//**
* @brief            Lock a mutex(timed lock).
* @param[in]        p_info       The pointor of interrupt thread control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_timedlock_mutex(st_osal_interrupt_thread_control_t *p_info);

/*******************************************************************************************************************//**
* @brief            Unlock a mutex.
* @param[in]        p_info       The pointor of interrupt thread control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_unlock_mutex(st_osal_interrupt_thread_control_t *p_info);

/*******************************************************************************************************************//**
* @brief            Deinitialize Interrupt Manager process.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_deinit_pre_proc(void);

/*******************************************************************************************************************//**
* @brief            Internal interrupt registers ISR.
* @param[in]        handle              The address of osal_device_handle_t.
* @param[in]        irq_channel         To set the value of Irq channel number.
* @param[in]        irqHandlerFunction  pointer of CallBackFucntion.
* @param[in]        p_irq_arg           interrupt handler function's argument.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_reg_isr(osal_device_handle_t const handle,
                                                            uint64_t irq_channel,
                                                            p_osal_isr_func_t irqHandlerFunction,
                                                            void* p_irq_arg);

/*******************************************************************************************************************//**
* @brief            Internal interrupt unregisters ISR.
* @param[in]        handle              The address of osal_device_handle_t.
* @param[in]        irq_channel         To set the value of Irq channel number.
* @param[in]        irqHandlerFunction  pointer of CallBackFucntion.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_unreg_isr(osal_device_handle_t const handle,
                                                              uint64_t irq_channel,
                                                              p_osal_isr_func_t irqHandlerFunction);

/*******************************************************************************************************************//**
* @brief            Internal interrupt enable ISR.
* @param[in]        handle              The address of osal_device_handle_t.
* @param[in]        irq_channel         To set the value of Irq channel number.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_enable_isr(osal_device_handle_t const handle, uint64_t irq_channel);

/*******************************************************************************************************************//**
* @brief            Internal interrupt enable ISR process.
* @param[in]        handle               The address of osal_device_handle_t.
* @param[in]        p_interrupt_control  The pointor of Interrupt control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_enable_isr_proc(osal_device_handle_t const handle,
                                                                    st_osal_interrupt_thread_control_t *p_interrupt_control);

/*******************************************************************************************************************//**
* @brief            Internal interrupt disable ISR.
* @param[in]        handle              The address of osal_device_handle_t.
* @param[in]        irq_channel         To set the value of Irq channel number.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_disable_isr(osal_device_handle_t const handle, uint64_t irq_channel);

/*******************************************************************************************************************//**
* @brief            Internal interrupt disable ISR process.
* @param[in]        handle               The address of osal_device_handle_t.
* @param[in]        p_interrupt_control  The pointor of Interrupt control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_disable_isr_proc(osal_device_handle_t const handle,
                                                                     st_osal_interrupt_thread_control_t *p_interrupt_control);

/*******************************************************************************************************************//**
* @brief            Set interrupt thread priority.
* @param[in]        handle              The address of osal_device_handle_t.
* @param[in]        irq_channel         To set the value of Irq channel number.
* @param[in]        priority_value      To set the value of priority.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_set_isr_priority(osal_device_handle_t const handle,
                                                                     uint64_t irq_channel,
                                                                     e_osal_interrupt_priority_t priority_value);

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_InterruptGetNumOfIrqChannels()
* XOS3_OSAL_EMCOS_CD_CD_INT_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_014]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_015]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_InterruptGetNumOfIrqChannels(osal_device_handle_t handle,
                                                    size_t *const p_numOfChannels)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
    True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_DEVICE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
             (false == handle->active))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (NULL == p_numOfChannels)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret         = OSAL_RETURN_OK;
        *p_numOfChannels = handle->irq_channels;
    }   /* end of if(false == gs_osal_interrupt_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_InterruptGetNumOfIrqChannels()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_InterruptRegisterIsr()
* XOS3_OSAL_EMCOS_CD_CD_INT_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_017]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_018]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptRegisterIsr(osal_device_handle_t handle,
                                            uint64_t irq_channel,
                                            e_osal_interrupt_priority_t priority_value,
                                            p_osal_isr_func_t irqHandlerFunction,
                                            void *p_irq_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_DEVICE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
              True if the correct handle was passed. False detection of the tool. */
             (false == handle->active) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((irq_channel >= handle->irq_channels) ||
             (NULL == irqHandlerFunction) ||
             (OSAL_INTERRUPT_PRIORITY_LOWEST > priority_value) ||
             (OSAL_INTERRUPT_PRIORITY_HIGHEST < priority_value))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_interrupt_reg_isr(handle, irq_channel, irqHandlerFunction, p_irq_arg);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_InterruptRegisterIsr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_InterruptUnregisterIsr()
* XOS3_OSAL_EMCOS_CD_CD_INT_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_020]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_021]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptUnregisterIsr(osal_device_handle_t handle,
                                              uint64_t irq_channel,
                                              p_osal_isr_func_t irqHandlerFunction)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_DEVICE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
             (false == handle->active) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((irq_channel >= handle->irq_channels) ||
             (NULL == irqHandlerFunction))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_interrupt_unreg_isr(handle, irq_channel, irqHandlerFunction);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_InterruptUnregisterIsr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_InterruptEnableIsr()
* XOS3_OSAL_EMCOS_CD_CD_INT_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_011]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_012]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_013]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptEnableIsr(osal_device_handle_t handle,
                                          uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_DEVICE_HANDLE_ID != handle->handle_id) ||
             (false == handle->active) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
              True if the correct handle was passed. False detection of the tool. */
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (irq_channel >= handle->irq_channels)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_interrupt_enable_isr(handle, irq_channel);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_InterruptEnableIsr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_InterruptDisableIsr()
* XOS3_OSAL_EMCOS_CD_CD_INT_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_009]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_010]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptDisableIsr(osal_device_handle_t handle,
                                           uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_DEVICE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
              True if the correct handle was passed. False detection of the tool. */
             (false == handle->active) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (irq_channel >= handle->irq_channels)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_interrupt_disable_isr(handle, irq_channel);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_InterruptDisableIsr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_InterruptIsISRContext()
* XOS3_OSAL_EMCOS_CD_CD_INT_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_016]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptIsISRContext(bool *const p_is_isr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    const void      *p_get_value;
    bool            is_isr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == p_is_isr)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        p_get_value = R_OSAL_Internal_GetThreadSpecific();
        if (NULL != p_get_value)
        {
            is_isr = true;
        }
        else
        {
            is_isr = false;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_is_isr = is_isr;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_InterruptIsISRContext()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_InterruptSetIsrPriority()
* XOS3_OSAL_EMCOS_CD_CD_INT_007
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_019]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptSetIsrPriority(osal_device_handle_t handle,
                                               uint64_t irq_channel,
                                               e_osal_interrupt_priority_t priority_value)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_DEVICE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
              True if the correct handle was passed. False detection of the tool. */
             (false == handle->active) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((irq_channel >= handle->irq_channels) ||
             (OSAL_INTERRUPT_PRIORITY_HIGHEST < priority_value) ||
             (OSAL_INTERRUPT_PRIORITY_LOWEST > priority_value))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_interrupt_set_isr_priority(handle, irq_channel, priority_value);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_InterruptSetIsrPriority()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_interrupt_initialize()
* XOS3_OSAL_EMCOS_CD_CD_INT_008
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_007]
***********************************************************************************************************************/
e_osal_return_t osal_interrupt_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    pthread_mutexattr_t mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_interrupt_is_initialized = true;

        osal_ret = osal_internal_interrupt_set_mutex_attr(&mutex_attr);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_Internal_CreateThreadKey();
            if (OSAL_RETURN_OK == osal_ret)
            {
                /* Do nothing */
            }
            else
            {
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)pthread_mutexattr_destroy(&mutex_attr);
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_interrupt_init_with_dev(&mutex_attr);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_create_uring_and_thread();
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutexattr_destroy(&mutex_attr);
            if ((OSAL_RETURN_OK == osal_ret) && (0 != os_ret))
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

        if (OSAL_RETURN_OK != osal_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)osal_interrupt_deinitialize();
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(true == gs_osal_interrupt_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_interrupt_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_interrupt_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_INT_009
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_005]
***********************************************************************************************************************/
e_osal_return_t osal_interrupt_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret           = OSAL_RETURN_OK;
    e_osal_return_t                    api_ret;
    size_t                             num_of_device      = 0;
    size_t                             num_of_interrupt   = 0;
    size_t                             loop_count         = 0;
    st_osal_device_control_t           *p_device_control  = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_interrupt_is_initialized = false;

    osal_internal_interrupt_get_num_of_cfg(&num_of_device, &num_of_interrupt);
    while ((num_of_device > loop_count) &&
           (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)loop_count,
                                            &p_device_control);
        if ((OSAL_RETURN_OK != api_ret) ||
            (NULL == p_device_control))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if ((0U != p_device_control->irq_channels) &&
                (NULL != p_device_control->interrupt_index))
            {
                /* PRQA S 0314 2 # The 1st parameter must be (void *), and there is no side side-effect in this case. */
                /* PRQA S 5118 1 # Safty Manual,  free use are allowed.No problem because it is called by DeInitilize. */
                free((void *)p_device_control->interrupt_index);
                p_device_control->interrupt_index = NULL;
            }
            else
            {
                /* Do nothing */
            }
        }
        loop_count++;
    }   /* end of while((num_of_device > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_DeleteThreadKey();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_join_user_thread(num_of_interrupt);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_join_rev_thread();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_interrupt_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_interrupt_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_INT_010
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_006]
***********************************************************************************************************************/
e_osal_return_t osal_interrupt_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_interrupt_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_interrupt_is_initialized = false;
        osal_ret = osal_internal_interrupt_deinit_pre_proc();
    }

    if (OSAL_RETURN_BUSY == osal_ret)
    {
        gs_osal_interrupt_is_initialized = true;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_interrupt_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_interrupt_set_is_init()
* XOS3_OSAL_EMCOS_CD_CD_INT_011
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_008]
***********************************************************************************************************************/
void osal_interrupt_set_is_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_interrupt_is_initialized = true;

    return;
}
/***********************************************************************************************************************
* End of function osal_interrupt_set_is_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_user_thread()
* XOS3_OSAL_EMCOS_CD_CD_INT_013
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_002]
***********************************************************************************************************************/
OSAL_STATIC void *osal_internal_user_thread(void *p_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    local_ret          = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                os_ret;
    st_osal_interrupt_thread_control_t *interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_arg)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        interrupt_control = (st_osal_interrupt_thread_control_t *)p_arg;
        interrupt_control->thread_active = true;

        while ((true == gs_osal_interrupt_is_initialized) &&
               (OSAL_RETURN_OK == local_ret))
        {
            local_ret = R_OSAL_Internal_TimedlockMutex(&interrupt_control->inner_mtx,
                                                       interrupt_control->inner_timeout);
            if (OSAL_RETURN_OK == local_ret)
            {
                local_ret = osal_internal_interrupt_wait_user_thread(interrupt_control);

                if (OSAL_RETURN_OK == local_ret)
                {
                    local_ret = osal_internal_interrupt_call_handler(interrupt_control);
                }
                else
                {
                    /* Do nothing */
                }

                os_ret = pthread_mutex_unlock(&interrupt_control->inner_mtx);
                if (0 != os_ret)
                {
                    local_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                local_ret = OSAL_RETURN_FAIL;
            }   /* end of if(OSAL_RETURN_OK == local_ret). */
        }   /* end of while((true == gs_osal_interrupt_is_initialized) && (OSAL_RETURN_OK == local_ret)). */

        interrupt_control->thread_active = false;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != p_arg). */

    return NULL;
}
/***********************************************************************************************************************
* End of function osal_internal_user_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_receive_thread()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the 1st parameter of pthread_create(). */
OSAL_STATIC void *osal_internal_receive_thread(void *p_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    local_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                os_ret;
    uint32_t                           interrupt_count;
    uint32_t                           loop_count;
    const st_osal_pdev_entry_t         *pdev_entry_info;
    struct lfs_devgetdone              recv;
    uint32_t                           interrupt_index;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_arg)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        pdev_entry_info = (st_osal_pdev_entry_t *)p_arg;

        /* PRQA S 2855 2 # It is used according to the specifications defined in eMCOS. */
        /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
        os_ret = posix_devctl(pdev_entry_info->fd, PDEV_INTERRUPT_GET_COUNT, (void *)&interrupt_count, sizeof(uint32_t), NULL);
        if (0 != os_ret)
        {
            local_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            local_ret = osal_internal_set_interrupt_config(pdev_entry_info, interrupt_count);
        }

        while ((true == gs_osal_interrupt_is_initialized) &&
               (OSAL_RETURN_OK == local_ret))
        {
            /* PRQA S 0314 1 # The 2nd parameter must be (void *), and there is no side side-effect in this case. */
            os_ret = uring_interrupt_receive(pdev_entry_info->uring, (void *)&recv, sizeof(struct lfs_devgetdone));
            if (0 != os_ret)
            {
                local_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                interrupt_index = (uint32_t)recv.type >> 8U;
                local_ret = osal_internal_thread_signal(pdev_entry_info, interrupt_count, interrupt_index);
            }
        }

        /* PRQA S 2855 2 # It is used according to the specifications defined in eMCOS. */
        /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
        os_ret = posix_devctl(pdev_entry_info->fd, PDEV_INTERRUPT_GET_COUNT, (void *)&interrupt_count, sizeof(uint32_t), NULL);
        if (0 == os_ret)
        {
            for (loop_count = 0U; interrupt_count > loop_count; loop_count++)
            {
                /* PRQA S 4446 1 # It is used according to the specifications defined in eMCOS. */
                (void)posix_devctl(pdev_entry_info->fd, PDEV_INTERRUPT_UNCONFIG(loop_count), NULL, 0, NULL);
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != p_arg). */

    return NULL;
}
/***********************************************************************************************************************
* End of function osal_internal_receive_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_set_mutex_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_set_mutex_attr(pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutexattr_init(p_mutex_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_mutexattr_settype(p_mutex_attr, (int)PTHREAD_MUTEX_ERRORCHECK);
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_mutexattr_destroy(p_mutex_attr);
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_set_mutex_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_init_with_dev()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_init_with_dev(const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t                   num_of_device      = 0;
    size_t                   num_of_interrupt   = 0;
    size_t                   device_count       = 0;
    st_osal_device_control_t *p_device_control  = NULL;
    e_osal_return_t          osal_ret           = OSAL_RETURN_OK;
    e_osal_return_t          api_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_internal_interrupt_get_num_of_cfg(&num_of_device, &num_of_interrupt);
    if (0U == num_of_interrupt)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        /* Do nothing */
    }

    while ((num_of_device > device_count) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)device_count, &p_device_control);
        if ((OSAL_RETURN_OK != api_ret) || (NULL == p_device_control) || (NULL == p_device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (NULL != p_device_control->interrupt_index)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (0U != p_device_control->irq_channels))
            {
                /* PRQA S 5118,2706 2 # Safty Manual, malloc use are allowed.No problem 
                because  Interrupt Manager Deintialize osal_interrupt_deinitialize free is called */
                p_device_control->interrupt_index = (int32_t *)malloc(sizeof(int32_t) * p_device_control->irq_channels);
                if (NULL == p_device_control->interrupt_index)
                {
                    osal_ret = OSAL_RETURN_MEM;
                }
                else
                {
                    osal_ret = osal_internal_init_internal_thread(p_device_control, num_of_interrupt, p_mutex_attr);
                }
            }
            else
            {
                /* Do nothing */
            }
            /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
        }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == p_device_control) || 
                         (NULL == p_device_control->dev_info)). */
        device_count++;
    }   /* end of while((num_of_device > device_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_init_with_dev()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_get_num_of_cfg()
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_interrupt_get_num_of_cfg(size_t *p_device_num, size_t *p_interrupt_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    *p_device_num    = R_OSAL_DCFG_GetNumOfDevice();
    *p_interrupt_num = R_OSAL_RCFG_GetNumOfInterruptThread();

    return;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_get_num_of_cfg()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_init_internal_thread()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_init_internal_thread(st_osal_device_control_t *p_device_control,
                                                               size_t num_of_interrupt,
                                                               const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          channel_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    channel_count = 0;
    while ((p_device_control->irq_channels > channel_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = osal_internal_find_interrupt_info(p_device_control,
                                                     num_of_interrupt,
                                                     channel_count,
                                                     p_mutex_attr);

        channel_count++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_init_internal_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_find_interrupt_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_find_interrupt_info(st_osal_device_control_t *p_device_control,
                                                              size_t num_of_interrupt,
                                                              size_t channel_count,
                                                              const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret = OSAL_RETURN_OK;
    e_osal_return_t                     api_ret;
    size_t                              interrupt_count;
    st_osal_interrupt_thread_control_t  *interrupt_control;
    st_osal_interrupt_thread_control_t  *p_local_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    interrupt_count = 0;
    interrupt_control = NULL;
    while ((num_of_interrupt > interrupt_count) &&
           (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)interrupt_count,
                                                     &p_local_control);
        if ((OSAL_RETURN_OK != api_ret) ||
            (NULL == p_local_control) ||
            (NULL == p_local_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Checking the match between the device definition and the resource information */
            if (p_device_control->dev_info->interrupt[channel_count].irq_number ==
                (uint64_t)p_local_control->usr_config->irq_number)
            {
                api_ret = osal_internal_compare_id(p_device_control->dev_info->id,
                                                   p_local_control->usr_config->id,
                                                   (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);

                if ((OSAL_RETURN_OK == api_ret) && (NULL == interrupt_control))
                {
                    interrupt_control = p_local_control;
                    interrupt_control->irq_channel = (uint64_t)channel_count;
                    p_device_control->interrupt_index[channel_count] = (int32_t)interrupt_count;
                    osal_ret = osal_internal_interrupt_initialize(interrupt_control,
                                                                  p_device_control,
                                                                  p_mutex_attr);
                }
                else
                {
                    osal_ret = OSAL_RETURN_CONF;
                }
            }
            else
            {
                /* Do nothing */
                /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
            }   /* end of if((p_device_control->dev_info->interrupt[channel_count].irq_number ==
                             p_local_control->usr_config->irq_number)). */
            /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
        }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == p_local_control) ||
                         (NULL == p_local_control->usr_config)). */
        interrupt_count++;
    }   /* end of while((num_of_interrupt > interrupt_count) && (OSAL_RETURN_OK == osal_ret)). */

    /* Check if the interrupt control information is found. */
    if ((OSAL_RETURN_OK == osal_ret) && (NULL == interrupt_control))
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_find_interrupt_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_compare_id()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_compare_id(const char *p_str1, const char *p_str2, size_t len)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret   = OSAL_RETURN_OK;
    size_t          loop_count = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((loop_count < len) && ('\0' != p_str1[loop_count]) && ('\0' != p_str2[loop_count]) && (OSAL_RETURN_OK == osal_ret))
    {
        if (p_str1[loop_count] != p_str2[loop_count])
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }

    if (loop_count != len)
    {
        if (('\0' != p_str1[loop_count]) || ('\0' != p_str2[loop_count]))
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_CONF;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_compare_id()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_initialize()
* XOS3_OSAL_EMCOS_CD_CD_INT_012
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_001]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_initialize(st_osal_interrupt_thread_control_t *p_info,
                                                               st_osal_device_control_t *p_device_handle,
                                                               const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    pthread_attr_t  thread_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((OSAL_INTERRUPT_PRIORITY_HIGHEST < p_info->usr_config->priority) ||
        (OSAL_INTERRUPT_PRIORITY_LOWEST > p_info->usr_config->priority) ||
        (0U == p_info->usr_config->stack_size) ||
        (0U == p_info->usr_config->interrupt_timeout))
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        osal_ret = osal_internal_get_pdev_int_channel(p_info, p_device_handle);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2736 1 # No problem because osal_internal_interrupt_join is called pthread_mutex_destroy. */
            os_ret = pthread_mutex_init(&p_info->inner_mtx, p_mutex_attr);
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

        if (OSAL_RETURN_OK == osal_ret)
        {
            os_ret = pthread_cond_init(&p_info->cond, NULL);
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

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_interrupt_set_thread_attr(&thread_attr, p_info->usr_config);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            p_info->thread_active = false;
            p_info->notify        = false;
            p_info->device_handle = p_device_handle;
            /* PRQA S 0314 4 # The 4th parameter must be (void *) and there is no side side-effect in this case. */
            os_ret = pthread_create(&p_info->thread_id,
                                    &thread_attr,
                                    osal_internal_user_thread,
                                    (void *)p_info);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
            os_ret = pthread_attr_destroy(&thread_attr);
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
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        if (OSAL_RETURN_OK == osal_ret)
        {
            p_info->handle_id     = (uint64_t)OSAL_INTERRUPT_HANDLE_ID;
            p_info->irq_handler   = NULL;
            p_info->irq_arg       = NULL;
            p_info->irq_enable    = false;
        }
        else
        {
            /* Do nothing */
        }
        /* PRQA S 2053 4 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if((OSAL_INTERRUPT_PRIORITY_HIGHEST < p_info->usr_config->priority)  ||
                     (OSAL_INTERRUPT_PRIORITY_LOWEST > p_info->usr_config->priority) ||
                     (0U == p_info->usr_config->stack_size) ||
                     (0U == p_info->usr_config->interrupt_timeout)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_get_pdev_int_channel()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_pdev_int_channel(st_osal_interrupt_thread_control_t *p_interrupt_control,
                                                               const st_osal_device_control_t *p_device_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    uint32_t            interrupt_count;
    uint32_t            loop_count;
    uint16_t            interrupt_id;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 2855 2 # It is used according to the specifications defined in eMCOS. */
    /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
    os_ret = posix_devctl(p_device_handle->fd, PDEV_INTERRUPT_GET_COUNT, (void *)&interrupt_count, sizeof(uint32_t), NULL);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        for (loop_count = 0U; (interrupt_count > loop_count) && (OSAL_RETURN_OK == osal_ret); loop_count++)
        {
            /* PRQA S 2856,4394 2 # It is used according to the specifications defined in eMCOS. */
            /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
            os_ret = posix_devctl(p_device_handle->fd, PDEV_INTERRUPT_GET_IID(loop_count), (void *)&interrupt_id, sizeof(uint16_t), NULL);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                if (interrupt_id == (p_interrupt_control->usr_config->irq_number + OSAL_INTERRUPT_SPI_OFFSET))
                {
                    p_interrupt_control->pdev_int_channel = loop_count;
                    break;
                }
                else
                {
                    /* Do nothing */
                }
            }
        }

        if ((OSAL_RETURN_OK == osal_ret) && (interrupt_count == loop_count))
        {
            /* The target interrupt ID is not registered */
            osal_ret = OSAL_RETURN_CONF;
        }
    }   /* end of if(0 != os_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_get_pdev_int_channel()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_set_thread_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_set_thread_attr(pthread_attr_t *p_thread_attr,
                                                                    const struct st_osal_interrupt_thread_config *p_usr_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    struct sched_param  int_thread_priority;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_attr_init(p_thread_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_attr_setinheritsched(p_thread_attr, (int)PTHREAD_EXPLICIT_SCHED);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            os_ret = pthread_attr_setschedpolicy(p_thread_attr, (int)SCHED_FIFO);
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

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            int_thread_priority.sched_priority = (int)((int32_t)p_usr_config->priority + OSAL_INTERRUPT_PRIO_BASE - 1);
            os_ret = pthread_attr_setschedparam(p_thread_attr, &int_thread_priority);
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

        if (OSAL_RETURN_OK == osal_ret)
        {
            os_ret = pthread_attr_setstacksize(p_thread_attr, (size_t)(p_usr_config->stack_size + (size_t)PTHREAD_STACK_MIN));
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
        if (OSAL_RETURN_OK != osal_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_attr_destroy(p_thread_attr);
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(0 != os_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_set_thread_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_create_uring_and_thread()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_create_uring_and_thread(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret    = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    size_t              pdev_count;
    uint32_t            interrupt_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (pdev_count = 0; (g_pdev_entry.num > pdev_count) && (OSAL_RETURN_OK == osal_ret); pdev_count++)
    {
        if (-1 != g_pdev_entry.entry[pdev_count].fd)
        {
            /* PRQA S 2855 2 # It is used according to the specifications defined in eMCOS. */
            /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
            os_ret = posix_devctl(g_pdev_entry.entry[pdev_count].fd, PDEV_INTERRUPT_GET_COUNT, (void *)&interrupt_count, sizeof(uint32_t), NULL);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (0U != interrupt_count))
            {
                osal_ret = osal_internal_set_int_ctl_to_pdev(&g_pdev_entry.entry[pdev_count], interrupt_count);
            }
            else
            {
                g_pdev_entry.entry[pdev_count].uring = NULL;
            }

            if ((OSAL_RETURN_OK == osal_ret) && (0U != interrupt_count))
            {
                /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
                os_ret = uring_interrupt_create(&g_pdev_entry.entry[pdev_count].uring, (int)interrupt_count);
                if (0 != os_ret)
                {
                    g_pdev_entry.entry[pdev_count].uring = NULL;
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    osal_ret = osal_internal_create_rev_thread(&g_pdev_entry.entry[pdev_count]);
                }
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            g_pdev_entry.entry[pdev_count].uring = NULL;
        }   /* end of if(-1 != g_pdev_entry.entry[pdev_count].fd). */
    }   /* end of for(pdev_count = 0; (g_pdev_entry.num > pdev_count) && (OSAL_RETURN_OK == osal_ret); pdev_count++). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_create_uring_and_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_set_int_ctl_to_pdev()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_int_ctl_to_pdev(st_osal_pdev_entry_t *p_pdev_entry_info,
                                                              uint32_t interrupt_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    uint32_t            loop_count;
    uint32_t            ctl_count;
    uint16_t            interrupt_id;
    size_t              num_of_interrupt;
    st_osal_interrupt_thread_control_t *interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num_of_interrupt = R_OSAL_RCFG_GetNumOfInterruptThread();
    for (loop_count = 0U; (interrupt_count > loop_count) && (OSAL_RETURN_OK == osal_ret); loop_count++)
    {
        /* PRQA S 2856,4394 2 # It is used according to the specifications defined in eMCOS. */
        /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
        os_ret = posix_devctl(p_pdev_entry_info->fd, PDEV_INTERRUPT_GET_IID(loop_count), (void *)&interrupt_id, sizeof(uint16_t), NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        for (ctl_count = 0U; (num_of_interrupt > ctl_count) && (OSAL_RETURN_OK == osal_ret); ctl_count++)
        {
            osal_ret = R_OSAL_RCFG_GetInterruptThreadInfo(ctl_count,
                                                          &interrupt_control);
            if ((OSAL_RETURN_OK != osal_ret) ||
                (NULL == interrupt_control) ||
                (NULL == interrupt_control->usr_config))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else if ((uint32_t)interrupt_id == (interrupt_control->usr_config->irq_number + OSAL_INTERRUPT_SPI_OFFSET))
            {
                p_pdev_entry_info->control_table[loop_count] = interrupt_control;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of for(loop_count = 0U; (interrupt_count > loop_count) && (OSAL_RETURN_OK == osal_ret); loop_count++). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_set_int_ctl_to_pdev()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_create_rev_thread()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_create_rev_thread(st_osal_pdev_entry_t *p_pdev_entry_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    pthread_attr_t      thread_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_set_rev_thread_attr(&thread_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 4 # The 4th parameter must be (void *) and there is no side side-effect in this case. */
        os_ret = pthread_create(&p_pdev_entry_info->thread_id,
                                &thread_attr,
                                osal_internal_receive_thread,
                                (void *)p_pdev_entry_info);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_attr_destroy(&thread_attr);
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
* End of function osal_internal_create_rev_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_set_rev_thread_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_rev_thread_attr(pthread_attr_t *p_thread_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    struct sched_param  int_thread_priority;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_attr_init(p_thread_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_attr_setinheritsched(p_thread_attr, (int)PTHREAD_EXPLICIT_SCHED);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_attr_setschedpolicy(p_thread_attr, (int)SCHED_FIFO);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        int_thread_priority.sched_priority = 64;
        os_ret = pthread_attr_setschedparam(p_thread_attr, &int_thread_priority);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_attr_setstacksize(p_thread_attr,
                                           1024U + (size_t)PTHREAD_STACK_MIN);
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
* End of function osal_internal_set_rev_thread_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_join_user_thread()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_join_user_thread(size_t num_of_interrupt)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret           = OSAL_RETURN_OK;
    e_osal_return_t                     api_ret;
    size_t                              loop_count         = 0;
    st_osal_interrupt_thread_control_t  *interrupt_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((num_of_interrupt > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)loop_count, &interrupt_control);
        if ((OSAL_RETURN_OK != api_ret) ||
            (NULL == interrupt_control) ||
            (NULL == interrupt_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else if (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id)
        {
            /* Do nothing because initialization is not performed. */
        }
        else
        {
            osal_ret = osal_internal_join_user_thread_proc(interrupt_control);
        }
        loop_count++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_join_user_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_join_user_thread_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_join_user_thread_proc(st_osal_interrupt_thread_control_t  *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                 os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_notify_user_th_termination(p_interrupt_control);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2810 1 # Following the specification. */
        os_ret = pthread_join(p_interrupt_control->thread_id, NULL);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_mutex_destroy(&p_interrupt_control->inner_mtx);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_destroy(&p_interrupt_control->cond);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_interrupt_control->handle_id     = OSAL_INVALID_USER_HANDLE_ID;
        p_interrupt_control->device_handle = NULL;
        p_interrupt_control->irq_enable    = false;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_join_user_thread_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_notify_user_th_termination()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_notify_user_th_termination(st_osal_interrupt_thread_control_t  *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret = OSAL_RETURN_OK;
    e_osal_return_t                     api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                 os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = R_OSAL_Internal_TimedlockMutex(&p_interrupt_control->inner_mtx,
                                             p_interrupt_control->inner_timeout);
    if (OSAL_RETURN_OK != api_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_cond_signal(&p_interrupt_control->cond);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            p_interrupt_control->notify = true;
        }

        os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_notify_user_th_termination()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_join_rev_thread()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_join_rev_thread(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret    = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                     os_ret;
    uint32_t                pdev_count;
    uint32_t                ctl_count;
    uint32_t                interrupt_count;
    struct lfs_devgetdone   send = { 0 };

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (pdev_count = 0; (g_pdev_entry.num > pdev_count) && (OSAL_RETURN_OK == osal_ret); pdev_count++)
    {
        if (NULL != g_pdev_entry.entry[pdev_count].uring)
        {
            /* PRQA S 0314 1 # The 2nd parameter must be (void *), and there is no side side-effect in this case. */
            os_ret = uring_interrupt_notify(g_pdev_entry.entry[pdev_count].uring, (void *)&send, sizeof(struct lfs_devgetdone));
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* PRQA S 2810 1 # Following the specification. */
                os_ret = pthread_join(g_pdev_entry.entry[pdev_count].thread_id, NULL);
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

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* PRQA S 2855 2 # It is used according to the specifications defined in eMCOS. */
                /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
                os_ret = posix_devctl(g_pdev_entry.entry[pdev_count].fd, PDEV_INTERRUPT_GET_COUNT, (void *)&interrupt_count, sizeof(uint32_t), NULL);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                }
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                for (ctl_count = 0U; interrupt_count > ctl_count; ctl_count++)
                {
                    g_pdev_entry.entry[pdev_count].control_table[ctl_count] = NULL;
                }
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                (void)uring_destroy(g_pdev_entry.entry[pdev_count].uring);
                g_pdev_entry.entry[pdev_count].uring = NULL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(NULL != g_pdev_entry.entry[pdev_count].uring). */
    }   /* end of for(pdev_count = 0; (g_pdev_entry.num > pdev_count) && (OSAL_RETURN_OK == osal_ret); pdev_count++). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_join_rev_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_wait_user_thread()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_wait_user_thread(st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t local_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == p_interrupt_control->notify)
    {
        os_ret = pthread_cond_wait(&p_interrupt_control->cond, &p_interrupt_control->inner_mtx);
        if (0 != os_ret)
        {
            local_ret = OSAL_RETURN_FAIL;
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
    p_interrupt_control->notify = false;

    return local_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_wait_user_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_call_handler()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_call_handler(const st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t local_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL != p_interrupt_control->irq_handler) &&
        (true == p_interrupt_control->irq_enable))
    {
        /* PRQA S 0311,0314 1 # The parameter must be void* and there is no side side-effect in this case. */
        local_ret = R_OSAL_Internal_SetThreadSpecific((void *)&p_interrupt_control->irq_channel);
        if (OSAL_RETURN_OK != local_ret)
        {
            local_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            p_interrupt_control->irq_handler(p_interrupt_control->device_handle,
                                             p_interrupt_control->irq_channel,
                                             p_interrupt_control->irq_arg);
        }
        /* PRQA S 2995 2 # It is necessary to check again because p_interrupt_control->irq_enable may be rewritten 
           at the destination where irq_handler is called. */
        if ((OSAL_RETURN_OK == local_ret) && (true == p_interrupt_control->irq_enable))
        {
            /* PRQA S 4446 2 # It is used according to the specifications defined in eMCOS. */
            os_ret = posix_devctl(p_interrupt_control->device_handle->fd,
                                  PDEV_INTERRUPT_UNMASK(p_interrupt_control->pdev_int_channel),
                                  NULL, 0, NULL);
            if (0 != os_ret)
            {
                local_ret = OSAL_RETURN_FAIL;
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
    }   /* end of if((NULL != p_interrupt_control->irq_handler) && (true == p_interrupt_control->irq_enable)). */

    return local_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_call_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_set_interrupt_config()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_interrupt_config(const st_osal_pdev_entry_t * p_pdev_entry_info,
                                                               uint32_t interrupt_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     local_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    uint32_t            loop_count;
    pdev_itr_config_t   data;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    data.tid   = 0;
    data.uring = p_pdev_entry_info->uring;

    for (loop_count = 0U; (interrupt_count > loop_count) && (OSAL_RETURN_OK == local_ret); loop_count++)
    {
        /* PRQA S 4446 2 # It is used according to the specifications defined in eMCOS. */
        /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
        os_ret = posix_devctl(p_pdev_entry_info->fd, PDEV_INTERRUPT_SET_CONFIG(loop_count), (void *)&data, sizeof(data), NULL);
        if (0 != os_ret)
        {
            local_ret = OSAL_RETURN_FAIL;
        }
        else
        {
#if !defined(EMCOS_GIC_WA_ENABLE)
            /* PRQA S 4446 1 # It is used according to the specifications defined in eMCOS. */
            os_ret = posix_devctl(p_pdev_entry_info->fd, PDEV_INTERRUPT_MASK(loop_count), NULL, 0, NULL);
            if (0 != os_ret)
            {
                local_ret = OSAL_RETURN_FAIL;
            }
#endif
        }
    }

    return local_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_set_interrupt_config()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_thread_signal()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_thread_signal(const st_osal_pdev_entry_t * p_pdev_entry_info,
                                                        uint32_t interrupt_count,
                                                        uint32_t interrupt_index)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t local_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (interrupt_count > interrupt_index)
    {
        local_ret = R_OSAL_Internal_TimedlockMutex(&p_pdev_entry_info->control_table[interrupt_index]->inner_mtx,
                                                   p_pdev_entry_info->control_table[interrupt_index]->inner_timeout);
        if (OSAL_RETURN_OK == local_ret)
        {
            os_ret = pthread_cond_signal(&p_pdev_entry_info->control_table[interrupt_index]->cond);
            if (0 != os_ret)
            {
                local_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                p_pdev_entry_info->control_table[interrupt_index]->notify = true;
            }

            os_ret = pthread_mutex_unlock(&p_pdev_entry_info->control_table[interrupt_index]->inner_mtx);
            if (0 != os_ret)
            {
                local_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK == local_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(interrupt_count > interrupt_index). */

    return local_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_thread_signal()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_timedlock_mutex()
* XOS3_OSAL_EMCOS_CD_CD_INT_014
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_003]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_timedlock_mutex(st_osal_interrupt_thread_control_t *p_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    const void*     p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_get_value = R_OSAL_Internal_GetThreadSpecific();
    if ((NULL == p_get_value) ||
        /* PRQA S 0316,0311 1 # There is no problem because uint64_t* is a type that represents a pointer type as an unsigned integer.  */
        (p_info->irq_channel != *(uint64_t *)p_get_value))
    {
        api_ret = R_OSAL_Internal_TimedlockMutex(&p_info->inner_mtx,
                                                 p_info->inner_timeout);
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_timedlock_mutex()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_unlock_mutex()
* XOS3_OSAL_EMCOS_CD_CD_INT_015
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_004]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_unlock_mutex(st_osal_interrupt_thread_control_t *p_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    const void*     p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_get_value = R_OSAL_Internal_GetThreadSpecific();
    if ((NULL == p_get_value) ||
        /* PRQA S 0316,0311 1 # There is no problem because uint64_t* is a type that represents a pointer type as an unsigned integer. */
        (p_info->irq_channel != *(uint64_t *)p_get_value))
    {
        os_ret = pthread_mutex_unlock(&p_info->inner_mtx);
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
* End of function osal_internal_unlock_mutex()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_deinit_pre_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_interrupt_deinit_pre_proc(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret           = OSAL_RETURN_OK;
    e_osal_return_t                    api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                os_ret;
    size_t                             num_of_interrupt;
    size_t                             loop_count;
    st_osal_interrupt_thread_control_t *interrupt_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num_of_interrupt = R_OSAL_RCFG_GetNumOfInterruptThread();
    loop_count = 0;
    while ((num_of_interrupt > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)loop_count,
                                                     &interrupt_control);
        if ((OSAL_RETURN_OK != api_ret) ||
            (NULL == interrupt_control) ||
            (NULL == interrupt_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            api_ret = R_OSAL_Internal_TimedlockMutex(&interrupt_control->inner_mtx,
                                                     interrupt_control->inner_timeout);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                if (NULL != interrupt_control->irq_handler)
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }
                else
                {
                    /* Do nothing */
                }

                os_ret = pthread_mutex_unlock(&interrupt_control->inner_mtx);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }   /* end of if(OSAL_RETURN_OK != api_ret). */
            /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
        }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == interrupt_control) ||
                         (NULL == interrupt_control->usr_config)). */
        loop_count++;
    }   /* end of while((num_of_interrupt > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_deinit_pre_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_reg_isr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_interrupt_reg_isr(osal_device_handle_t const handle,
                                                            uint64_t irq_channel,
                                                            p_osal_isr_func_t irqHandlerFunction,
                                                            void* p_irq_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret;
    e_osal_return_t                    api_ret;
    st_osal_interrupt_thread_control_t *interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)handle->interrupt_index[irq_channel], &interrupt_control);
    if ((OSAL_RETURN_OK != api_ret) ||
        /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (NULL == interrupt_control) ||
        /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
        api_ret  = osal_internal_timedlock_mutex(interrupt_control);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Check if the block is released after the de-initialization. */
            if (false == gs_osal_interrupt_is_initialized)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else if ((NULL != interrupt_control->irq_handler) ||
                     (true == interrupt_control->irq_enable))
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                interrupt_control->irq_handler = irqHandlerFunction;
                interrupt_control->irq_arg     = p_irq_arg;
            }

            api_ret = osal_internal_unlock_mutex(interrupt_control);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                interrupt_control->irq_handler = NULL;
                interrupt_control->irq_arg     = NULL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != api_ret). */
        /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == interrupt_control) ||
                     (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_reg_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_unreg_isr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_interrupt_unreg_isr(osal_device_handle_t const handle,
                                                              uint64_t irq_channel,
                                                              p_osal_isr_func_t irqHandlerFunction)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret;
    e_osal_return_t                     api_ret;
    st_osal_interrupt_thread_control_t  *interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)handle->interrupt_index[irq_channel], &interrupt_control);
    if ((OSAL_RETURN_OK != api_ret) ||
        /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (NULL == interrupt_control) || 
        /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
        api_ret  = osal_internal_timedlock_mutex(interrupt_control);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (irqHandlerFunction != interrupt_control->irq_handler)
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else if (true == interrupt_control->irq_enable)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                interrupt_control->irq_handler = NULL;
                interrupt_control->irq_arg     = NULL;
            }

            api_ret = osal_internal_unlock_mutex(interrupt_control);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != api_ret). */
        /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == interrupt_control) ||
                     (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_unreg_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_enable_isr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_interrupt_enable_isr(osal_device_handle_t const handle, uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret;
    e_osal_return_t                    api_ret;
    st_osal_interrupt_thread_control_t *interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)handle->interrupt_index[irq_channel], &interrupt_control);
    if ((OSAL_RETURN_OK != api_ret) ||
        (NULL == interrupt_control) ||
        /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (NULL == interrupt_control->usr_config) ||
        /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        api_ret = osal_internal_timedlock_mutex(interrupt_control);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = osal_internal_interrupt_enable_isr_proc(handle, interrupt_control);

            api_ret = osal_internal_unlock_mutex(interrupt_control);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == interrupt_control) ||
                     (NULL == interrupt_control->usr_config) ||
                     (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_enable_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_enable_isr_proc()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_interrupt_enable_isr_proc(osal_device_handle_t const handle,
                                                                    st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    uint32_t        elapsed_time;
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_interrupt_control->irq_handler)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    elapsed_time = 0;
    while ((true != p_interrupt_control->thread_active) &&
           ((uint32_t)OSAL_INTERRUPT_ACTIVE_TIMEOUT > elapsed_time) &&
           (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_Internal_MsSleep(OSAL_INTERRUPT_MS_SLEEP_TIME);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            elapsed_time += (uint32_t)OSAL_INTERRUPT_MS_SLEEP_TIME;
        }
    }

    if ((OSAL_RETURN_OK == osal_ret) &&
        ((uint32_t)OSAL_INTERRUPT_ACTIVE_TIMEOUT <= elapsed_time))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    /* Checking the irq_enable flag prevents duplicate settings. */
    if ((OSAL_RETURN_OK == osal_ret) && (false == p_interrupt_control->irq_enable))
    {
        p_get_value = R_OSAL_Internal_GetThreadSpecific();
        if ((NULL == p_get_value) ||
            /* PRQA S 0316,0311 2 # There is no problem because uint64_t* is a type
             that represents a pointer type as an unsigned integer. */
            (p_interrupt_control->irq_channel != *(uint64_t *)p_get_value))
        {
            /* PRQA S 4446 2 # It is used according to the specifications defined in eMCOS. */
            os_ret = posix_devctl(handle->fd, PDEV_INTERRUPT_UNMASK(p_interrupt_control->pdev_int_channel), NULL, 0, NULL);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /*  Do nothing */
            }
        }
        else
        {
            /*  Do nothing */
        }
    }
    else
    {
        /*  Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_interrupt_control->irq_enable = true;
    }
    else
    {
        /*  Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_enable_isr_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_disable_isr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_interrupt_disable_isr(osal_device_handle_t const handle, uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret;
    e_osal_return_t                     api_ret;
    st_osal_interrupt_thread_control_t  *interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)handle->interrupt_index[irq_channel],
                                                  &interrupt_control);
    if ((OSAL_RETURN_OK != api_ret) ||
        (NULL == interrupt_control) ||
        /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (NULL == interrupt_control->usr_config) ||
        /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
        True if the correct handle was passed. False detection of the tool. */
        api_ret = osal_internal_timedlock_mutex(interrupt_control);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = osal_internal_interrupt_disable_isr_proc(handle, interrupt_control);

            api_ret = osal_internal_unlock_mutex(interrupt_control);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == interrupt_control) ||
                     (NULL == interrupt_control->usr_config) ||
                     (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_disable_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_disable_isr_proc()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_interrupt_disable_isr_proc(osal_device_handle_t const handle,
                                                                     st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
#if !defined(EMCOS_GIC_WA_ENABLE)
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    const void      *p_get_value;
#else
    (void)handle;
#endif

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_interrupt_control->irq_handler)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    /* Checking the irq_enable flag prevents duplicate settings. */
    if ((OSAL_RETURN_OK == osal_ret) && (true == p_interrupt_control->irq_enable))
    {
#if !defined(EMCOS_GIC_WA_ENABLE)
        p_get_value = R_OSAL_Internal_GetThreadSpecific();
        if ((NULL == p_get_value) ||
            /* PRQA S 0316,0311 1 # There is no problem because uint64_t* is a type that represents a pointer type as an unsigned integer. */
            (p_interrupt_control->irq_channel != *(uint64_t *)p_get_value))
        {
            /* PRQA S 4446 2 # It is used according to the specifications defined in eMCOS. */
            os_ret = posix_devctl(handle->fd, PDEV_INTERRUPT_MASK(p_interrupt_control->pdev_int_channel), NULL, 0, NULL);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /*  Do nothing */
            }
        }
        else
        {
            /*  Do nothing */
        }
#endif  /* end of #if !definedEMCOS_GIC_WA_ENABLE. */
    }
    else
    {
        /* Do nothing */
    }   /* end of if ((OSAL_RETURN_OK == osal_ret) && (true == p_interrupt_control->irq_enable)). */

    if(OSAL_RETURN_OK == osal_ret)
    {
        p_interrupt_control->irq_enable = false;
    }
    else
    {
        /*  Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_disable_isr_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_interrupt_set_isr_priority()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_interrupt_set_isr_priority(osal_device_handle_t const handle,
                                                                     uint64_t irq_channel,
                                                                     e_osal_interrupt_priority_t priority_value)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret;
    e_osal_return_t                    api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                os_ret;
    st_osal_interrupt_thread_control_t *interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)handle->interrupt_index[irq_channel],
                                                  &interrupt_control);
    if ((OSAL_RETURN_OK != api_ret) ||
        /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (NULL == interrupt_control) ||
        /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
        api_ret  = R_OSAL_Internal_TimedlockMutex(&interrupt_control->inner_mtx,
                                                  interrupt_control->inner_timeout);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* minimum scheduling priority is 16 for Interrupt Manager, so the actual priority range is 16 - 30. */
            /* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
            os_ret = pthread_setschedprio(interrupt_control->thread_id,
                                          ((int)priority_value + (int)OSAL_INTERRUPT_PRIO_BASE));
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&interrupt_control->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != api_ret). */
        /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == interrupt_control) ||
                     (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_interrupt_set_isr_priority()
***********************************************************************************************************************/

