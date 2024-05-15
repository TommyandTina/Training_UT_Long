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
* File Name :    r_osal_os_mutex.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for Mutex
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <errno.h>
#include <pthread.h>
#include "target/linux/r_osal_os_types.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexCreate()
***********************************************************************************************************************/
/* PRQA S 3673 2 # Implemented in accordance with the requirement. */
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexCreate(const osal_os_mutexattr_t mutex_attr, osal_os_mutex_t* p_mutex)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                 linux_ret;

    if ((NULL == mutex_attr) ||
        (NULL == p_mutex))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        *p_mutex = (osal_os_mutex_t)R_OSAL_OS_Malloc(sizeof(st_osal_os_mutex_t));
        if (NULL == *p_mutex)
        {
            osal_ret = OSAL_RETURN_MEM;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2736 1 # The mutex is destroyed in caller managers. */
            linux_ret = pthread_mutex_init(&(*p_mutex)->mutex, &mutex_attr->attr);
            if (0 != linux_ret)
            {
                /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
                R_OSAL_OS_Free((void *)*p_mutex);
                *p_mutex = NULL;
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_102
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexDestroy()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexDestroy(osal_os_mutex_t mutex)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == mutex)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_mutex_destroy(&mutex->mutex);
        switch (linux_ret)
        {
            case 0 :
                break;

            case EBUSY :
                osal_ret = OSAL_RETURN_BUSY;
                break;

            default :
                osal_ret = OSAL_RETURN_FAIL;
                break;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)mutex);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexDestroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_026]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_103
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexTimedLock()
***********************************************************************************************************************/
/* PRQA S 3673 2 # Implemented in accordance with the requirement. */
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexTimedLock(osal_os_mutex_t mutex, const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    struct timespec abstime;

    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (NULL == mutex)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        abstime.tv_sec  = p_time_stamp->tv_sec;
        abstime.tv_nsec = p_time_stamp->tv_nsec;

        linux_ret = pthread_mutex_timedlock(&mutex->mutex, &abstime);
        switch (linux_ret)
        {
            case 0 :
                osal_ret = OSAL_RETURN_OK;
                break;

            case ETIMEDOUT :
                osal_ret = OSAL_RETURN_TIME;
                break;

            case EINVAL :
                osal_ret = OSAL_RETURN_HANDLE;
                break;

            default :
                osal_ret = OSAL_RETURN_FAIL;
                break;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexTimedLock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_104
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexTryLock()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexTryLock(osal_os_mutex_t mutex)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == mutex)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_mutex_trylock(&mutex->mutex);
        switch (linux_ret)
        {
            case 0 :
                osal_ret = OSAL_RETURN_OK;
                break;

            case EBUSY :
                osal_ret = OSAL_RETURN_BUSY;
                break;

            case EINVAL :
                osal_ret = OSAL_RETURN_HANDLE;
                break;

            default :
                osal_ret = OSAL_RETURN_FAIL;
                break;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexTryLock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_105
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexUnlock()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexUnlock(osal_os_mutex_t mutex)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == mutex)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_mutex_unlock(&mutex->mutex);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexUnlock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_106
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexInitAttr()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexInitAttr(osal_os_mutexattr_t* p_mutex_attr)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == p_mutex_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        *p_mutex_attr = (osal_os_mutexattr_t)R_OSAL_OS_Malloc(sizeof(st_osal_os_mutexattr_t));
        if (NULL == *p_mutex_attr)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_mutexattr_init(&(*p_mutex_attr)->attr);
        if (0 != linux_ret)
        {
            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)*p_mutex_attr);
            *p_mutex_attr = NULL;
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_mutexattr_settype(&(*p_mutex_attr)->attr, PTHREAD_MUTEX_ERRORCHECK);
        if (0 != linux_ret)
        {
            (void)R_OSAL_OS_MutexDeinitAttr(*p_mutex_attr);
            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)*p_mutex_attr);
            *p_mutex_attr = NULL;
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexInitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_107
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexDeinitAttr()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexDeinitAttr(osal_os_mutexattr_t mutex_attr)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == mutex_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_mutexattr_destroy(&mutex_attr->attr);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)mutex_attr);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexDeinitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_038]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
