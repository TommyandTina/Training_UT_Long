/***********************************************************************************************************************
* Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_os_thread.c
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Thread Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <zephyr/posix/pthread.h>
#include <zephyr/kernel.h>

#include "target/zephyr/r_osal_os_type.h"
#include "rcar-xos/osal_configuration/target/zephyr/r_osal_configuration.h"
#include "target/common/res_cfg/static/r_osal_res_cfg.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private macro definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @def OSAL_WRAPPER_VERSION_MAJOR
 * @brief Major version.
***********************************************************************************************************************/
#define OSAL_PMA_TASK_NAME "pma_mng_task"

/*******************************************************************************************************************//**
 * @def OSAL_PMA_STACK_SIZE
 * @brief Thread PMA Starck Size.
***********************************************************************************************************************/
#define R_PMA_MNG_TASK_STACK_SIZE  (4096U)

/*******************************************************************************************************************//**
 * @def OSAL_THREAD_STACK_ADDR
 * @brief Thread Starck address.
***********************************************************************************************************************/
/* PRQA S 1514 2 # Definition within a function reduces readability and maintainability, so no change. */
/* PRQA S 1840, 3408 1 # Defined in Zephyr OS and used according to the specification. */
K_THREAD_STACK_DEFINE(addr_pma,  R_PMA_MNG_TASK_STACK_SIZE);

/*======================================================================================================================
 Private data types
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_thread_self_control_t
 * @brief structure to self thread control.
 **********************************************************************************************************************/
typedef struct
{
    st_osal_os_thread_t self_thread;
    void                *p_arg;
}st_osal_thread_self_control_t;

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * @var gsp_osal_thread_self_control
 * @brief self thread id control information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_thread_self_control_t *gsp_osal_thread_self_control = NULL;

