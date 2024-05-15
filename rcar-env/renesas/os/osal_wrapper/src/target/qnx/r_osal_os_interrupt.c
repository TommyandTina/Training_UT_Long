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
* File Name :    r_osal_os_interrupt.c
* Version :      1.0.1
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Interrupt Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <pthread.h>

#include "target/qnx/r_osal_os_type.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private data types
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_interrupt_thread_control_t
 * @brief typedef of st_osal_interrupt_thread_control.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_interrupt_thread_control
 * @brief structure to control interrupt thread.
 **********************************************************************************************************************/
typedef struct st_osal_interrupt_thread_control
{
    uint64_t              handle_id;            /*!< Fixed value: OSAL_INTERRUPT_HANDLE_ID */
    bool                  register_signal;      /*!< Registration notification flag */
    bool                  thread_active;        /*!< interrupt thread active flag(true: active interrupt thread is active, false: interrupt thread is inactive) */
    pthread_cond_t        inner_cond;           /*!< Internal condition variable object */
    pthread_mutex_t       inner_mtx;            /*!< Internal mutex object */
    pthread_mutex_t       inner_mtx_for_cond;   /*!< Internal mutex object for waiting for condition variables  */
    osal_milli_sec_t      inner_timeout;        /*!< Waiting time of lock inner_mtx */
    osal_device_handle_t  device_handle;        /*!< Handle of device according to interrupt */
    pthread_t             thread_id;            /*!< Handle of interrupt thread for QNX */
    p_osal_isr_func_t     irq_handler;          /*!< Pointer of Worker function of Interrupt thread */
    void                  *irq_arg;             /*!< Pointer of Worker function argument */
    uint64_t              irq_channel;          /*!< irq number channel */
    bool                  irq_enable;           /*!< interrupt status (true: Enable interrupt, false: Disable interrupt) */
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                   event_id;             /*!< Attach event ID */

    const struct st_osal_interrupt_thread_config *usr_config; /*!< User configuration for message queue */
} st_osal_interrupt_thread_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_interrupt_thread_affinity_addr_t
 * @brief structure to interrupt thread's affinity table address.
 **********************************************************************************************************************/
typedef struct
{
    const st_osal_interrupt_thread_affinity_t *p_affinity_info;
} st_osal_interrupt_thread_affinity_addr_t;

/*======================================================================================================================
 Global variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var g_context_key
 * @brief thread-specific data key
***********************************************************************************************************************/
pthread_key_t g_context_key;

/*******************************************************************************************************************//**
 * @var g_osal_interrupt_enable_timeout
 * @brief Timeout value that waiting for the HW interrupt event setting of the interrupt thread performed 
 *        by the R_OSAL_InterruptEnableIsr()
 * @note The unit of the timeout value is [ms]
***********************************************************************************************************************/
/* PRQA S 1514 2 # Make it a global variable so that it can be referenced from the user application. */
/* PRQA S 3408 1 # Prototype declaration is not done in order to limit the call destination. */
int64_t g_osal_interrupt_enable_timeout = (int64_t)OSAL_INTERRUPT_ACTIVE_TIMEOUT;

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var gs_osal_os_interrupt_init_flag
 * @brief Interrupt Internal thread operating condition variable.
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_os_interrupt_init_flag = false;

/*******************************************************************************************************************//**
 * @var gs_osal_num_of_interrupt_thread_table
 * @brief Number of the Interrupt thread control information table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_num_of_interrupt_thread_table = 0;

/*******************************************************************************************************************//**
 * @var gsp_osal_interrupt_thread_control
 * @brief The value for the user to operate the interrupt without being aware of the interrupt information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_interrupt_thread_control_t *gsp_osal_interrupt_thread_control = NULL;

/***********************************************************************************************************************
 * @var gsp_osal_interrupt_affinity_addr
 * @brief A pointer of interrupt thread's CPU Affinity table.
***********************************************************************************************************************/
OSAL_STATIC st_osal_interrupt_thread_affinity_addr_t *gsp_osal_interrupt_affinity_addr = NULL;

/***********************************************************************************************************************
 * @var gs_osal_thread_affinity_num
 * @brief Number of the thread's CPU Affinity table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_interrupt_affinity_num = 0U;

/*******************************************************************************************************************//**
 * @var gs_osal_holds_timeout_for_interrupt_enable
 * @brief A static value that holds the timeout value for R_OSAL_InterruptEnableIsr()
 * @note The unit of the timeout value is [ms]
***********************************************************************************************************************/
OSAL_STATIC int64_t gs_osal_holds_timeout_for_interrupt_enable = (int64_t)OSAL_INTERRUPT_ACTIVE_TIMEOUT;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Initialize the attribute object for mutex and condition variable.
* @param[in,out]    p_mutex_attr            The pointer of mutex attributes.
* @param[in,out]    p_cond_attr             The pointer of condition variable attributes.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_mtx_cond_attr(pthread_mutexattr_t *p_mutex_attr,
                                                                   pthread_condattr_t *p_cond_attr);

