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
* File Name :    r_osal_os_thread.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Thread Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/neutrino.h>

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
 * @typedef st_osal_thread_self_control_t
 * @brief structure to self thread control.
 **********************************************************************************************************************/
typedef struct
{
    st_osal_os_thread_t self_thread;
    void                *p_arg;
}st_osal_thread_self_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_thread_affinity_addr_t
 * @brief structure to thread's affinity table address.
 **********************************************************************************************************************/
typedef struct
{
    const st_osal_thread_affinity_t *p_affinity_info;
} st_osal_thread_affinity_addr_t;

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
OSAL_STATIC size_t gs_osal_thread_self_control_num = 0U;

/***********************************************************************************************************************
 * @var gs_osal_thread_self_register_num
 * @brief Number of the thread self control information.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_thread_self_register_num = 0U;

/***********************************************************************************************************************
 * @var gspp_osal_thread_affinity_addr
 * @brief A pointer of thread's CPU Affinity table.
***********************************************************************************************************************/
OSAL_STATIC st_osal_thread_affinity_addr_t *gsp_osal_thread_affinity_addr = NULL;

/***********************************************************************************************************************
 * @var gs_osal_thread_affinity_num
 * @brief Number of the thread's CPU Affinity table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_thread_affinity_num = 0U;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Sort thread's CPU Affinity table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_thread_sort_affinity(void);

/***********************************************************************************************************************
* @brief            Initialize attribute of Thread.
* @param[in]        priority        The priority.
* @param[in]        stack_size      The stack size.
* @param[in,out]    p_local_attr    The pointer to st_osal_os_threadattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_thread_init_attr(e_osal_thread_priority_t priority,
                                                       size_t stack_size,
                                                       st_osal_os_threadattr_t* p_local_attr);

/***********************************************************************************************************************
* @brief            Set CPU Affinity to user thread.
* @param[in]        id              OSAL Thread ID.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_thread_set_affinity(osal_thread_id_t id);

/***********************************************************************************************************************
* @brief            Compare function for the argument of bsearch().
* @param[in]        p_key           The key.
* @param[in]        p_search_info   The search information.
* @return           int
* @retval           Returns the number value of the comparison result.
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_os_thread_search(const void *p_key, 
                                        const void *p_search_info);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_001
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
    int32_t         loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((1 > g_osal_thread_priority_matrix[0]) ||
        ((255 - OSAL_INTERRUPT_PRIORITY_MATRIX_SIZE) < g_osal_thread_priority_matrix[(OSAL_THREAD_PRIORITY_MATRIX_SIZE - 1)]))
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else if (g_osal_thread_priority_matrix[(OSAL_THREAD_PRIORITY_MATRIX_SIZE - 1)] >= g_osal_interrupt_thread_priority_matrix[0])
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        for (loop_count = 1; OSAL_THREAD_PRIORITY_MATRIX_SIZE > loop_count; loop_count++)
        {
            if (g_osal_thread_priority_matrix[(loop_count - 1)] >= g_osal_thread_priority_matrix[loop_count])
            {
                osal_ret = OSAL_RETURN_CONF;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if ((1 > g_osal_thread_priority_matrix[0]). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        gs_osal_thread_affinity_num = 0U;
        while ((uint64_t)OSAL_THREAD_AFFINITY_INVALID_ID != g_osal_thread_affinity[gs_osal_thread_affinity_num].id)
        {
            gs_osal_thread_affinity_num++;
        }

        if (0U != gs_osal_thread_affinity_num)
        {
            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            gsp_osal_thread_affinity_addr = (st_osal_thread_affinity_addr_t *)calloc(gs_osal_thread_affinity_num,
                                                                                     sizeof(st_osal_thread_affinity_addr_t));
            if (NULL == gsp_osal_thread_affinity_addr)
            {
                osal_ret = OSAL_RETURN_MEM;
                gs_osal_thread_affinity_num = 0U;
            }
            else
            {
                osal_ret = r_osal_os_thread_sort_affinity();
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
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        gsp_osal_thread_self_control = (st_osal_thread_self_control_t *)calloc(1, sizeof(st_osal_thread_self_control_t) * thread_cfg_num);
        if (NULL == gsp_osal_thread_self_control)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            gs_osal_thread_self_control_num = thread_cfg_num;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadStaticInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_002
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
        gs_osal_thread_self_control_num  = 0U;
        gs_osal_thread_self_register_num = 0U;
    }
    else
    {
        /* Do nothing */
    }

    if (NULL != gsp_osal_thread_affinity_addr)
    {
        /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
        /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
        free((void*)gsp_osal_thread_affinity_addr);
        gsp_osal_thread_affinity_addr = NULL;
        gs_osal_thread_affinity_num   = 0U;
    }
    else
    {
        /* Do nothing */
    }

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadStaticDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_003
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
    e_osal_return_t     osal_ret         = OSAL_RETURN_OK;
    int                 os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    st_osal_os_thread_t *p_local_thread;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == p_config) || (NULL == func) || (NULL == p_arg) || (NULL == thread_attr) || (NULL == p_thread))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
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
                *p_thread = p_local_thread;
            }
        }
    }   /* end of if ((NULL == p_config) || (NULL == func) || (NULL == p_arg) || (NULL == p_thread)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadStaticCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_004
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
    e_osal_return_t  osal_ret     = OSAL_RETURN_OK;
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
        os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
        if ((0 != os_ret) || (OSAL_MAX_TV_NSEC <= time_stamp.tv_nsec))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
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

            os_ret = pthread_timedjoin_monotonic(thread->thread, NULL, &time_stamp);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
                free((void*)thread);
            }
        }   /* end of if ((0 != os_ret) || (OSAL_MAX_TV_NSEC <= time_stamp.tv_nsec)) */
    }   /* end of if (NULL == thread) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadWaitFinish()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_008
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
    struct timespec abstime;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        abstime.tv_sec  = p_time_stamp->tv_sec;
        abstime.tv_nsec = p_time_stamp->tv_nsec;

        /* PRQA S 5209,2855 1 # System is requesting an int, so it cannot be changed. */
        os_ret = clock_nanosleep(CLOCK_MONOTONIC, (int)TIMER_ABSTIME, &abstime, NULL);
        if (0 != os_ret)
        {
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
* End of function R_OSAL_OS_ThreadSleep()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_009
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
        osal_ret = r_osal_os_thread_init_attr(priority, stack_size, p_local_attr);
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_010
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_005
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_006
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_028]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSetCpuAffinity()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSetCpuAffinity(osal_thread_id_t id)
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
    if ((osal_thread_id_t)OSAL_THREAD_INVALID_ID == id)
    {
        if (0x0U != g_osal_internal_thread_affinity)
        {
            /* PRQA S 0326 1 # No problem because it follows the specification of the calling function. */
            os_ret = ThreadCtl(_NTO_TCTL_RUNMASK, (void *)g_osal_internal_thread_affinity);
            if (-1 == os_ret)
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
    else if (0U != gs_osal_thread_affinity_num)
    {
        osal_ret = r_osal_os_thread_set_affinity(id);
    }
    else
    {
        /* Do nothing */
    }   /* end of if ((osal_thread_id_t)OSAL_THREAD_INVALID_ID == id). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadSetCpuAffinity()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSetTaskName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSetTaskName(const char* p_task_name)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    char            task_name[OSAL_TASK_NAME_MAX_LENGTH + 1UL];   /* task name + \0 */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
    (void)memset((void*)task_name, 0x00, sizeof(task_name));

    if (NULL != p_task_name)
    {
        /* Copy up to 15 characters */
        (void)strncpy(task_name, p_task_name, (size_t)OSAL_TASK_NAME_MAX_LENGTH);
    }
    else
    {
        /* Do nothing */
    }

    /* Set to the calling thread. */
    os_ret = pthread_setname_np(0, task_name);
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
* End of function R_OSAL_OS_ThreadSetTaskName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_thread_sort_affinity()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_ThreadStaticInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_thread_sort_affinity(void)
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
    gsp_osal_thread_affinity_addr[0U].p_affinity_info = &g_osal_thread_affinity[0U];
    for (loop_count = 1U; ((loop_count < gs_osal_thread_affinity_num) && (OSAL_RETURN_OK == osal_ret)); loop_count++)
    {
        tmp = g_osal_thread_affinity[loop_count].id;
        for (cmp_count = loop_count; ((cmp_count > 0U) && (OSAL_RETURN_OK == osal_ret)); cmp_count--)
        {
            if (gsp_osal_thread_affinity_addr[(cmp_count - 1U)].p_affinity_info->id > tmp)
            {
                gsp_osal_thread_affinity_addr[cmp_count].p_affinity_info = gsp_osal_thread_affinity_addr[(cmp_count - 1U)].p_affinity_info;
            }
            else if (gsp_osal_thread_affinity_addr[(cmp_count - 1U)].p_affinity_info->id == tmp)
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                break;
            }
        }
        gsp_osal_thread_affinity_addr[cmp_count].p_affinity_info = &g_osal_thread_affinity[loop_count];
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_thread_sort_affinity()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_thread_init_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_ThreadInitAttr() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_thread_init_attr(e_osal_thread_priority_t priority,
                                                       size_t stack_size,
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
        os_ret = pthread_attr_setinheritsched(&p_local_attr->attr, PTHREAD_EXPLICIT_SCHED);
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
        os_ret = pthread_attr_setschedpolicy(&p_local_attr->attr, SCHED_RR);
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
        th_priority.sched_priority = (int)g_osal_thread_priority_matrix[priority];
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_attr_setstacksize(&p_local_attr->attr, (size_t)(stack_size + (size_t)PTHREAD_STACK_MIN));
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_thread_set_affinity()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_ThreadSetCpuAffinity() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_thread_set_affinity(osal_thread_id_t id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                       osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                   os_ret;
    const st_osal_thread_affinity_addr_t  *p_affinity_addr;
    osal_thread_id_t                      local_id = id;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5135 3 # The compare function does not modify array and return consistent comparison results. */
    /* PRQA S 0314 3 # No problem because it follows the specification of the calling function. */
    /* PRQA S 0316 1 # No problem because it is returned to the type of the second argument. */
    p_affinity_addr = (const st_osal_thread_affinity_addr_t *)bsearch((const void *)&local_id,
                                                                      (const void *)gsp_osal_thread_affinity_addr,
                                                                      gs_osal_thread_affinity_num,
                                                                      sizeof(st_osal_thread_affinity_addr_t),
                                                                      r_osal_os_thread_search);

    if (NULL != p_affinity_addr)
    {
        if (0x0U != p_affinity_addr->p_affinity_info->affinity)
        {
            /* PRQA S 0326 1 # No problem because it follows the specification of the calling function. */
            os_ret = ThreadCtl(_NTO_TCTL_RUNMASK, (void *)p_affinity_addr->p_affinity_info->affinity);
            if (-1 == os_ret)
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
    }   /* end of if (NULL != p_affinity_addr). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_thread_set_affinity()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_TH_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_thread_search()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_os_thread_search(const void *p_key, 
                                        const void *p_search_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 2 # There is no problem because it is just restored to the type at the time of function call. */
    osal_thread_id_t                     key              = *(const osal_thread_id_t *)p_key;
    const st_osal_thread_affinity_addr_t *p_affinity_addr = (const st_osal_thread_affinity_addr_t *)p_search_info;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                                  result;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (key < p_affinity_addr->p_affinity_info->id)
    {
        result = -1;
    }
    else if (key > p_affinity_addr->p_affinity_info->id)
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
* End of function r_osal_os_thread_search()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_TH_030]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
