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
* File Name :    r_osal_os_thread.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for Thread
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
/* PRQA S 0602, 0603, 3214 1 # This definition is required to use glibc. */
#define _GNU_SOURCE
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "target/linux/r_osal_os_types.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Set the attribute for the thread.
 * @param[in]        p_attr      A pointer to the .
 * @param[in]        p_config    A pointer to the .
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_thread_init_attr(pthread_attr_t* p_attr, const st_osal_thread_config_t* const p_config);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadCreate()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_ThreadCreate(const st_osal_thread_config_t* const p_config, p_osal_os_thread_func_t func, void* p_arg, osal_os_thread_t* p_thread)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                 linux_ret;
    /* PRQA S 0759 1 # This object is used in accordance with the specification of pthread. */
    pthread_attr_t      attr;

    if ((NULL == p_config) ||
        (NULL == func) ||
        (NULL == p_arg) || /* p_arg is internal parameter. Never be NULL. */
        (NULL == p_thread))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_thread_init_attr(&attr, p_config);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0316 1 # Required cast on system, not modifiable. */
            *p_thread = (osal_os_thread_t)R_OSAL_OS_Malloc(sizeof(st_osal_os_thread_t));
            if (NULL == *p_thread)
            {
                osal_ret = OSAL_RETURN_MEM;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* PRQA S 0314 1 # The 4th arguments must be (void *) but no side effects in this case. */
                linux_ret = pthread_create(&(*p_thread)->thread, &attr, func, (void *)p_arg);
                if (0 != linux_ret)
                {
                    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
                    R_OSAL_OS_Free((void *)*p_thread);
                    *p_thread = NULL;
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }

            linux_ret = pthread_attr_destroy(&attr);
            if (0 != linux_ret)
            {
                /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
                R_OSAL_OS_Free((void *)*p_thread);
                *p_thread = NULL;
                osal_ret = OSAL_RETURN_FAIL;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_os_thread_init_attr */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from if ((NULL == p_config) || (NULL == func) || (NULL == p_arg) || (NULL == p_thread)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_102
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadWaitFinish()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_ThreadWaitFinish(osal_os_thread_t thread)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == thread)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_join(thread->thread, NULL); /* PRQA S 2810 # Following the specification. */
        switch(linux_ret)
        {
            case 0 :
                break;

            case ESRCH :
            case EINVAL :
            case EDEADLK :
                osal_ret = OSAL_RETURN_HANDLE;
                break;

            default :
                osal_ret = OSAL_RETURN_FAIL;
                break;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)thread);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadWaitFinish()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_103
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSelf()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSelf(osal_os_thread_t* p_self_id)
{
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    /* Thread manager configurator is exclusive control to this function.   */
    /* So, this function does not run from the multiple thread.             */
    static st_osal_os_thread_t  st_thread;

    if (NULL == p_self_id)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_self_id = &st_thread;
        st_thread.thread = pthread_self();
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadSelf()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_104
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadEqual()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_ThreadEqual(osal_os_thread_t thread1, osal_os_thread_t thread2, bool* const p_result)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if ((NULL == thread1) ||
        (NULL == thread2) ||
        (NULL == p_result))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_equal(thread1->thread, thread2->thread);
        if (0 != linux_ret)
        {
            *p_result = true;
        }
        else
        {
            *p_result = false;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadEqual()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_105
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSleep()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_ThreadSleep(const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    struct timespec abstime;

    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        abstime.tv_sec  = p_time_stamp->tv_sec;
        abstime.tv_nsec = p_time_stamp->tv_nsec;

        linux_ret = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &abstime, NULL);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadSleep()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_106
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSetCpuAffinity()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSetCpuAffinity(osal_thread_id_t id)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    uint64_t        cnt;
    cpu_set_t       cpu_set;
    uint64_t        affinity = 0;

    cnt = 0;
    while (OSAL_THREAD_AFFINITY_INVALID_ID != g_osal_thread_affinity[cnt].id)
    {
        if (id == g_osal_thread_affinity[cnt].id)
        {
            affinity = g_osal_thread_affinity[cnt].affinity;
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
* End of function R_OSAL_OS_ThreadSetCpuAffinity()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_107
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_ThreadSetTaskName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSetTaskName(const char* p_task_name)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    /* Maximum length of thread name is 16byte (include \0) */
    char            thread_name[16U];

    if (NULL != p_task_name)
    {
        /* Truncation to meet linux specification */
        (void)strncpy(thread_name, p_task_name, sizeof(thread_name));
        thread_name[sizeof(thread_name) - 1U] = '\0';

        linux_ret = pthread_setname_np(pthread_self(), thread_name);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_ThreadSetTaskName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_TH_108
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_thread_init_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_thread_init_attr(pthread_attr_t* p_attr, const st_osal_thread_config_t* const p_config)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                 linux_ret;
    struct sched_param  thread_priority;

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
            thread_priority.sched_priority = (int)p_config->priority + OSAL_THREAD_PRIORITY_BASE;

            linux_ret = pthread_attr_setschedparam(p_attr, &thread_priority);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            linux_ret = pthread_attr_setstacksize(p_attr, p_config->stack_size + (size_t)PTHREAD_STACK_MIN);
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
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function r_osal_os_thread_init_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_TH_015]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