/***********************************************************************************************************************
* @brief            Initialize the interrupt information based on the device information.
* @param[in]        p_mutex_attr            The pointer of mutex attributes.
* @param[in]        p_cond_attr             The pointer of condition variable attributes.
* @param[in]        device_table_num        The number of device table.
* @param[in,out]    p_device_control_table  The pointer to st_osal_device_control_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_proc(const pthread_mutexattr_t *p_mutex_attr,
                                                          const pthread_condattr_t *p_cond_attr,
                                                          size_t device_table_num,
                                                          osal_device_table_t device_control_table);

/***********************************************************************************************************************
* @brief            Initialize configuration table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_config(void);

/***********************************************************************************************************************
* @brief            Sort interrupt thread's CPU Affinity table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_sort_affinity(void);

/***********************************************************************************************************************
* @brief            Find interrupt control information.
* @param[in]        p_device_control  The pointer of device control information.
* @param[in]        num_of_interrupt  Interrupt thread number.
* @param[in]        channel_count     Channel count.
* @param[in]        p_mutex_attr      The pointer of mutex attributes.
* @param[in]        p_cond_attr       The pointer of condition variable attributes.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_find_control_info(st_osal_device_control_t *p_device_control,
                                                                  size_t channel_count,
                                                                  const pthread_mutexattr_t *p_mutex_attr,
                                                                  const pthread_condattr_t *p_cond_attr);

/***********************************************************************************************************************
* @brief            Initialize Interrupt Thread Information.
* @param[in/out]    p_info          Specify internal management data for interrupt thread control.
* @param[in]        p_device_handle Device handle acquired by "R_OSAL_IoOpen".
* @param[in]        p_mutex_attr    The pointer of mutex attribute object.
* @param[in]        p_cond_attr     The pointer of condition variable attributes.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_init(st_osal_interrupt_thread_control_t *p_info,
                                                            st_osal_device_control_t *p_device_control,
                                                            const pthread_mutexattr_t *p_mutex_attr,
                                                            const pthread_condattr_t *p_cond_attr);

/***********************************************************************************************************************
* @brief            Set thread attributes.
* @param[in]        p_thread_attr      The pointer of thread attributes.
* @param[in]        p_usr_config       Specify internal management data for interrupt thread control.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_thread_attr(pthread_attr_t *p_thread_attr,
                                                                const struct st_osal_interrupt_thread_config *p_usr_config);

/***********************************************************************************************************************
* @brief            Deinitialize interrupt thread resource.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_deinit(void);

/***********************************************************************************************************************
* @brief            Internal timed join monotonic.
* @param[in]        p_interrupt_control     The pointer of interrupt thread control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_timedjoin(st_osal_interrupt_thread_control_t *p_interrupt_control);

/***********************************************************************************************************************
* @brief            Notifies the interrupt thread of finish.
* @param[in]        p_interrupt_control     The pointer of interrupt thread control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_notify_finish(st_osal_interrupt_thread_control_t *p_interrupt_control);

/***********************************************************************************************************************
* @brief            Destroy mutex object.
* @param[in]        p_interrupt_control     The pointer of interrupt thread control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_destroy_mutex(st_osal_interrupt_thread_control_t *p_interrupt_control);

/***********************************************************************************************************************
* @brief            Wait for thread active.
* @param[in]        p_interrupt_control     The pointer of interrupt thread control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_wait_for_thread_active(st_osal_interrupt_thread_control_t *p_interrupt_control);

/***********************************************************************************************************************
* @brief            Lock a mutex(timed lock).
* @param[in]        p_info       The pointer of interrupt thread control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_timedlock_mutex(st_osal_interrupt_thread_control_t *p_info);

/***********************************************************************************************************************
* @brief            Common of Lock a mutex(timed lock).
* @param[in,out]    p_mutex      Mutex object.
* @param[in]        timeout      Timeout value.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_timedlock_mutex_cmn(pthread_mutex_t *p_mutex,
                                                                    osal_milli_sec_t timeout);

/***********************************************************************************************************************
* @brief            Unlock a mutex.
* @param[in]        p_info       The pointer of interrupt thread control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_unlock_mutex(st_osal_interrupt_thread_control_t *p_info);

/***********************************************************************************************************************
* @brief            Internal interrupt thread.
* @param[in]        p_arg       The address of Interrupt control information.
* @return           void*
* @retval           NULL
***********************************************************************************************************************/
OSAL_STATIC void *r_osal_os_interrupt_thread(void *p_arg);