/***********************************************************************************************************************
 * @var gs_osal_thread_self_control_num
 * @brief Number of the thread self control information.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_thread_self_control_num = 0;

/***********************************************************************************************************************
 * @var gs_osal_thread_self_register_num
 * @brief Number of the thread self control information.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_thread_self_register_num = 0;

/***********************************************************************************************************************
 * @var gs_osal_thread_stack_config_num
 * @brief Number of the thread stack config.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_thread_stack_config_num = 0U;

/***********************************************************************************************************************
 * @var gs_osal_thread_affinity_num
 * @brief Number of the CPU Affinity config.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_thread_affinity_num = 0U;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Initialize attribute of Thread.
* @param[in]        priority        The priority.
* @param[in,out]    p_local_attr    The pointer to st_osal_os_threadattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_thread_init_attr(e_osal_thread_priority_t priority,
                                                       st_osal_os_threadattr_t* p_local_attr);

/***********************************************************************************************************************
* @brief            Thread join to OS independent part.
                    Execute the thread join with timeout in OSAL wrapper for Zephyr OS.
                    This function is created to based on the pthread_join.
* @param[in]        thread          The target thread whose termination you're waiting for.
* @param[in]        abstime         A pointer to a timespec structure that specifies the time
                                    at which the time-out is to expire.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_thread_timedjoin(pthread_t thread, const struct timespec* abstime);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadStaticInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadStaticInit(size_t thread_cfg_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_thread_stack_config_num = 0U;
    gs_osal_thread_affinity_num = 0U;

    /* Count up the number of valid items in the thread stack config table. */
    while (OSAL_THREAD_STACK_INVALID_ID != g_osal_thread_stack_config[gs_osal_thread_stack_config_num].id)
    {
        gs_osal_thread_stack_config_num++;
    }

    /* Count up the number of valid items in the thread affinity setting table. */
    while (OSAL_THREAD_AFFINITY_INVALID_ID != g_osal_thread_affinity[gs_osal_thread_affinity_num].id)
    {
        gs_osal_thread_affinity_num++;
    }

    /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
    gsp_osal_thread_self_control = (st_osal_thread_self_control_t *)calloc(1, sizeof(st_osal_thread_self_control_t) * thread_cfg_num);
    if (NULL == gsp_osal_thread_self_control)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        gs_osal_thread_self_control_num = thread_cfg_num;
    }   /* end of if (NULL == gsp_osal_thread_self_control) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadStaticInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadStaticDeinit()
***********************************************************************************************************************/
void R_OSAL_OS_ThreadStaticDeinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_osal_thread_self_control)
    {
        /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
        /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
        free((void*)gsp_osal_thread_self_control);
        gsp_osal_thread_self_control     = NULL;
        gs_osal_thread_self_control_num  = 0;
        gs_osal_thread_self_register_num = 0;
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != gsp_osal_thread_self_control). */

    return;

}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadStaticDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadStaticCreate()
***********************************************************************************************************************/
/* PRQA S 3673 4 # Since the pointer type is typedefed, const cannot be added. */
e_osal_return_t R_OSAL_OS_ThreadStaticCreate(const st_osal_thread_config_t* const p_config,
                                             p_osal_os_thread_func_t func,
                                             void* p_arg,
                                             const osal_os_threadattr_t thread_attr,
                                             osal_os_thread_t * p_thread)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                   osal_ret            = OSAL_RETURN_OK;
    int                               os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    st_osal_os_thread_t               *p_local_thread;
    const st_osal_thread_control_t    *p_control;
    /* PRQA S 2812 1 # p_config is guaranteed not to be NULL in this case. */
    size_t                            local_stack_size    = p_config->stack_size;
    size_t                            stack_loop_count    = 0U;
    size_t                            affinity_loop_count = 0U;
    void                              *local_addr_stack   = NULL;
    uint8_t                           local_affinity      = 0;
    uint8_t                           result_bit;
    uint32_t                          bit_loop;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == p_config) || (NULL == func) || (NULL == p_arg) || (NULL == thread_attr) || (NULL == p_thread))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = strcmp(p_config->task_name, OSAL_PMA_TASK_NAME);
        if(0 == os_ret)
        {
            /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
            local_addr_stack = (void *)addr_pma;
            local_affinity = g_osal_internal_thread_affinity;
        }
        else
        {
            /* Find the corresponding stack. */
            /* PRQA S 0316 1 # No problem because cast from void * to object is allowed. */
            p_control  = (st_osal_thread_control_t *)p_arg;
            while ((gs_osal_thread_stack_config_num > stack_loop_count) && (NULL == local_addr_stack))
            {
                if(p_control->usr_config->id == g_osal_thread_stack_config[stack_loop_count].id)
                {
                    local_addr_stack = g_osal_thread_stack_config[stack_loop_count].addr_stack;
                }
                else
                {
                    /* Do nothing */
                }
                stack_loop_count++;
            }

            /* Find the affinity setting if there is the corresponding affinity setting for the target thread. */
            while (gs_osal_thread_affinity_num > affinity_loop_count)
            {
                if(p_control->usr_config->id == g_osal_thread_affinity[affinity_loop_count].id)
                {
                    local_affinity = g_osal_thread_affinity[affinity_loop_count].affinity;
                }
                else
                {
                    /* Do nothing */
                }
                affinity_loop_count++;
            }
        }
        
        if(NULL != local_addr_stack)
        {
            os_ret = pthread_attr_setstack(&thread_attr->attr, local_addr_stack, local_stack_size);
            
            if(os_ret == 0)
            {
                /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_ThreadWaitFinish(). */
                /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
                p_local_thread = (st_osal_os_thread_t*)calloc(1, sizeof(st_osal_os_thread_t));
                if (NULL == p_local_thread)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    os_ret = pthread_create(&p_local_thread->thread, &thread_attr->attr, func, p_arg);
                    if (0 != os_ret)
                    {
                        osal_ret = OSAL_RETURN_FAIL;
                        /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                        /* PRQA S 5118 1 # Memory deallocation functions are available during initialization. */
                        free((void*)p_local_thread);
                    }
                    else
                    {
                        /* Set the CPU affinity setting if it exists. */
                        if((uint8_t)0 != local_affinity)
                        {
                            /* PRQA S 3335 2 # QAC false indication */
                            /* PRQA S 0316 1 # There is no problem because p_local_thread->thread stores ID of thread. */
                            k_thread_suspend((k_tid_t)p_local_thread->thread);
                            
                            /* PRQA S 3335 2 # QAC false indication */
                            /* PRQA S 0316 1 # There is no problem because p_local_thread->thread stores ID of thread. */
                            os_ret = k_thread_cpu_mask_clear((k_tid_t)p_local_thread->thread);
                            if(0 != os_ret)
                            {
                                osal_ret = OSAL_RETURN_FAIL;
                            }
                            else
                            {
                                for(bit_loop = 0; bit_loop < OSAL_THREAD_AFFINITY_MAX_BIT; bit_loop++)
                                {
                                    result_bit = ((local_affinity >> bit_loop) & 1U);
                                    if((uint8_t)0 != result_bit)
                                    {
                                        /* PRQA S 3335 2 # QAC false indication */
                                        /* PRQA S 0316 1 # There is no problem because p_local_thread->thread stores ID of thread. */
                                        os_ret = k_thread_cpu_mask_enable((k_tid_t)p_local_thread->thread, bit_loop);
                                        if(0 != os_ret)
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
                            }

                            /* PRQA S 0316 1 # There is no problem because p_local_thread->thread stores ID of thread. */
                            k_thread_resume((k_tid_t)p_local_thread->thread);
                        }
                        else
                        {
                            /* Do nothing */
                        }

                        if(OSAL_RETURN_OK != osal_ret)
                        {
                            /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                            /* PRQA S 5118 1 # Memory deallocation functions are available during initialization. */
                            free((void*)p_local_thread);
                        }
                        else
                        {
                            /* Set up the target thread name. */
                            os_ret = pthread_setname_np(p_local_thread->thread, p_config->task_name);
                            if(0 != os_ret)
                            {
                                osal_ret = OSAL_RETURN_FAIL;
                                /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                                /* PRQA S 5118 1 # Memory deallocation functions are available during initialization. */
                                free((void*)p_local_thread);
                            }
                            else
                            {
                                *p_thread = p_local_thread;
                            }
                        }
                    }
                }
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }   /* end of if ((NULL == p_config) || (NULL == func) || (NULL == p_arg) || (NULL == p_thread)). */

    return osal_ret;
/* PRQA S 9104,9107,9110 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadStaticCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadWaitFinish()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_ThreadWaitFinish(osal_os_thread_t thread)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret;
    int              os_ret;    /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec  time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == thread)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = clock_gettime(CLOCK_REALTIME, &time_stamp);
        if ((0 != os_ret) || (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Set up the timeout timestamp. */
            time_stamp.tv_sec  += (time_t)(OSAL_TH_JOIN_TIMEOUT / OSAL_SECTOMSEC);
            /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
            time_stamp.tv_nsec += (long)((OSAL_TH_JOIN_TIMEOUT % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);
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
            }   /* end of if ((time_stamp.tv_nsec % OSAL_MSECTONSEC) != 0L) */

            osal_ret = r_osal_os_thread_timedjoin(thread->thread, &time_stamp);
            if (OSAL_RETURN_OK == osal_ret)
            {
                /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
                free((void*)thread);
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if ((0 != os_ret) || (OSAL_MAX_TV_NSEC <= time_stamp.tv_nsec)) */
    }   /* end of if (NULL == thread) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadWaitFinish()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSleep()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSleep(const st_osal_time_t* const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret   = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = clock_gettime(CLOCK_REALTIME, &time_stamp);
        if ((0 != os_ret) || (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            time_stamp.tv_sec  = p_time_stamp->tv_sec - time_stamp.tv_sec;
            time_stamp.tv_nsec = p_time_stamp->tv_nsec - time_stamp.tv_nsec;

            if (0 > time_stamp.tv_nsec)
            {
                time_stamp.tv_sec -= 1;
                time_stamp.tv_nsec = OSAL_SECTONSEC + time_stamp.tv_nsec;
            }

            if (0 > time_stamp.tv_sec)
            {
                /* Do nothing */
            }
            else
            {
                os_ret = nanosleep(&time_stamp, NULL);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
        }

    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadSleep()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadInitAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadInitAttr(e_osal_thread_priority_t priority,
                                         size_t stack_size,
                                         osal_os_threadattr_t* p_thread_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret = OSAL_RETURN_OK;
    st_osal_os_threadattr_t* p_local_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)stack_size; /* unused */
    if (NULL == p_thread_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_ThreadDeinitAttr(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_local_attr = (st_osal_os_threadattr_t*)calloc(1, sizeof(st_osal_os_threadattr_t));
        if (NULL == p_local_attr)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if (NULL == p_thread_attr) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_thread_init_attr(priority, p_local_attr);
        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)pthread_attr_destroy(&p_local_attr->attr);
            /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
            /* PRQA S 5118 1 # Memory deallocation functions are available during initialization. */
            free((void *)p_local_attr);
        }
        else
        {
            /* PRQA S 2916 1 # Since it is the address of the dynamically allocated area, there is no problem. */
            *p_thread_attr = p_local_attr;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadInitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadDeinitAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadDeinitAttr(osal_os_threadattr_t thread_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == thread_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_attr_destroy(&thread_attr->attr);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
            /* PRQA S 5118 1 # Memory deallocation functions are available during initialization. */
            free((void *)thread_attr);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadDeinitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSetSelf()
***********************************************************************************************************************/
/* PRQA S 3673 1 # It does not rewrite the arguments. */
e_osal_return_t R_OSAL_OS_ThreadSetSelf(const osal_os_thread_t thread, void *p_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == thread) || (NULL == p_arg))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if (NULL == gsp_osal_thread_self_control)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (gs_osal_thread_self_control_num > gs_osal_thread_self_register_num)
        {
            gsp_osal_thread_self_control[gs_osal_thread_self_register_num].self_thread.thread = thread->thread;
            gsp_osal_thread_self_control[gs_osal_thread_self_register_num].p_arg              = p_arg;
            gs_osal_thread_self_register_num++;
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadSetSelf()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadStaticSelf()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadStaticSelf(void **pp_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    size_t          loop_count;
    pthread_t       self_thread_id;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == pp_arg) || (NULL == gsp_osal_thread_self_control))
    {
         osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        self_thread_id = pthread_self();

        loop_count = 0;
        while (gs_osal_thread_self_register_num > loop_count)
        {
            os_ret = pthread_equal(self_thread_id, gsp_osal_thread_self_control[loop_count].self_thread.thread);
            if (0 != os_ret)
            {
                *pp_arg = gsp_osal_thread_self_control[loop_count].p_arg;
                break;
            }
            else
            {
                /* Do nothing */
            }
            loop_count++;
        }   /* end of while ((OSAL_RETURN_OK == osal_ret) && (gs_osal_thread_self_register_num > loop_count)) */

        /* It is not a thread created using the OSAL API. */
        if (gs_osal_thread_self_register_num == loop_count)
        {
            *pp_arg  = NULL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if ((NULL == pp_arg) || (NULL == gsp_osal_thread_self_control)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadStaticSelf()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSetCpuAffinity()
***********************************************************************************************************************/
/*Note: This function exists only for compatibility with the OSAL common part and only returns OSAL_RETURN_OK. 
        There is no actual processing for the Zephyr OSAL wrapper. */
e_osal_return_t R_OSAL_OS_ThreadSetCpuAffinity(osal_thread_id_t id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void) id;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadSetCpuAffinity()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_TH_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSetTaskName()
***********************************************************************************************************************/
/*Note: This function exists only for compatibility with the OSAL common part and only returns OSAL_RETURN_OK. 
        There is no actual processing for the Zephyr OSAL wrapper. */
e_osal_return_t R_OSAL_OS_ThreadSetTaskName(const char* p_task_name)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void) p_task_name;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadSetTaskName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_TH_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_osal_os_thread_init_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_thread_init_attr(e_osal_thread_priority_t priority,
                                                       st_osal_os_threadattr_t* p_local_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret   = OSAL_RETURN_OK;
    int                 os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct sched_param  th_priority;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_attr_init(&p_local_attr->attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_attr_setschedpolicy(&p_local_attr->attr, SCHED_RR);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    } /* end of if (0 != os_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        th_priority.sched_priority = (int)priority + (int)OSAL_THREAD_PRIO_BASE;
        os_ret = pthread_attr_setschedparam(&p_local_attr->attr, &th_priority);
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
* End of function r_osal_os_thread_init_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_osal_os_thread_timedjoin()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the specification. */
OSAL_STATIC e_osal_return_t r_osal_os_thread_timedjoin(pthread_t thread, const struct timespec* abstime)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret    = OSAL_RETURN_OK;
    int              os_ret;    /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    /* PRQA S 3679 1 # Set the return value of pthread_self(). const should not be granted. */
    pthread_t  self_id;
    /* PRQA S 316 1 # pthread_t is convertible to posix_thread. */
    struct posix_thread *pthread = (struct posix_thread *) thread;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    self_id = pthread_self();
    if (self_id == pthread)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_mutex_lock(&pthread->state_lock);
        if (0 == os_ret)
        {
            if (PTHREAD_JOINABLE == pthread->state)
            {
                os_ret = pthread_cond_timedwait(&pthread->state_cond, &pthread->state_lock, abstime);
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

            if (PTHREAD_EXITED != pthread->state)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&pthread->state_lock);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                if (OSAL_RETURN_OK == osal_ret)
                {
                    os_ret = pthread_detach(thread);
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
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
/* PRQA S 9110 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function r_osal_os_thread_timedjoin()
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