/***********************************************************************************************************************
* @brief            Set CPU Affinity.
* @param[in,out]    p_interrupt_control  The pointer of Interrupt control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_cpu_affinity(st_osal_interrupt_thread_control_t *p_interrupt_control);

/***********************************************************************************************************************
* @brief            Internal interrupt thread pre process.
* @param[in,out]    p_interrupt_control  The pointer of Interrupt control information.
* @param[in,out]    p_attach_event       attach event flag.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_pre_proc(st_osal_interrupt_thread_control_t *p_interrupt_control,
                                                                bool *p_attach_event);

/***********************************************************************************************************************
* @brief            Attach an event to an interrupt source.
* @param[in,out]    p_interrupt_control  The pointer of Interrupt control information.
* @param[in,out]    p_attach_event       attach event flag.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_attach_event(st_osal_interrupt_thread_control_t *p_interrupt_control,
                                                             bool *p_attach_event);

/***********************************************************************************************************************
* @brief            Internal interrupt thread process.
* @param[in,out]    p_interrupt_control  The pointer of Interrupt control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_proc(st_osal_interrupt_thread_control_t *p_interrupt_control);

/***********************************************************************************************************************
* @brief            Call user function registered with the interrupt number.
* @param[in,out]    p_interrupt_control  The pointer of Interrupt control information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_call_handler(st_osal_interrupt_thread_control_t *p_interrupt_control);

/***********************************************************************************************************************
* @brief            Compare function for the argument of bsearch().
* @param[in]        p_key           The key.
* @param[in]        p_search_info   The search information.
* @return           int
* @retval           Returns the number value of the comparison result.
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_os_interrupt_search(const void *p_key, 
                                           const void *p_search_info);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptStaticInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptStaticInit(osal_device_table_t device_control_table,
                                              size_t device_table_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t  cond_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_os_interrupt_init_flag = true;

    osal_ret = r_osal_os_interrupt_init_mtx_cond_attr(&mutex_attr, &cond_attr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_key_create(&g_context_key, NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_condattr_destroy(&cond_attr);
            (void)pthread_mutexattr_destroy(&mutex_attr);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_interrupt_init_proc(&mutex_attr, &cond_attr, device_table_num, device_control_table);

        os_ret = pthread_condattr_destroy(&cond_attr);
        if ((OSAL_RETURN_OK == osal_ret) && (0 != os_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptStaticInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptStaticDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptStaticDeinit(osal_device_table_t device_control_table,
                                                size_t device_table_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    size_t                   loop_count        = 0;
    st_osal_device_control_t *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_os_interrupt_init_flag = false;

    if (NULL == device_control_table)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (NULL != gsp_osal_interrupt_thread_control)
        {
            osal_ret = r_osal_os_interrupt_deinit();
            if (OSAL_RETURN_OK == osal_ret)
            {
                /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
                free((void*)gsp_osal_interrupt_thread_control);
                gsp_osal_interrupt_thread_control = NULL;
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

        if (NULL != gsp_osal_interrupt_affinity_addr)
        {
            /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
            /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
            free((void*)gsp_osal_interrupt_affinity_addr);
            gsp_osal_interrupt_affinity_addr = NULL;
            gs_osal_interrupt_affinity_num   = 0U;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            while (device_table_num > loop_count)
            {
                p_device_control = &device_control_table[loop_count];

                if ((0U != p_device_control->dev_info->irq_channels) &&
                    (NULL != p_device_control->interrupt_index))
                {
                    /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                    /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
                    free((void*)p_device_control->interrupt_index);
                    p_device_control->interrupt_index = NULL;
                }
                else
                {
                    /* Do nothing */
                }
                loop_count++;
            }   /* end of while(device_table_num > loop_count). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */

        if (OSAL_RETURN_OK == osal_ret)
        {
            os_ret = pthread_key_delete(g_context_key);
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

    }   /* end of if (NULL == device_control_table). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptStaticDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret   = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                os_ret;
    size_t                             loop_count = 0U;
    st_osal_interrupt_thread_control_t *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((gs_osal_num_of_interrupt_thread_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_interrupt_control = &gsp_osal_interrupt_thread_control[loop_count];

        osal_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx,
                                                           p_interrupt_control->inner_timeout);
        if (OSAL_RETURN_OK == osal_ret)
        {
            if (NULL != p_interrupt_control->irq_handler)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx);
            if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
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
        }/* end of if(OSAL_RETURN_OK != osal_ret). */
        loop_count++;
    }   /* end of while((num_of_interrupt > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptRegisterIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptRegisterIsr(osal_device_handle_t handle,
                                               uint64_t irq_channel,
                                               e_osal_interrupt_priority_t priority_value,
                                               p_osal_isr_func_t irqHandlerFunction,
                                               void* p_irq_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret;
    e_osal_return_t                    api_ret;
    st_osal_interrupt_thread_control_t *p_interrupt_control;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)priority_value;   /* unused */
    p_interrupt_control = &gsp_osal_interrupt_thread_control[handle->interrupt_index[irq_channel]];

    osal_ret = r_osal_os_interrupt_timedlock_mutex(p_interrupt_control);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_os_interrupt_init_flag)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else if ((NULL != p_interrupt_control->irq_handler) ||
                 (true == p_interrupt_control->irq_enable))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            p_interrupt_control->irq_handler = irqHandlerFunction;
            p_interrupt_control->irq_arg     = p_irq_arg;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx_for_cond,
                                                               p_interrupt_control->inner_timeout);
            if (OSAL_RETURN_OK == osal_ret)
            {
                os_ret = pthread_cond_signal(&p_interrupt_control->inner_cond);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    p_interrupt_control->register_signal = true;
                }

                os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx_for_cond);
                if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
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
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */

        api_ret = r_osal_os_interrupt_unlock_mutex(p_interrupt_control);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
            p_interrupt_control->irq_handler = NULL;
            p_interrupt_control->irq_arg     = NULL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptRegisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptUnregisterIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptUnregisterIsr(osal_device_handle_t handle,
                                                 uint64_t irq_channel,
                                                 p_osal_isr_func_t irqHandlerFunction)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret;
    e_osal_return_t                    api_ret;
    st_osal_interrupt_thread_control_t *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_interrupt_control = &gsp_osal_interrupt_thread_control[handle->interrupt_index[irq_channel]];

    osal_ret  = r_osal_os_interrupt_timedlock_mutex(p_interrupt_control);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_os_interrupt_init_flag)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else if (irqHandlerFunction != p_interrupt_control->irq_handler)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else if (true == p_interrupt_control->irq_enable)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            p_interrupt_control->irq_handler = NULL;
            p_interrupt_control->irq_arg     = NULL;
        }

        api_ret = r_osal_os_interrupt_unlock_mutex(p_interrupt_control);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function R_OSAL_OS_InterruptUnregisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptEnableIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptEnableIsr(osal_device_handle_t handle,
                                             uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret;
    e_osal_return_t                     api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                 os_ret;
    uint64_t                            mask_value;
    const void                          *p_get_value;
    st_osal_interrupt_thread_control_t  *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_interrupt_control = &gsp_osal_interrupt_thread_control[handle->interrupt_index[irq_channel]];

    osal_ret = r_osal_os_interrupt_timedlock_mutex(p_interrupt_control);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_os_interrupt_init_flag)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else if (NULL == p_interrupt_control->irq_handler)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = r_osal_os_interrupt_wait_for_thread_active(p_interrupt_control);
        }

        /* Checking the irq_enable flag prevents duplicate settings. */
        if ((OSAL_RETURN_OK == osal_ret) && (false == p_interrupt_control->irq_enable))
        {
            p_get_value = pthread_getspecific(g_context_key);
            if ((NULL == p_get_value) ||
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                (p_interrupt_control->irq_channel != *(const uint64_t *)p_get_value))
            {
                mask_value = p_interrupt_control->usr_config->irq_number + (uint64_t)OSAL_INTERRUPT_SPI_OFFSET;
                /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
                os_ret     = InterruptUnmask((int)mask_value, p_interrupt_control->event_id);
                if (-1 == os_ret)
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

        api_ret = r_osal_os_interrupt_unlock_mutex(p_interrupt_control);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if (OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptEnableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptDisableIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptDisableIsr(osal_device_handle_t handle,
                                              uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret;
    e_osal_return_t                     api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                 os_ret;
    uint64_t                            mask_value;
    const void                          *p_get_value;
    st_osal_interrupt_thread_control_t  *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_interrupt_control = &gsp_osal_interrupt_thread_control[handle->interrupt_index[irq_channel]];

    osal_ret = r_osal_os_interrupt_timedlock_mutex(p_interrupt_control);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_os_interrupt_init_flag)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else if (NULL == p_interrupt_control->irq_handler)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = OSAL_RETURN_OK;
        }

        /* Checking the irq_enable flag prevents duplicate settings. */
        if ((OSAL_RETURN_OK == osal_ret) && (true == p_interrupt_control->irq_enable))
        {
            p_get_value = pthread_getspecific(g_context_key);
            if ((NULL == p_get_value) ||
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                (p_interrupt_control->irq_channel != *(const uint64_t *)p_get_value))
            {
                mask_value = p_interrupt_control->usr_config->irq_number + (uint64_t)OSAL_INTERRUPT_SPI_OFFSET;
                /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
                os_ret     = InterruptMask((int)mask_value, p_interrupt_control->event_id);
                if (-1 == os_ret)
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

        api_ret = r_osal_os_interrupt_unlock_mutex(p_interrupt_control);
        if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if (OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptDisableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptSetIsrPriority()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptSetIsrPriority(osal_device_handle_t handle,
                                                  uint64_t irq_channel,
                                                  e_osal_interrupt_priority_t priority_value)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret;
    st_osal_interrupt_thread_control_t *p_interrupt_control;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_interrupt_control = &gsp_osal_interrupt_thread_control[handle->interrupt_index[irq_channel]];

    osal_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx,
                                                       p_interrupt_control->inner_timeout);
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_os_interrupt_init_flag)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            /* minimum scheduling priority is 16 for Interrupt Manager, so the actual priority range is 16 - 30. */
            /* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
            os_ret = pthread_setschedprio(p_interrupt_control->thread_id,
                                          ((int)priority_value + (int)OSAL_INTERRUPT_PRIO_BASE));
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (false == gs_osal_os_interrupt_init_flag). */

        os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx);
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
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptSetIsrPriority()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptIsIsrHandler()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptIsIsrHandler(uint32_t num,
                                                bool* p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret;
    bool                                is_isr_handler_ret   = false;
    st_osal_interrupt_thread_control_t  *p_interrupt_control;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                 os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_result)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        p_interrupt_control = &gsp_osal_interrupt_thread_control[num];

        osal_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx,
                                                           p_interrupt_control->inner_timeout);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check if the block is released after the de-initialization. */
            if (false == gs_osal_os_interrupt_init_flag)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else
            {
                if (NULL != p_interrupt_control->irq_handler)
                {
                    is_isr_handler_ret = true;
                }
                else
                {
                    is_isr_handler_ret = false;
                }
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
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }   /* end of if(OSAL_RETURN_OK != api_ret). */

        *p_result = is_isr_handler_ret;
    }   /* end of if (NULL == p_result). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptIsIsrHandler()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_025]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_init_mtx_cond_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_InterruptStaticInit() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_mtx_cond_attr(pthread_mutexattr_t *p_mutex_attr,
                                                                   pthread_condattr_t *p_cond_attr)
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
        os_ret = pthread_mutexattr_settype(p_mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_condattr_init(p_cond_attr);
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_mutexattr_destroy(p_mutex_attr);
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            os_ret = pthread_condattr_setclock(p_cond_attr, CLOCK_MONOTONIC);
            if (0 != os_ret)
            {
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)pthread_condattr_destroy(p_cond_attr);
                (void)pthread_mutexattr_destroy(p_mutex_attr);
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (0 != os_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_init_mtx_cond_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_init_proc()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_InterruptStaticInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_proc(const pthread_mutexattr_t *p_mutex_attr,
                                                          const pthread_condattr_t *p_cond_attr,
                                                          size_t device_table_num,
                                                          osal_device_table_t device_control_table)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret           = OSAL_RETURN_OK;
    size_t                   loop_count;
    size_t                   device_count       = 0;
    size_t                   channel_count;
    st_osal_device_control_t *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_num_of_interrupt_thread_table = 0;
    while (OSAL_INTERRUPT_INVALID_ID != g_osal_interrupt_thread_config[gs_osal_num_of_interrupt_thread_table].id)
    {
        gs_osal_num_of_interrupt_thread_table++;
    }

    if (0U != gs_osal_num_of_interrupt_thread_table)
    {
        osal_ret = r_osal_os_interrupt_init_config();
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
            gsp_osal_interrupt_thread_control = (st_osal_interrupt_thread_control_t *)calloc(1,
                                                (sizeof(st_osal_interrupt_thread_control_t) * gs_osal_num_of_interrupt_thread_table));
            if (NULL == gsp_osal_interrupt_thread_control)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                for (loop_count = 0; gs_osal_num_of_interrupt_thread_table > loop_count; loop_count++)
                {
                    gsp_osal_interrupt_thread_control[loop_count].usr_config    = &g_osal_interrupt_thread_config[loop_count];
                    gsp_osal_interrupt_thread_control[loop_count].inner_timeout = (osal_milli_sec_t)OSAL_INNER_TIMEOUT;
                    gsp_osal_interrupt_thread_control[loop_count].handle_id     = (uint64_t)OSAL_INVALID_INTERRUPT_HANDLE_ID;
                }
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
    }   /* end of if (0U != gs_osal_num_of_interrupt_thread_table). */

    while ((device_table_num > device_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_device_control = &device_control_table[device_count];
        p_device_control->interrupt_index = NULL;

        if (0U != p_device_control->dev_info->irq_channels)
        {
            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            p_device_control->interrupt_index = (int32_t *)calloc(1, (sizeof(int32_t) * p_device_control->dev_info->irq_channels));
            if (NULL == p_device_control->interrupt_index)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                channel_count = 0;
                while ((p_device_control->dev_info->irq_channels > channel_count) && (OSAL_RETURN_OK == osal_ret))
                {
                    osal_ret = r_osal_os_interrupt_find_control_info(p_device_control,
                                                                     channel_count,
                                                                     p_mutex_attr,
                                                                     p_cond_attr);
                    channel_count++;
                }
            }
        }
        else
        {
            /* Do nothing */
        }
        device_count++;
    }   /* end of while((device_table_num > device_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_init_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_028
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_init_config()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_init_proc() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_config(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t         loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (((1 + OSAL_THREAD_PRIORITY_MATRIX_SIZE) > g_osal_interrupt_thread_priority_matrix[0]) ||
        (255 < g_osal_interrupt_thread_priority_matrix[(OSAL_INTERRUPT_PRIORITY_MATRIX_SIZE - 1)]))
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else if (g_osal_thread_priority_matrix[(OSAL_THREAD_PRIORITY_MATRIX_SIZE - 1)] >= g_osal_interrupt_thread_priority_matrix[0])
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        for (loop_count = 1; OSAL_INTERRUPT_PRIORITY_MATRIX_SIZE > loop_count; loop_count++)
        {
            if (g_osal_interrupt_thread_priority_matrix[(loop_count - 1)] >= g_osal_interrupt_thread_priority_matrix[loop_count])
            {
                osal_ret = OSAL_RETURN_CONF;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if ((1 > g_osal_interrupt_thread_priority_matrix[0]). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        gs_osal_interrupt_affinity_num = 0U;
        while ((uint64_t)OSAL_INTERRUPT_AFFINITY_INVALID_ID != g_osal_interrupt_thread_affinity[gs_osal_interrupt_affinity_num].irq_number)
        {
            gs_osal_interrupt_affinity_num++;
        }

        if (0U != gs_osal_interrupt_affinity_num)
        {
            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            gsp_osal_interrupt_affinity_addr = (st_osal_interrupt_thread_affinity_addr_t *)calloc(gs_osal_interrupt_affinity_num,
                                                                                                  sizeof(st_osal_interrupt_thread_affinity_addr_t));
            if (NULL == gsp_osal_interrupt_affinity_addr)
            {
                osal_ret = OSAL_RETURN_MEM;
                gs_osal_interrupt_affinity_num = 0U;
            }
            else
            {
                osal_ret = r_osal_os_interrupt_sort_affinity();
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (0L >= g_osal_interrupt_enable_timeout)
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* The timeout value that can be changed externally is retained in the initialization process. */
            gs_osal_holds_timeout_for_interrupt_enable = g_osal_interrupt_enable_timeout;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_init_config()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_029
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_sort_affinity()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_init_config() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_sort_affinity(void)
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
    gsp_osal_interrupt_affinity_addr[0U].p_affinity_info = &g_osal_interrupt_thread_affinity[0U];
    for (loop_count = 1U; ((loop_count < gs_osal_interrupt_affinity_num) && (OSAL_RETURN_OK == osal_ret)); loop_count++)
    {
        tmp = g_osal_interrupt_thread_affinity[loop_count].irq_number;
        for (cmp_count = loop_count; ((cmp_count > 0U) && (OSAL_RETURN_OK == osal_ret)); cmp_count--)
        {
            if (gsp_osal_interrupt_affinity_addr[(cmp_count - 1U)].p_affinity_info->irq_number > tmp)
            {
                gsp_osal_interrupt_affinity_addr[cmp_count].p_affinity_info = gsp_osal_interrupt_affinity_addr[(cmp_count - 1U)].p_affinity_info;
            }
            else if (gsp_osal_interrupt_affinity_addr[(cmp_count - 1U)].p_affinity_info->irq_number == tmp)
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                break;
            }
        }
        gsp_osal_interrupt_affinity_addr[cmp_count].p_affinity_info = &g_osal_interrupt_thread_affinity[loop_count];
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_sort_affinity()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_find_control_info()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_init_proc() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_find_control_info(st_osal_device_control_t *p_device_control,
                                                                  size_t channel_count,
                                                                  const pthread_mutexattr_t *p_mutex_attr,
                                                                  const pthread_condattr_t *p_cond_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret             = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                os_ret;
    size_t                             interrupt_count;
    st_osal_interrupt_thread_control_t *p_interrupt_control;
    st_osal_interrupt_thread_control_t *p_local_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    interrupt_count     = 0;
    p_interrupt_control = NULL;
    while ((gs_osal_num_of_interrupt_thread_table > interrupt_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_local_control = &gsp_osal_interrupt_thread_control[interrupt_count];

        if (p_device_control->dev_info->interrupt[channel_count].irq_number == p_local_control->usr_config->irq_number)
        {
            os_ret = strncmp(p_device_control->dev_info->id,
                             p_local_control->usr_config->id,
                             (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);

            if ((0 == os_ret) && (NULL == p_interrupt_control))
            {
                p_interrupt_control = p_local_control;
                p_interrupt_control->irq_channel = (uint64_t)channel_count;
                p_device_control->interrupt_index[channel_count] = (int32_t)interrupt_count;
                osal_ret = r_osal_os_interrupt_thread_init(p_interrupt_control,
                                                           p_device_control,
                                                           p_mutex_attr,
                                                           p_cond_attr);
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
        }   /* end of if(p_device_control->dev_info->interrupt[channel_count].irq_number ==
                         p_local_control->usr_config->irq_number). */
        interrupt_count++;
    }   /* end of while((gs_osal_num_of_interrupt_thread_table > interrupt_count) && (OSAL_RETURN_OK == osal_ret)). */

    /* Set if only the OSAL_INTERRUPT_INVALID_ID or undefined in the interrupt thread config */
    if ((OSAL_RETURN_OK == osal_ret) && (NULL == p_interrupt_control))
    {
        p_device_control->interrupt_index[channel_count] = OSAL_DEV_CFG_INTERRUPT_NONE_INDEX;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_find_control_info()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_thread_init()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_init(st_osal_interrupt_thread_control_t *p_info,
                                                            st_osal_device_control_t *p_device_control,
                                                            const pthread_mutexattr_t *p_mutex_attr,
                                                            const pthread_condattr_t *p_cond_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
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
        /* PRQA S 2736 1 # There is no problem because it will be released at R_OSAL_OS_InterruptStaticDeinit(). */
        os_ret = pthread_mutex_init(&p_info->inner_mtx, p_mutex_attr);
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
            /* PRQA S 2736 1 # There is no problem because it will be released at R_OSAL_OS_InterruptStaticDeinit(). */
            os_ret = pthread_mutex_init(&p_info->inner_mtx_for_cond, p_mutex_attr);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)pthread_mutex_destroy(&p_info->inner_mtx);
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
            os_ret = pthread_cond_init(&p_info->inner_cond, p_cond_attr);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)pthread_mutex_destroy(&p_info->inner_mtx_for_cond);
                (void)pthread_mutex_destroy(&p_info->inner_mtx);
            }
            else
            {
                /* Do nothing */
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_interrupt_set_thread_attr(&thread_attr, p_info->usr_config);
            if (OSAL_RETURN_OK == osal_ret)
            {
                p_info->register_signal = false;
                p_info->thread_active   = false;
                p_info->event_id        = -1;

                /* PRQA S 0314 4 # No problem because it follows the specification of the calling function. */
                os_ret = pthread_create(&p_info->thread_id,
                                        &thread_attr,
                                        r_osal_os_interrupt_thread,
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
                p_info->device_handle = p_device_control;
                p_info->irq_handler   = NULL;
                p_info->irq_arg       = NULL;
                p_info->irq_enable    = false;
            }
            else
            {
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)pthread_cond_destroy(&p_info->inner_cond);
                (void)pthread_mutex_destroy(&p_info->inner_mtx_for_cond);
                (void)pthread_mutex_destroy(&p_info->inner_mtx);
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if((OSAL_INTERRUPT_PRIORITY_HIGHEST < p_info->usr_config->priority)  ||
                     (OSAL_INTERRUPT_PRIORITY_LOWEST > p_info->usr_config->priority) ||
                     (0U == p_info->usr_config->interrupt_timeout)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_thread_init()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_017
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_set_thread_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_thread_init() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_thread_attr(pthread_attr_t *p_thread_attr,
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
        os_ret = pthread_attr_setinheritsched(p_thread_attr, PTHREAD_EXPLICIT_SCHED);
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
            os_ret = pthread_attr_setschedpolicy(p_thread_attr, SCHED_FIFO);
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
            int_thread_priority.sched_priority = (int)g_osal_interrupt_thread_priority_matrix[p_usr_config->priority];
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
    }   /* end of if(0 != os_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_set_thread_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_018
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_deinit()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_InterruptStaticDeinit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_deinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret             = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                 os_ret;
    size_t                              loop_count           = 0;
    st_osal_interrupt_thread_control_t  *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((gs_osal_num_of_interrupt_thread_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_interrupt_control = &gsp_osal_interrupt_thread_control[loop_count];

        if ((uint64_t)OSAL_INTERRUPT_HANDLE_ID == p_interrupt_control->handle_id)
        {
            osal_ret = r_osal_os_interrupt_timedjoin(p_interrupt_control);
            if (OSAL_RETURN_OK == osal_ret)
            {
                os_ret = pthread_cond_destroy(&p_interrupt_control->inner_cond);
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
                osal_ret = r_osal_os_interrupt_destroy_mutex(p_interrupt_control);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    p_interrupt_control->handle_id     = (uint64_t)OSAL_INVALID_INTERRUPT_HANDLE_ID;
                    p_interrupt_control->device_handle = NULL;
                    p_interrupt_control->irq_enable    = false;
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
        }   /* end of if ((uint64_t)OSAL_INTERRUPT_HANDLE_ID == p_interrupt_control->handle_id). */

        loop_count++;
    }   /* end of while((gs_osal_num_of_interrupt_thread_table > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_deinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_019
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_timedjoin()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_deinit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_timedjoin(st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;
    struct timespec     time_stamp;
    time_t              tv_sec_tmp;
    uint64_t            tv_nsec_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_os_interrupt_notify_finish(p_interrupt_control);
    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
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
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        tv_sec_tmp  = (((time_t)p_interrupt_control->usr_config->interrupt_timeout + (time_t)OSAL_INTERRUPT_TIMEOUT_OFFSET) / (time_t)OSAL_SECTONSEC);
        tv_nsec_tmp = ((p_interrupt_control->usr_config->interrupt_timeout + OSAL_INTERRUPT_TIMEOUT_OFFSET) % (uint64_t)OSAL_SECTONSEC);

        time_stamp.tv_sec  += tv_sec_tmp;
        /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
        time_stamp.tv_nsec += (long)tv_nsec_tmp;
        if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
        {
            /* round up */
            /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
            time_stamp.tv_nsec -= (long)(OSAL_MAX_TV_NSEC + 1);
            time_stamp.tv_sec  += (time_t)1;
        }
        else
        {
            /* do nothing */
        }

        os_ret = pthread_timedjoin_monotonic(p_interrupt_control->thread_id, NULL, &time_stamp);
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_timedjoin()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_020
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_notify_finish()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_timedjoin() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_notify_finish(st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx_for_cond,
                                                       p_interrupt_control->inner_timeout);
    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_signal(&p_interrupt_control->inner_cond);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            p_interrupt_control->register_signal = true;
        }

        os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx_for_cond);
        if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_os_interrupt_notify_finish()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_021
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_destroy_mutex()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_deinit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_destroy_mutex(st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
    int                 os_mutex_ret;
    int                 os_ret;
    struct timespec     time_stamp;
    int32_t             waiting_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_mutex_ret = pthread_mutex_destroy(&p_interrupt_control->inner_mtx);
    if (0 != os_mutex_ret)
    {
        waiting_time = 0;
        while ((OSAL_INTERRUPT_MUTEX_DESTROY_COUNT > waiting_time) &&
               (EBUSY == os_mutex_ret) &&
               (OSAL_RETURN_OK == osal_ret))
        {
            os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* PRQA S 2985 1 # OSAL_INTERRUPT_MS_SLEEP_TIME may vary depending on your environment. */
                time_stamp.tv_sec  += (time_t)(OSAL_INTERRUPT_MS_SLEEP_TIME / OSAL_SECTOMSEC);
                /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
                time_stamp.tv_nsec += (long)((OSAL_INTERRUPT_MS_SLEEP_TIME % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);
                if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
                {
                    time_stamp.tv_sec++;
                    time_stamp.tv_nsec = time_stamp.tv_nsec - (OSAL_MAX_TV_NSEC + 1);
                }
                else
                {
                    /* Do nothing */
                }

                /* PRQA S 5209,2855 1 # System is requesting an int, so it cannot be changed. */
                os_ret = clock_nanosleep(CLOCK_MONOTONIC, (int)TIMER_ABSTIME, &time_stamp, NULL);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    waiting_time += OSAL_INTERRUPT_MS_SLEEP_TIME;
                }
            }   /* end of if (0 != os_ret) */
            os_mutex_ret = pthread_mutex_destroy(&p_interrupt_control->inner_mtx);
        }   /* end of while ((OSAL_INTERRUPT_MUTEX_DESTROY_COUNT > waiting_time) && (EBUSY == os_mutex_ret) && (OSAL_RETURN_OK == osal_ret)). */

        if ((OSAL_INTERRUPT_MUTEX_DESTROY_COUNT <= waiting_time) || (0 != os_mutex_ret))
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
    }   /* end of if(0 != os_mutex_ret). */

    /* Since it is always unused after inner_mtx has been discarded, there is no need to check if it is in use. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_mutex_destroy(&p_interrupt_control->inner_mtx_for_cond);
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
* End of function r_osal_os_interrupt_destroy_mutex()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_022
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_wait_for_thread_active()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_InterruptEnableIsr() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
/* PRQA S 3673 1 # The caller of this function modifies p_interrupt_control, so const is not needed. */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_wait_for_thread_active(st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int              os_ret;
    struct timespec  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx_for_cond,
                                                       p_interrupt_control->inner_timeout);
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (true != p_interrupt_control->thread_active)
        {
            os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                time_stamp.tv_sec  += ((time_t)gs_osal_holds_timeout_for_interrupt_enable / (time_t)OSAL_SECTOMSEC);
                /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
                time_stamp.tv_nsec += (long)((gs_osal_holds_timeout_for_interrupt_enable % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);
                if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
                {
                    time_stamp.tv_sec++;
                    time_stamp.tv_nsec = time_stamp.tv_nsec - (OSAL_MAX_TV_NSEC + 1);
                }
                else
                {
                    /* Do nothing */
                }

                os_ret = pthread_cond_timedwait(&p_interrupt_control->inner_cond,
                                                &p_interrupt_control->inner_mtx_for_cond,
                                                &time_stamp);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }   /* end of if (0 != os_ret). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if (true != p_interrupt_control->thread_active). */

        os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx_for_cond);
        if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_os_interrupt_wait_for_thread_active()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_timedlock_mutex()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_timedlock_mutex(st_osal_interrupt_thread_control_t *p_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret  = OSAL_RETURN_OK;
    const void       *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_get_value = pthread_getspecific(g_context_key);
    if ((NULL == p_get_value) ||
        /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
        (p_info->irq_channel != *(const uint64_t *)p_get_value))
    {
        osal_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_info->inner_mtx, p_info->inner_timeout);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_timedlock_mutex()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_023
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_timedlock_mutex_cmn()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_timedlock_mutex_cmn(pthread_mutex_t *p_mutex,
                                                                    osal_milli_sec_t timeout)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret  = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int              os_ret;
    struct timespec  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        time_stamp.tv_sec  += ((time_t)timeout / (time_t)OSAL_SECTOMSEC);
        /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
        time_stamp.tv_nsec += (long)((timeout % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);

        if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
        {
            time_stamp.tv_sec++;
            time_stamp.tv_nsec = time_stamp.tv_nsec - (OSAL_MAX_TV_NSEC + 1);
        }
        else
        {
            /* Do nothing */
        }

        if ((time_stamp.tv_nsec % OSAL_MSECTONSEC) != 0L)
        {
            /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
            time_stamp.tv_nsec = (long)(((time_stamp.tv_nsec / OSAL_MSECTONSEC) + 1) * OSAL_MSECTONSEC);
            if (time_stamp.tv_nsec > OSAL_MAX_TV_NSEC)
            {
                time_stamp.tv_sec++;
                time_stamp.tv_nsec = time_stamp.tv_nsec - (OSAL_MAX_TV_NSEC + 1);
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

        os_ret = pthread_mutex_timedlock_monotonic(p_mutex, &time_stamp);
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_timedlock_mutex_cmn()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_unlock_mutex()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_unlock_mutex(st_osal_interrupt_thread_control_t *p_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_get_value = pthread_getspecific(g_context_key);
    if ((NULL == p_get_value) ||
        /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
        (p_info->irq_channel != *(const uint64_t *)p_get_value))
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
* End of function r_osal_os_interrupt_unlock_mutex()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_thread()
***********************************************************************************************************************/
OSAL_STATIC void *r_osal_os_interrupt_thread(void *p_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     local_ret;
    st_osal_interrupt_thread_control_t  *p_interrupt_control;
    bool                                attach_event;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                 os_ret;
    char                                task_name[OSAL_TASK_NAME_MAX_LENGTH + 1UL];   /* task name + \0 */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_arg)
    {
        /* PRQA S 0316 1 # There is no problem because it is just restored to the type at the time of function call. */
        p_interrupt_control = (st_osal_interrupt_thread_control_t *)p_arg;

        /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
        (void)memset((void*)task_name, 0x00, sizeof(task_name));

        if (NULL != p_interrupt_control->usr_config->p_task_name)
        {
            /* Copy up to 15 characters */
            (void)strncpy(task_name, p_interrupt_control->usr_config->p_task_name, (size_t)OSAL_TASK_NAME_MAX_LENGTH);
        }
        else
        {
            /* Do nothing */
        }

        /* Set to the calling thread. */
        os_ret = pthread_setname_np(0, task_name);
        if (0 != os_ret)
        {
            local_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            local_ret = r_osal_os_interrupt_set_cpu_affinity(p_interrupt_control);
        }

        attach_event = false;
        while ((true == gs_osal_os_interrupt_init_flag) && (OSAL_RETURN_OK == local_ret))
        {
            local_ret = r_osal_os_interrupt_thread_pre_proc(p_interrupt_control, &attach_event);

            /* If the wait is exited by notification from Deinitialize, the subsequent processing is not performed. */
            if ((true == gs_osal_os_interrupt_init_flag) && (OSAL_RETURN_OK == local_ret))
            {
                local_ret = r_osal_os_interrupt_thread_proc(p_interrupt_control);
            }
            else
            {
                /* Do nothing */
            }
        }

        p_interrupt_control->thread_active = false;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != p_arg). */

    return NULL;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_thread()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_030
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_set_cpu_affinity()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_thread() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
/* PRQA S 3673 1 # The caller of this function modifies p_interrupt_control, so const is not needed. */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_set_cpu_affinity(st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                                 local_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                             os_ret;
    const st_osal_interrupt_thread_affinity_addr_t  *p_affinity_addr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0U != gs_osal_interrupt_affinity_num)
    {
        /* PRQA S 5135 3 # The compare function does not modify array and return consistent comparison results. */
        /* PRQA S 0314 3 # No problem because it follows the specification of the calling function. */
        /* PRQA S 0316 1 # No problem because it is returned to the type of the second argument. */
        p_affinity_addr = (const st_osal_interrupt_thread_affinity_addr_t *)bsearch((const void *)&p_interrupt_control->usr_config->irq_number,
                                                                                    (const void *)gsp_osal_interrupt_affinity_addr,
                                                                                    gs_osal_interrupt_affinity_num,
                                                                                    sizeof(st_osal_interrupt_thread_affinity_addr_t),
                                                                                    r_osal_os_interrupt_search);

        if (NULL != p_affinity_addr)
        {
            if (0x0U != p_affinity_addr->p_affinity_info->affinity)
            {
                /* PRQA S 0326 1 # No problem because it follows the specification of the calling function. */
                os_ret = ThreadCtl(_NTO_TCTL_RUNMASK, (void *)p_affinity_addr->p_affinity_info->affinity);
                if (-1 == os_ret)
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
        }   /* end of if (NULL != p_affinity_addr). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (0U != gs_osal_interrupt_affinity_num). */

    return local_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_set_cpu_affinity()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_024
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_thread_pre_proc()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_thread() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_pre_proc(st_osal_interrupt_thread_control_t *p_interrupt_control,
                                                                bool *p_attach_event)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t local_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    local_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx_for_cond,
                                                        p_interrupt_control->inner_timeout);
    if (OSAL_RETURN_OK == local_ret)
    {
        if (false == p_interrupt_control->register_signal)
        {
            os_ret = pthread_cond_wait(&p_interrupt_control->inner_cond, &p_interrupt_control->inner_mtx_for_cond);
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

        p_interrupt_control->register_signal = false;

        os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx_for_cond);
        if (0 != os_ret)
        {
            local_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        /* If the wait is exited by notification from Deinitialize, the subsequent processing is not performed. */
        if ((true == gs_osal_os_interrupt_init_flag) &&
            /* Call InterruptAttachEvent only once. */
            (false == *p_attach_event) &&
            (OSAL_RETURN_OK == local_ret))
        {
            local_ret = r_osal_os_interrupt_attach_event(p_interrupt_control, p_attach_event);
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == local_ret). */

    return local_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_thread_pre_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_025
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_attach_event()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_thread_pre_proc() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_attach_event(st_osal_interrupt_thread_control_t *p_interrupt_control,
                                                             bool *p_attach_event)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t local_ret = OSAL_RETURN_OK;
    struct sigevent event;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    uint64_t        mask_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    event.sigev_notify = SIGEV_INTR;

    mask_value = p_interrupt_control->usr_config->irq_number + (uint64_t)OSAL_INTERRUPT_SPI_OFFSET;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    p_interrupt_control->event_id = InterruptAttachEvent((int)mask_value,
                                                         &event,
                                                         _NTO_INTR_FLAGS_TRK_MSK);
    if (-1 == p_interrupt_control->event_id)
    {
        local_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = InterruptMask((int)mask_value,
                               p_interrupt_control->event_id);
        if (-1 == os_ret)
        {
            local_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            *p_attach_event = true;
        }
    }   /* end of if (-1 == p_interrupt_control->event_id). */

    if (OSAL_RETURN_OK == local_ret)
    {
        local_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx_for_cond,
                                                            p_interrupt_control->inner_timeout);
        if (OSAL_RETURN_OK == local_ret)
        {
            os_ret = pthread_cond_signal(&p_interrupt_control->inner_cond);
            if (0 != os_ret)
            {
                local_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                p_interrupt_control->thread_active = true;
            }

            os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx_for_cond);
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
        }   /* end of if (OSAL_RETURN_OK == local_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == local_ret). */

    return local_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_attach_event()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_026
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_thread_proc()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_thread() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_thread_proc(st_osal_interrupt_thread_control_t *p_interrupt_control)
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
    while ((NULL != p_interrupt_control->irq_handler) &&
           (OSAL_RETURN_OK == local_ret))
    {
        /* PRQA S 4543,0499,4524 2 # _NTO_TIMEOUT_INTR is used according to the QNX specification. */
        os_ret = TimerTimeout(CLOCK_MONOTONIC,
                              _NTO_TIMEOUT_INTR,
                              NULL,
                              &p_interrupt_control->usr_config->interrupt_timeout,
                              NULL);
        if (0 != os_ret)
        {
            local_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            os_ret = InterruptWait_r(0, NULL);
            if (EOK == os_ret)
            {
                local_ret = r_osal_os_interrupt_call_handler(p_interrupt_control);
            }
            else
            {
                if (ETIMEDOUT != os_ret)
                {
                    local_ret = OSAL_RETURN_FAIL;
                }
                else  /* ETIMEDOUT == os_ret */
                {
                    /* Do nothing */
                }
            }
        }   /* end of if(0 != os_ret). */
    }   /* end of while((NULL != p_interrupt_control->irq_handler) && (OSAL_RETURN_OK == local_ret)). */

    return local_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_thread_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_027
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_call_handler()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_interrupt_thread_proc() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_call_handler(st_osal_interrupt_thread_control_t *p_interrupt_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t local_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    uint64_t        mask_value = p_interrupt_control->usr_config->irq_number + (uint64_t)OSAL_INTERRUPT_SPI_OFFSET;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    local_ret = r_osal_os_interrupt_timedlock_mutex_cmn(&p_interrupt_control->inner_mtx,
                                                        p_interrupt_control->inner_timeout);
    if (OSAL_RETURN_OK == local_ret)
    {
        if ((NULL != p_interrupt_control->irq_handler) &&
            (true == p_interrupt_control->irq_enable))
        {
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            os_ret = pthread_setspecific(g_context_key, (const void *)&p_interrupt_control->irq_channel);
            if (0 != os_ret)
            {
                local_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                p_interrupt_control->irq_handler(p_interrupt_control->device_handle,
                                                 p_interrupt_control->irq_channel,
                                                 p_interrupt_control->irq_arg);
            }

            /* PRQA S 2995 1 # No problem as it can be changed in irq_handler. */
            if ((OSAL_RETURN_OK == local_ret) && (true == p_interrupt_control->irq_enable))
            {
                /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
                os_ret = InterruptUnmask((int)mask_value,
                                         p_interrupt_control->event_id);
                if (-1 == os_ret)
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
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            os_ret = InterruptUnmask((int)mask_value,
                                     p_interrupt_control->event_id);
            if (-1 == os_ret)
            {
                local_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if((NULL != p_interrupt_control->irq_handler) && (true == p_interrupt_control->irq_enable)). */

        os_ret = pthread_mutex_unlock(&p_interrupt_control->inner_mtx);
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

    return local_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_call_handler()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_031
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_search()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_os_interrupt_search(const void *p_key, 
                                           const void *p_search_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 2 # There is no problem because it is just restored to the type at the time of function call. */
    uint64_t                                       key              = *(const uint64_t *)p_key;
    const st_osal_interrupt_thread_affinity_addr_t *p_affinity_addr = (const st_osal_interrupt_thread_affinity_addr_t *)p_search_info;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                  result;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (key < p_affinity_addr->p_affinity_info->irq_number)
    {
        result = -1;
    }
    else if (key > p_affinity_addr->p_affinity_info->irq_number)
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
* End of function r_osal_os_interrupt_search()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_029]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
